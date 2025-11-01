export module serex.polymorphic;
import std;


export namespace serex {
    template <typename Base, typename Derived> requires std::derived_from<Derived, Base>
    auto base_object(Derived *d) -> Base& {
        return *d;
    }

    struct SerializablePolymorphicBase;

    template <typename To, typename From>
    requires std::derived_from<From, SerializablePolymorphicBase> and std::derived_from<To, From>
    auto poly_owning_cast(std::unique_ptr<From> &&from) -> std::unique_ptr<To> {
        return dynamic_cast<To*>(from.release());
    }

    template <typename To, typename From>
    requires std::derived_from<From, SerializablePolymorphicBase> and std::derived_from<To, From>
    auto poly_non_owning_cast(const std::unique_ptr<From> &from) -> To* {
        return dynamic_cast<To*>(from.get());
    }
}


struct serex::SerializablePolymorphicBase {
    virtual ~SerializablePolymorphicBase() = default;
    SerializablePolymorphicBase() = default;

    virtual auto serex_type() -> std::string = 0;

    inline static std::unordered_map<std::string, std::function<std::unique_ptr<SerializablePolymorphicBase>()>> registry;

    template <typename A>
    auto save(A &ar) -> void {
        ar & serex_type();
    }

    template <typename A>
    auto load(A &ar) -> void {
        return ar & registry[serex_type()]();
    }
};


export namespace serex {
    template <typename T>
    auto register_polymorphic_type(const std::string &type_name) -> void {
        SerializablePolymorphicBase::registry[type_name] = [] { return std::make_unique<T>(); };
    }
}
