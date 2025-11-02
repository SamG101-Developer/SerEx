export module serex.vector;
import std;
import serex.archive;


template <typename T, typename A>
requires std::is_trivially_copyable_v<T>
struct serex::Serializer<std::vector<T, A>> {
    static auto save(std::vector<T, A> const &obj) -> std::string {
        auto stream = std::string();
        auto vec_size = obj.size();
        stream.append(reinterpret_cast<const char*>(&vec_size), sizeof(vec_size));
        for (const auto &item : obj) {
            auto partial = Serializer<T>::save(item);
            stream.append(partial);
        }
        return stream;
    }

    static auto load(const std::string &s) -> std::vector<T, A> {
        auto vec = std::vector<T, A>{};
        auto elem_size = sizeof(T);
        auto offset = sizeof(std::size_t);
        auto vec_size = 0uz;
        std::memcpy(&vec_size, s.data(), sizeof(vec_size));
        for (auto i = 0uz; i < vec_size; ++i) {
            auto item_data = s.substr(offset, elem_size);
            auto item = Serializer<T>::load(item_data);
            vec.push_back(std::move(item));
            offset += elem_size;
        }
        return vec;
    }
};


template <typename T, typename A>
struct serex::Serializer<std::vector<T, A>> {
    static auto save(std::vector<T, A> const &obj) -> std::string {
        auto stream = std::string();
        for (const auto &item : obj) {
            auto partial = Serializer<T>::save(item);
            auto partial_size = partial.size();
            stream.append(reinterpret_cast<const char*>(&partial_size), sizeof(std::size_t));
            stream.append(partial);
        }
        return stream;
    }

    static auto load(const std::string &s) -> std::vector<T, A> {
        auto vec = std::vector<T, A>{};
        for (auto i = 0uz; i < s.size(); i += sizeof(T)) {
            std::size_t item_size;
            std::memcpy(&item_size, s.data() + i, sizeof(std::size_t));
            i += sizeof(std::size_t);
            auto item_data = s.substr(i, item_size);
            auto item = Serializer<T>::load(item_data);
            vec.push_back(std::move(item));
            i += item_size;
        }
        return vec;
    }
};
