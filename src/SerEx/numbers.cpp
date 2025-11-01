export module serex.numbers;
import std;
import serex.archive;


template <typename T> requires std::integral<T> or std::floating_point<T>
struct serex::Serializer<T> {
    static auto save(T obj) -> std::string {
        return std::to_string(obj);
    }

    static auto load(const std::string &s) -> T {
        if constexpr (std::same_as<T, std::uint8_t>) {
            return static_cast<T>(std::stoi(s));
        }
        else if constexpr (std::same_as<T, std::int8_t>) {
            return static_cast<T>(std::stoul(s));
        }
        else if constexpr (std::same_as<T, std::int16_t>) {
            return static_cast<T>(std::stoi(s));
        }
        else if constexpr (std::same_as<T, std::uint16_t>) {
            return static_cast<T>(std::stoul(s));
        }
        else if constexpr (std::same_as<T, std::int32_t>) {
            return static_cast<T>(std::stoi(s));
        }
        else if constexpr (std::same_as<T, std::uint32_t>) {
            return static_cast<T>(std::stoul(s));
        }
        else if constexpr (std::same_as<T, std::int64_t>) {
            return static_cast<T>(std::stoll(s));
        }
        else if constexpr (std::same_as<T, std::uint64_t>) {
            return static_cast<T>(std::stoull(s));
        }
        else if constexpr (std::same_as<T, float>) {
            return std::stof(s);
        }
        else if constexpr (std::same_as<T, double>) {
            return std::stod(s);
        }
        else if constexpr (std::same_as<T, long double>) {
            return std::stold(s);
        }
        else {
            std::unreachable();
        }
    }
};
