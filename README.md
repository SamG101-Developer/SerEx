# SerEx

SerEx is a serialization library that provides extremely easy-to-use serialization and deserialization functionalities.

- Single string output format
- Polymorphism supported
- STL containers supported
- Exclusive to C++ modules (NO MACROS)
- Either define `serialize` or `load+save` methods for custom types
- Use `serex:::save` and `serex::load` for serialization and deserialization

## Examples

### Custom struct and vector

```cpp
import std;
import serex.serialize;

struct TestStruct {
    int a;
    float b;
    std::string c;
    std::vector<float> d;

    TestStruct() = default;

    TestStruct(const int a_, const float b_, std::string const &c_, std::vector<float> const &d_ = {})
        : a(a_), b(b_), c(c_), d(d_) {}

    auto Serialize(serex::Archive &ar) -> void {
        serex::PushIntoArchive(ar, a, b, c, d);
    }
};

auto main() -> int {
    auto test = TestStruct(42, 3.14f, "Hello Serex", {1.1f, 2.2f, 3.3f});
    const auto serialized = serex::Save(test);
    const auto new_test = serex::Load<TestStruct>(serialized);
    return 0;
}
```

### Tuple of different types

```cpp
import std;
import serex.serialize;

auto main() -> int {
    auto test2 = std::tuple(std::string("hello world"), 123, 4.56f, std::vector{7, 8, 9}, true, std::tuple{5, false});
    const auto serialized2 = serex::Save(test2);
    const auto new_test2 = serex::Load<decltype(test2)>(serialized2);
}
```

### Nested custom types

```cpp
struct TestStruct3 {
    TestStruct2 t1;
    TestStruct2 t2;

    auto Serialize(serex::Archive &ar) -> void {
        serex::PushIntoArchive(ar, t1, t2);
    }
};


auto main() -> int {
    auto test3 = TestStruct3{TestStruct2{10}, TestStruct2{20}};
    const auto serialized3 = serex::Save(test3);
    const auto new_test3 = serex::Load<TestStruct3>(serialized3);
}
```

### Basic Polymorphism

```cpp
import std;
import serex.serialize;

struct TestStruct4 : TestStruct2 {
    int b;

    TestStruct4() = default;
    TestStruct4(const int a_, const int b_) : TestStruct2{a_}, b(b_) {}

    auto Serialize(serex::Archive &ar) -> void {
        TestStruct2::Serialize(ar);
        serex::PushIntoArchive(ar, b);
    }
};


auto main() -> int {
    auto test4 = TestStruct4{30, 40};
    const auto serialized4 = serex::Save(test4);
    const auto new_test4 = serex::Load<TestStruct4>(serialized4);
}
```

### Advanced Polymorphism with Base Class Pointer

```cpp
import std;
import serex.serialize;

struct Base : serex::SerializablePolymorphicBase {
    int a;

    auto SerexType() -> std::string override {
        return "Base";
    }
    
    auto Serialize(serex::Archive &ar) override -> void {
        serex::PushIntoArchive(ar, a);
    }

    Base() = default;
    explicit Base(const int a) : a{a} {}
};


struct Derived final : Base {
    int b;

    auto SerexType() -> std::string override {
        return "Derived";
    }
    
    auto Serialize(serex::Archive &ar) override -> void {
        Base::Serialize(ar);
        serex::PushIntoArchive(ar, b);
    }

    Derived() = default;
    explicit Derived(const int a, const int b) : Base{a}, b{b} {}
};


auto test() {
    // Call once in a startup function (before any serialization)
    // These type names must match those returned by serex_type()
    serex::register_polymorphic_type<Base>("Base");
    serex::register_polymorphic_type<Derived>("Derived");

    auto d = Derived{0, 1};
    auto serialized = serex::Save(d);
    // send over socket for example, received unknown type as string
    // --->
    
    // <---
    // load as known common base type and cast to derived type
    auto dd = serex::Load<Base*>(serialized);
    auto ee = serex::poly_non_owning_cast<Derived>(dd);         // Derived*
    auto ff = serex::poly_owning_cast<Derived>(std::move(dd));  // std::unique_ptr<Derived> (moved from dd)
}
```

### Different load and save methods

```cpp
struct TestStruct5 {
    int a;

    auto Save() const -> std::string {
        return std::to_string(a);
    }

    static auto Load(std::string const &s) -> TestStruct5 {
        return TestStruct5{std::stoi(s)};
    }
};
```

```cpp
template <>
struct serex::Serializer<bool> {
    static auto Save(bool const &obj) -> std::string {
        return obj ? "1" : "0";
    }

    static auto Load(std::string const &s) -> bool {
        return s == "1";
    }
};
```
