export module serex.tuple;
import std;
import serex.archive;


template <typename... Ts>
struct serex::Serializer<std::tuple<Ts...>> {
    template <std::size_t N> requires (N < sizeof...(Ts))
    static auto save_helper(std::tuple<Ts...> const& obj) -> std::string {
        using current_t = std::tuple_element_t<N, std::tuple<Ts...>>;
        auto self = Serializer<current_t>::save(std::get<N>(obj));
        auto self_size = self.size();
        auto str = std::string(reinterpret_cast<const char*>(&self_size), sizeof(std::size_t)).append(self);
        return str.append(save_helper<N + 1>(obj));
    }

    template <std::size_t N> requires (N == sizeof...(Ts))
    static auto save_helper(std::tuple<Ts...> const& obj) -> std::string {
        return {};
    }

    static auto save(std::tuple<Ts...> const& obj) -> std::string {
        return save_helper<0>(obj);
    }

    template <std::size_t N> requires (N < sizeof...(Ts))
    static auto load_helper(const std::string& s) -> auto {
        using current_t = std::tuple_element_t<N, std::tuple<Ts...>>;
        std::size_t item_size;
        std::memcpy(&item_size, s.data(), sizeof(std::size_t));
        auto item_data = s.substr(sizeof(std::size_t), item_size);
        auto current_value = Serializer<current_t>::load(item_data);
        auto rest_tuple = load_helper<N + 1>(s.substr(sizeof(std::size_t) + item_size));
        return std::tuple_cat(std::make_tuple(current_value), rest_tuple);
    }

    template <std::size_t N> requires (N == sizeof...(Ts))
    static auto load_helper(const std::string& s) -> std::tuple<> {
        return {};
    }

    static auto load(const std::string& s) -> std::tuple<Ts...> {
        return load_helper<0>(s);
    }
};
