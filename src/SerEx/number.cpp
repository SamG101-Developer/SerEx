export module serex.number;
import serex.archive;
import std;


template <typename T>
requires (std::integral<T> or std::floating_point<T>) and (not std::same_as<T, bool>)
struct serex::Serializer<T> {
    static auto Save(const T obj) -> std::string {
        auto buffer = std::string(sizeof(T), '\0');
        std::memcpy(buffer.data(), &obj, sizeof(T));
        return buffer;
    }

    static auto Load(std::string const &s) -> T {
        T obj;
        std::memcpy(&obj, s.data(), sizeof(T));
        return obj;
    }
};
