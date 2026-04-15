export module serex.numbers;
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


// template <typename T>
// requires std::is_enum_v<T>
// struct serex::Serializer<T> {
//     static auto Save(const T &obj) -> std::string {
//         using Underlying = std::underlying_type_t<T>;
//         Underlying val = static_cast<Underlying>(obj);
//         auto buffer = std::string(sizeof(Underlying), '\0');
//         std::memcpy(buffer.data(), &val, sizeof(Underlying));
//         return buffer;
//     }
//
//     static auto Load(const std::string &s) -> T {
//         using Underlying = std::underlying_type_t<T>;
//         Underlying val;
//         std::memcpy(&val, s.data(), sizeof(Underlying));
//         return static_cast<T>(val);
//     }
// };
