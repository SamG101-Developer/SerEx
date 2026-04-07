export module serex.polymorphic;
import serex.archive;
import serex.string;
import std;


export namespace serex {
    struct SerializablePolymorphicBase;

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

    virtual auto SerexType() -> std::string = 0;

    virtual auto Serialize(Archive &ar) -> void = 0;

    inline static std::unordered_map<std::string, std::function<std::unique_ptr<SerializablePolymorphicBase>()>> registry;
};



template <typename T>
struct serex::Serializer<std::unique_ptr<T>> {
    static auto Save(std::unique_ptr<T> const &obj) -> std::string {
        return obj->SerexType() + "\n" + Serializer<T>::Save(*obj);
    }

    static auto Load(const std::string &s) -> std::unique_ptr<T> {
        // Get the "std::unique_ptr" of the correct derived type.
        const auto end_type = s.find('\n');
        const auto type = s.substr(0, end_type);
        if (not SerializablePolymorphicBase::registry.contains(type))
            throw std::runtime_error("Type '" + type + "' not registered for polymorphic deserialization.");
        auto ptr = SerializablePolymorphicBase::registry[type]();

        // Load the object data into the derived type object.
        auto archive = IArchive(s.substr(end_type + 1));
        ptr->Serialize(archive);
        return std::unique_ptr<T>(dynamic_cast<T*>(ptr.release()));
    }
};


export namespace serex {
    template <typename T>
    auto register_polymorphic_type(const std::string &type_name) -> void {
        SerializablePolymorphicBase::registry[type_name] = [] { return std::make_unique<T>(); };
    }
}
