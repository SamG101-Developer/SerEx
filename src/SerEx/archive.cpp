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
    concept has_func_serialize = requires(T &obj, Archive &ar)
    {
        { obj.Serialize(ar) } -> std::same_as<void>;
    };

    template <typename T>
    concept has_func_save = requires(T const &obj, Archive &ar)
    {
        { obj.Save(ar) } -> std::same_as<void>;
    };

    template <typename T>
    concept has_func_load = requires(T &obj, Archive &ar)
    {
        { obj.Load(ar) } -> std::same_as<void>;
    };

    export template <typename T> requires (not std::is_pointer_v<T>)
    auto Save(T const &obj) -> std::string {
        return Serializer<std::remove_cvref_t<T>>::Save(obj);
    }

    export template <typename T> requires (not std::is_pointer_v<T>)
    auto Load(std::string const &s) -> T {
        return Serializer<std::remove_cvref_t<T>>::Load(s);
    }

    export template <typename T> requires std::is_pointer_v<T>
    auto Load(std::string const &s) -> std::unique_ptr<std::remove_pointer_t<T>> {
        return Serializer<std::unique_ptr<std::remove_pointer_t<T>>>::Load(s);
    }

    export template <typename T>
    auto Load(std::string &&s) -> auto {
        return serex::Load<T>(std::move(s));
    }

    export template <typename... Args>
    auto Push(Archive &ar, Args &... args) -> void;
    export template <typename... Args>
    auto Push(OArchive &ar, Args const &... args) -> void;
    export template <typename... Args>
    auto Push(IArchive &ar, Args &... args) -> void;
}

struct serex::Archive {
    virtual ~Archive() = default;
    Archive() = default;
};

struct serex::OArchive final : Archive {
    std::string SerializedData;

    OArchive() = default;

    auto operator&(auto const &obj) -> OArchive& {
        auto partial = Serializer<std::decay_t<decltype(obj)>>::Save(obj);
        auto partial_size = partial.size();
        SerializedData.append(reinterpret_cast<const char*>(&partial_size), sizeof(std::size_t));
        SerializedData.append(partial);
        return *this;
    }
};

struct serex::IArchive final : Archive {
    std::string SerializedData;
    std::size_t Pos = 0;

    explicit IArchive(std::string data) :
        SerializedData(std::move(data)) {}

    auto operator&(auto &obj) -> IArchive& {
        auto partial_size = 0uz;
        std::memcpy(&partial_size, SerializedData.data() + Pos, sizeof(std::size_t));
        Pos += sizeof(std::size_t);

        auto partial = SerializedData.substr(Pos, partial_size);
        obj = Serializer<std::remove_cvref_t<decltype(obj)>>::Load(partial);
        Pos += partial_size;
        return *this;
    }
};

struct serex::Dispatcher {
    template <typename T>
    static auto DispatchSave(T const &obj) -> std::string {
        if constexpr (has_func_save<T>) {
            OArchive ar;
            obj.Save(ar);
            return ar.SerializedData;
        }
        else if constexpr (has_func_serialize<T>) {
            OArchive ar;
            const_cast<T&>(obj).Serialize(ar);
            return ar.SerializedData;
        }
        else {
            static_assert(false, "Type must have either Save or Serialize function for serialization");
        }
    }

    template <typename T>
    static auto DispatchLoad(const std::string &s) -> T {
        if constexpr (has_func_load<T>) {
            IArchive ar(s);
            T obj;
            obj.Load(ar);
            return obj;
        }
        else if constexpr (has_func_serialize<T>) {
            IArchive ar(s);
            T obj;
            obj.Serialize(ar);
            return obj;
        }
        else {
            static_assert(false, "Type must have either Load or Serialize function for deserialization");
        }
    }
};

template <typename T>
struct serex::Serializer {
    static auto Save(T const &obj) -> std::string {
        return Dispatcher::DispatchSave(obj);
    }

    static auto Load(const std::string &s) -> T {
        return Dispatcher::DispatchLoad<T>(s);
    }
};

template <typename... Args>
auto serex::Push(Archive &ar, Args &... args) -> void {
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

template <typename... Args>
auto serex::Push(OArchive &ar, Args const &... args) -> void {
    (ar.operator&(args), ...);
}

template <typename... Args>
auto serex::Push(IArchive &ar, Args &... args) -> void {
    (ar.operator&(args), ...);
}
