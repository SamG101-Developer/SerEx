export module serex.polymorphic;
import serex.archive;
import serex.numbers;
import serex.string;
import std;


export namespace serex {
    struct SerializablePolymorphicBase;

    std::unordered_map<std::size_t, SerializablePolymorphicBase*> pointer_map;

    template <typename To, typename From>
    requires std::derived_from<From, SerializablePolymorphicBase> and std::derived_from<To, From>
    auto poly_owning_cast(std::unique_ptr<From> &&from) -> std::unique_ptr<To> {
        return std::unique_ptr<To>(dynamic_cast<To*>(from.release()));
    }

    template <typename To, typename From>
    requires std::derived_from<From, SerializablePolymorphicBase> and std::derived_from<To, From>
    auto poly_non_owning_cast(const std::unique_ptr<From> &from) -> To* {
        return dynamic_cast<To*>(from.get());
    }
}


struct serex::SerializablePolymorphicBase {
    virtual ~SerializablePolymorphicBase() = default;
    std::size_t identifier = 0;
    static inline std::size_t next_id = 1;

    SerializablePolymorphicBase() {
        identifier = next_id++;
        pointer_map[identifier] = this;
    }

    virtual auto serex_type() -> std::string = 0;

    virtual auto serialize(Archive &ar) -> void = 0;

    inline static std::unordered_map<std::string, std::function<std::unique_ptr<SerializablePolymorphicBase>()>> registry;
};



template <typename T>
struct serex::Serializer<std::unique_ptr<T>> {
    static auto save(std::unique_ptr<T> const &obj) -> std::string {
        // auto raw_ptr = obj.get();
        // auto ptr_id = serex::save(raw_ptr);
        // return obj->serex_type() + "\n" + ptr_id + "\n" + Serializer<T>::save(*obj);
        return obj->serex_type() + "\n" + Serializer<T>::save(*obj);
    }

    static auto load(const std::string &s) -> std::unique_ptr<T> {
        // Get the "std::unique_ptr" of the correct derived type.
        const auto end_type = s.find('\n');
        const auto type = s.substr(0, end_type);
        auto ptr = SerializablePolymorphicBase::registry[type]();

        // If the id is in the list, serialize it from there, otherwise load normally.
        // const auto end_id = s.find('\n', end_type + 1);
        // const auto id_str = s.substr(end_type + 1, end_id - end_type - 1);
        // const auto id = std::stoull(id_str);
        // if (pointer_map.contains(id)) {
        //     return std::unique_ptr<T>(dynamic_cast<T*>(pointer_map[id]));
        // }
        auto &m = pointer_map;
        pointer_map[ptr->identifier] = ptr.get();

        // Load the object data into the derived type object.
        // auto archive = IArchive(s.substr(end_id + 1));
        auto archive = IArchive(s.substr(end_type + 1));
        ptr->serialize(archive);
        return std::unique_ptr<T>(dynamic_cast<T*>(ptr.release()));
    }
};


export namespace serex {
    template <typename T>
    auto register_polymorphic_type(const std::string &type_name) -> void {
        SerializablePolymorphicBase::registry[type_name] = [] { return std::make_unique<T>(); };
    }
}
