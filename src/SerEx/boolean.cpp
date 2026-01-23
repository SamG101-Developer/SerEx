export module serex.boolean;
import std;

import serex.archive;


template <>
struct serex::Serializer<bool> {
    static auto save(const bool &obj) -> std::string {
        return obj ? "1" : "0";
    }

    static auto load(const std::string &s) -> bool {
        return s == "1";
    }
};
