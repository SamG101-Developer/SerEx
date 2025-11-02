export module serex.numbers;
import std;
import serex.archive;


/**
 * Because numbers of the same type don't always have the same length ("1" and "10" for example), numbers are copied
 * into buffers with fixed lengths. For "std::uint8_t", this is an 1-byte buffer.
 * @tparam T
 */
template <typename T>
requires std::integral<T> or std::floating_point<T>
struct serex::Serializer<T> {
    static auto save(T obj) -> std::string {
        auto buffer = std::string(sizeof(T), '\0');
        std::memcpy(buffer.data(), &obj, sizeof(T));
        return buffer;
    }

    static auto load(const std::string &s) -> T {
        T obj;
        std::memcpy(&obj, s.data(), sizeof(T));
        return obj;
    }
};
