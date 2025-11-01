export module serex.boolean;
import std;

import serex.archive;


template <>
struct serex::Serializer<bool> {
    static auto save(const bool &obj) -> std::string {
        return obj ? "t" : "f";
    }

    static auto load(const std::string &s) -> bool {
        return s == "t";
    }
};
