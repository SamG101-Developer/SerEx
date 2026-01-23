export module serex.pointer;
import std;
import serex.archive;
import serex.polymorphic;


/**
 * Pointer serialization is more complex, because we want to be able to maintain pointer relations after serialization.
 * For example, two pointers pointing to tghe same data before, need to point to the same data afterwards. We therefore
 * require that all pointers being serialized inherit from @c serex::SerializablePointer, so we can use the identifier
 * on that type in a pointer map, allowing for easy retrieval.
 */
template <typename T>
requires std::is_pointer_v<T>
struct serex::Serializer<T> {
    static_assert(std::derived_from<std::remove_pointer_t<T>, SerializablePolymorphicBase>,
        "Cannot serialize raw pointers; pointer type must derive from serex::SerializablePointer");

    static auto save(const T &obj) -> std::string {
        if (obj == nullptr) { return "0\n"; }
        return std::to_string(obj->identifier) + "\n";
    }

    static auto load(const std::string &s) -> T {
        const auto id_end = s.find('\n');
        const auto id_str = s.substr(0, id_end);
        const auto id = std::stoull(id_str);
        if (id == 0) { return nullptr; }
        return static_cast<T>(pointer_map[id]);
    }
};


/**
 * Same as above, but for shared pointers.
 */
template <typename T>
struct serex::Serializer<std::shared_ptr<T>> {
    static_assert(std::derived_from<T, SerializablePolymorphicBase>,
        "Cannot serialize shared pointers; pointer type must derive from serex::SerializablePointer");

    static auto save(const std::shared_ptr<T> &obj) -> std::string {
        return Serializer::save(obj.get());
    }

    static auto load(const std::string &s) -> std::shared_ptr<T> {
        auto raw = Serializer<T*>::load(s);
        return std::shared_ptr<T>(static_cast<T*>(raw));
    }
};
