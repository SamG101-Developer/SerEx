export module serex.boolean;
import serex.archive;
import std;


template <>
struct serex::Serializer<bool> {
    static auto Save(bool const &obj) -> std::string {
        return obj ? "1" : "0";
    }

    static auto Load(std::string const &s) -> bool {
        return s == "1";
    }
};
