export module serex.archive;
import serex.encoding;
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
        { obj.Serialize(ar) } -> std::same_as<void>;
    };

    export template <typename T> requires (not std::is_pointer_v<T>)
    auto Save(T &obj) -> std::string {
        return Serializer<std::remove_cvref_t<T>>::Save(obj);
    }

    export template <typename T> requires (not std::is_pointer_v<T>)
    auto Load(const std::string &s) -> T {
        return Serializer<std::remove_cvref_t<T>>::Load(s);
    }

    export template <typename T> requires std::is_pointer_v<T>
    auto Load(const std::string &s) -> std::unique_ptr<std::remove_pointer_t<T>> {
        return Serializer<std::unique_ptr<std::remove_pointer_t<T>>>::Load(s);
    }

    export template <typename T>
    auto Load(std::string &&s) -> auto {
        return serex::Load<T>(s);
    }

    export template <typename... Args>
    auto PushToArchive(Archive &ar, Args &... args) -> void;
}


struct serex::Archive {
    virtual ~Archive() = default;
    Archive() = default;
};


struct serex::OArchive final : Archive {
    std::string serialized_data;

    OArchive() = default;

    auto operator&(auto &obj) -> OArchive& {
        auto partial = Serializer<std::decay_t<decltype(obj)>>::Save(obj);
        auto partial_size = partial.size();
        serialized_data.append(reinterpret_cast<const char*>(&partial_size), sizeof(std::size_t));
        serialized_data.append(partial);
        return *this;
    }
};


struct serex::IArchive final : Archive {
    std::string serialized_data;
    std::size_t pos = 0;

    explicit IArchive(std::string data) :
        serialized_data(std::move(data)) {}

    auto operator&(auto &obj) -> IArchive& {
        auto partial_size = 0uz;
        std::memcpy(&partial_size, serialized_data.data() + pos, sizeof(std::size_t));
        pos += sizeof(std::size_t);

        auto partial = serialized_data.substr(pos, partial_size);
        obj = Serializer<std::remove_cvref_t<decltype(obj)>>::Load(partial);
        pos += partial_size;
        return *this;
    }
};


struct serex::Dispatcher {
    template <typename T>
    static auto DispatchSave(T &obj) -> std::string {
        if constexpr (has_func_serialize<T>) {
            OArchive ar;
            obj.Serialize(ar);
            return ar.serialized_data;
        }
        else {
            OArchive ar;
            obj.Save(ar);
            return ar.serialized_data;
        }
    }

    template <typename T>
    static auto DispatchLoad(const std::string &s) -> T {
        if constexpr (has_func_serialize<T>) {
            IArchive ar{s};
            T obj;
            obj.Serialize(ar);
            return obj;
        }
        else {
            IArchive ar{s};
            T obj;
            obj.Load(ar);
            return obj;
        }
    }
};


template <typename T>
struct serex::Serializer {
    static auto Save(T &obj) -> std::string {
        return Dispatcher::DispatchSave(obj);
    }

    static auto Load(const std::string &s) -> T {
        return Dispatcher::DispatchLoad<T>(s);
    }
};


template <typename... Args>
auto serex::PushToArchive(Archive &ar, Args &... args) -> void {
    if (auto o_archive = dynamic_cast<OArchive*>(&ar)) {
        (o_archive->operator&(args), ...);
        return;
    }
    if (auto i_archive = dynamic_cast<IArchive*>(&ar)) {
        (i_archive->operator&(args), ...);
        return;
    }
    std::unreachable();
}
