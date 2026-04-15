export module serex.enum_;
import serex.archive;
import std;


template <typename T>
requires (std::is_enum_v<T> or std::is_scoped_enum_v<T>)
struct serex::Serializer<T> {
    static auto Save(const T obj) -> std::string {
        using Underlying = std::underlying_type_t<T>;
        const auto val = std::to_underlying(obj);
        auto buffer = std::string(sizeof(Underlying), '\0');
        std::memcpy(buffer.data(), &val, sizeof(Underlying));
        return buffer;
    }

    static auto Load(const std::string &s) -> T {
        using Underlying = std::underlying_type_t<T>;
        Underlying val;
        std::memcpy(&val, s.data(), sizeof(Underlying));
        return std::bit_cast<T>(val);
    }
};
