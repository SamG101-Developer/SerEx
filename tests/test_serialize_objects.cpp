#include <gtest/gtest.h>
import serex.serialize;
import std;


struct F {
    int x;
    std::string y;

    auto serialize(serex::Archive &ar) -> void {
        serex::push_into_archive(ar, x, y);
    }
};


struct G {
    F f;

    auto serialize(serex::Archive &ar) -> void {
        serex::push_into_archive(ar, f);
    }
};


struct H {
    F f;
    G g;

    auto serialize(serex::Archive &ar) -> void {
        serex::push_into_archive(ar, f, g);
    }
};


TEST(SerializeObjectsTest, NestedSerialization) {
    // Create an object of type H
    auto original = H{};
    original.f.x = 42;
    original.f.y = "Hello";
    original.g.f.x = 84;
    original.g.f.y = "World";

    const auto serialized = serex::save(original);
    const auto deserialized = serex::load<H>(serialized);

    // Verify that the deserialized object matches the original
    EXPECT_EQ(deserialized.f.x, original.f.x);
    EXPECT_EQ(deserialized.f.y, original.f.y);
    EXPECT_EQ(deserialized.g.f.x, original.g.f.x);
    EXPECT_EQ(deserialized.g.f.y, original.g.f.y);
}
