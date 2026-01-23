#include <gtest/gtest.h>
import serex.serialize;
import std;


struct M : serex::SerializablePolymorphicBase {
    int a = 0;

    auto serex_type() -> std::string override {
        return "M";
    }

    auto serialize(serex::Archive &ar) -> void override {
        serex::push_into_archive(ar, a);
    }
};


struct N {
    M *a;
    M *b;

    auto serialize(serex::Archive &ar) -> void {
        serex::push_into_archive(ar, a, b);
    }
};


TEST(SerializePointersTest, PointerSerialization) {
    // Check if the two pointers point to the same data, that they point to the same data after serialization.
    auto shared = M{};
    shared.a = 99;

    auto original = N{};
    original.a = &shared;
    original.b = &shared;

    const auto serialized = serex::save(original);
    const auto deserialized = serex::load<N>(serialized);

    EXPECT_EQ(deserialized.a, deserialized.b);
    EXPECT_EQ(deserialized.a, original.a);
    EXPECT_EQ(deserialized.b, original.b);

    EXPECT_EQ(deserialized.a->a, original.a->a);
    EXPECT_EQ(deserialized.b->a, original.b->a);
}
