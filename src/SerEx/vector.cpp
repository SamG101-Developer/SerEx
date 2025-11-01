export module serex.vector;
import std;
import serex.archive;


template <typename T>
struct serex::Serializer<std::vector<T>> {
    static auto save(std::vector<T> const &obj) -> std::string {
        auto stream = std::string();
        for (const auto &item : obj) {
            auto partial = Serializer<T>::save(item);
            auto partial_size = partial.size();
            stream.append(reinterpret_cast<const char*>(&partial_size), sizeof(std::size_t));
            stream.append(partial);
        }
        return stream;
    }

    static auto load(const std::string &s) -> std::vector<T> {
        auto vec = std::vector<T>{};
        for (auto i = 0uz; i < s.size(); i += sizeof(T)) {
            std::size_t item_size;
            std::memcpy(&item_size, s.data() + i, sizeof(std::size_t));
            i += sizeof(std::size_t);
            auto item_data = s.substr(i, item_size);
            auto item = Serializer<T>::load(item_data);
            vec.push_back(std::move(item));
            i += item_size - sizeof(T);
        }
        return vec;
    }
};
