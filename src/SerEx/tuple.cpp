export module serex.tuple;
import serex.archive;
import std;


template <typename... Ts>
struct serex::Serializer<std::tuple<Ts...>> {
    template <std::size_t N> requires (N < sizeof...(Ts))
    static auto SaveHelper(std::tuple<Ts...> const &obj) -> std::string {
        using current_t = std::tuple_element_t<N, std::tuple<Ts...>>;
        auto self = Serializer<current_t>::Save(std::get<N>(obj));
        auto self_size = self.size();
        auto str = std::string(reinterpret_cast<const char*>(&self_size), sizeof(std::size_t)).append(self);
        return str.append(SaveHelper<N + 1>(obj));
    }

    template <std::size_t N> requires (N == sizeof...(Ts))
    static auto SaveHelper(std::tuple<Ts...> const &) -> std::string {
        return {};
    }

    static auto Save(std::tuple<Ts...> const &obj) -> std::string {
        return SaveHelper<0>(obj);
    }

    template <std::size_t N> requires (N < sizeof...(Ts))
    static auto LoadHelper(const std::string &s) {
        using current_t = std::tuple_element_t<N, std::tuple<Ts...>>;
        std::size_t item_size;
        std::memcpy(&item_size, s.data(), sizeof(std::size_t));
        auto item_data = s.substr(sizeof(std::size_t), item_size);
        auto current_value = Serializer<current_t>::Load(item_data);
        auto rest_tuple = LoadHelper<N + 1>(s.substr(sizeof(std::size_t) + item_size));
        return std::tuple_cat(std::make_tuple(current_value), rest_tuple);
    }

    template <std::size_t N> requires (N == sizeof...(Ts))
    static auto LoadHelper(const std::string &s) -> std::tuple<> {
        return {};
    }

    static auto Load(std::string const &s) -> std::tuple<Ts...> {
        return LoadHelper<0>(s);
    }
};
