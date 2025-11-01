# SerEx

SerEx is a serialization library that provides extremely easy-to-use serialization and deserialization functionalities.

- Single string output format
- Polymorphism supported with `import serex.polymorphism`
- STL containers supported with `import serex.vector` etc
- Exclusive to C++ modules (NO MACROS)
- Either define `serialize` or `load+save` methods for custom types
- Use `serex:::save` and `serex::load` for serialization and deserialization

## Examples

### Custom struct and vector

```cpp
import std;
import serex.serialize;
import serex.vector;

struct TestStruct {
    int a;
    float b;
    std::string c;
    std::vector<float> d;

    TestStruct() = default;

    TestStruct(const int a_, const float b_, std::string const &c_, std::vector<float> const &d_ = {})
        : a(a_), b(b_), c(c_), d(d_) {}

    template <typename A>
    auto serialize(A &ar) -> void {
        ar & a & b & c & d;
    }
};

auto main() -> int {
    auto test = TestStruct(42, 3.14f, "Hello Serex", {1.1f, 2.2f, 3.3f});
    const auto serialized = serex::save(test);
    const auto new_test = serex::load<TestStruct>(serialized);
    return 0;
}
```

### Tuple of different types

```cpp
import std;
import serex.serialize;
import serex.vector;
import serex.tuple;

auto main() -> int {
    auto test2 = std::tuple(std::string("hello world"), 123, 4.56f, std::vector{7, 8, 9}, true, std::tuple{5, false});
    const auto serialized2 = serex::save(test2);
    const auto new_test2 = serex::load<decltype(test2)>(serialized2);
}
```

### Nested custom types

```cpp
struct TestStruct3 {
    TestStruct2 t1;
    TestStruct2 t2;

    template <typename A>
    auto serialize(A &ar) -> void {
        ar & t1 & t2;
    }
};


auto main() -> int {
    auto test3 = TestStruct3{TestStruct2{10}, TestStruct2{20}};
    const auto serialized3 = serex::save(test3);td::endl;
    const auto new_test3 = serex::load<TestStruct3>(serialized3);
}
```

### Polymorphism

```cpp
import std;
import serex.serialize;
import serex.polymorphism;

struct TestStruct4 : TestStruct2 {
    int b;

    TestStruct4() = default;
    TestStruct4(const int a_, const int b_) : TestStruct2{a_}, b(b_) {}

    template <typename A>
    auto serialize(A &ar) -> void {
        ar & serex::base_object<TestStruct2>(this);
        ar & b;
    }
};


auto main() -> int {
    auto test4 = TestStruct4{30, 40};
    const auto serialized4 = serex::save(test4);
    const auto new_test4 = serex::load<TestStruct4>(serialized4);
}
```
