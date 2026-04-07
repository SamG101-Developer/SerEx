export module serex.boolean;
import std;

import serex.archive;


template <>
struct serex::Serializer<bool> {
    static auto Save(const bool &obj) -> std::string {
        return obj ? "1" : "0";
    }

    static auto Load(const std::string &s) -> bool {
        return s == "1";
    }
};
