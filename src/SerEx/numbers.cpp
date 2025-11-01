export module serex.numbers;
import std;
import serex.archive;


template <typename T> requires std::integral<T> or std::floating_point<T>
struct serex::Serializer<T> {
    static auto save(T obj) -> std::string {
        return std::to_string(obj);
    }

    static auto load(const std::string &s) -> T {
        if constexpr (std::same_as<T, int>) {
            return std::stoi(s);
        }
        else if constexpr (std::same_as<T, long>) {
            return std::stol(s);
        }
        else if constexpr (std::same_as<T, long long>) {
            return std::stoll(s);
        }
        else if constexpr (std::same_as<T, unsigned int>) {
            return static_cast<unsigned int>(std::stoul(s));
        }
        else if constexpr (std::same_as<T, unsigned long>) {
            return std::stoul(s);
        }
        else if constexpr (std::same_as<T, unsigned long long>) {
            return std::stoull(s);
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
