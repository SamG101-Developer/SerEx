export module serex.string;
import serex.archive;
import std;


template <>
struct serex::Serializer<std::string> {
    static auto Save(std::string const &obj) -> std::string {
        return obj;
    }

    static auto Load(std::string const &s) -> std::string {
        return s;
    }
};

template <>
struct serex::Serializer<std::wstring> {
    static auto Save(std::wstring const &obj) -> std::string {
        std::string result;
        result.resize(obj.size() * sizeof(wchar_t));
        std::memcpy(result.data(), obj.data(), result.size());
        return result;
    }

    static auto Load(std::string const &s) -> std::wstring {
        std::wstring result;
        result.resize(s.size() / sizeof(wchar_t));
        std::memcpy(result.data(), s.data(), s.size());
        return result;
    }
};

template <>
struct serex::Serializer<std::u8string> {
    static auto Save(std::u8string const &obj) -> std::string {
        return {reinterpret_cast<const char*>(obj.data()), obj.size()};
    }

    static auto Load(std::string const &s) -> std::u8string {
        return {reinterpret_cast<const char8_t*>(s.data()), s.size()};
    }
};

template <>
struct serex::Serializer<std::u16string> {
    static auto Save(std::u16string const &obj) -> std::string {
        std::string result;
        result.resize(obj.size() * sizeof(char16_t));
        std::memcpy(result.data(), obj.data(), result.size());
        return result;
    }

    static auto Load(std::string const &s) -> std::u16string {
        std::u16string result;
        result.resize(s.size() / sizeof(char16_t));
        std::memcpy(result.data(), s.data(), s.size());
        return result;
    }
};

template <>
struct serex::Serializer<std::u32string> {
    static auto Save(std::u32string const &obj) -> std::string {
        std::string result;
        result.resize(obj.size() * sizeof(char32_t));
        std::memcpy(result.data(), obj.data(), result.size());
        return result;
    }

    static auto Load(std::string const &s) -> std::u32string {
        std::u32string result;
        result.resize(s.size() / sizeof(char32_t));
        std::memcpy(result.data(), s.data(), s.size());
        return result;
    }
};
