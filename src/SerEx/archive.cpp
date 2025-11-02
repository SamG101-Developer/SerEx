export module serex.archive;
import std;


namespace serex {
    export struct Archive;
    export struct IArchive;
    export struct OArchive;

    struct Dispatcher;

    export template <typename T>
    struct Serializer;

    template <typename T>
    concept has_func_serialize = requires(T &obj, OArchive &ar) {
        { obj.serialize(ar) } -> std::same_as<void>;
    };

    export template <typename T> requires (not std::is_pointer_v<T>)
    auto save(T &obj) -> std::string {
        return Serializer<T>::save(obj);
    }

    export template <typename T> requires (not std::is_pointer_v<T>)
    auto load(const std::string &s) -> T {
        return Serializer<T>::load(s);
    }

    export template <typename T> requires std::is_pointer_v<T>
    auto load(const std::string &s) -> std::unique_ptr<std::remove_pointer_t<T>> {
        return Serializer<std::unique_ptr<std::remove_pointer_t<T>>>::load(s);
    }

    template <typename... Args>
    auto push_into_archive(Archive &ar, Args &&... args) -> Archive&;
}


struct serex::Archive {
    virtual ~Archive() = default;
    Archive() = default;
};


struct serex::OArchive final : Archive {
    std::string serialized_data;

    OArchive() = default;

    auto operator&(auto &obj) -> OArchive& {
        auto partial = Serializer<std::decay_t<decltype(obj)>>().save(obj);
        serialized_data.append(partial).append("\n");
        return *this;
    }
};


struct serex::IArchive final : Archive {
    std::string serialized_data;
    std::size_t pos = 0;

    explicit IArchive(std::string const &data) :
        serialized_data(data) {}

    auto operator&(auto &obj) -> IArchive& {
        auto next_pos = serialized_data.find('\n', pos);
        while (serialized_data[next_pos + 1] == '\n') {
            ++next_pos;
        }
        const auto token = serialized_data.substr(pos, next_pos - pos);
        obj = Serializer<std::decay_t<decltype(obj)>>().load(token);
        pos = next_pos + 1;
        return *this;
    }
};


struct serex::Dispatcher {
    template <typename T>
    static auto dispatch_save(T &obj) -> std::string {
        if constexpr (has_func_serialize<T>) {
            OArchive ar;
            obj.serialize(ar);
            return ar.serialized_data;
        }
        else {
            OArchive ar;
            obj.save(ar);
            return ar.serialized_data;
        }
    }

    template <typename T>
    static auto dispatch_load(const std::string &s) -> T {
        if constexpr (has_func_serialize<T>) {
            IArchive ar{s};
            T obj;
            obj.serialize(ar);
            return obj;
        }
        else {
            IArchive ar{s};
            T obj;
            obj.load(ar);
            return obj;
        }
    }
};


template <typename T>
struct serex::Serializer {
    static auto save(T &obj) -> std::string {
        return Dispatcher::dispatch_save(obj);
    }

    static auto load(const std::string &s) -> T {
        return Dispatcher::dispatch_load<T>(s);
    }
};


template <typename... Args>
auto serex::push_into_archive(Archive &ar, Args &&... args) -> Archive& {
    if (auto o_archive = dynamic_cast<OArchive*>(&ar)) {
        (o_archive->operator&(args), ...);
        return *o_archive;
    }
    if (auto i_archive = dynamic_cast<IArchive*>(&ar)) {
        (i_archive->operator&(args), ...);
        return *i_archive;
    }
    std::unreachable();
}
