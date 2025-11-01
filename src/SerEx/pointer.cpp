export module serex.pointers;
import std;

import serex.archive;


template <typename T, typename D>
struct serex::Serializer<std::unique_ptr<T, D>> {
    static auto save(std::unique_ptr<T, D> const &obj) -> std::string {
        return Serializer<T>::save(*obj);
    }

    static auto load(const std::string &s) -> std::unique_ptr<T, D> {
        auto on_stack = Serializer<T>::load(s);
        return std::make_unique<T, D>(std::move(on_stack));
    }
};
