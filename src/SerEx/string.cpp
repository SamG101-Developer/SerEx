export module serex.string;
import std;

import serex.archive;


template <>
struct serex::Serializer<std::string> {
    static auto save(std::string const &obj) -> std::string {
        return obj;
    }

    static auto load(const std::string &s) -> std::string {
        return s;
    }
};
