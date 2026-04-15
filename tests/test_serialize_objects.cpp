#include <gtest/gtest.h>
import serex;
import std;


struct F {
    int x = 0;
    std::string y;

    auto Serialize(serex::Archive &ar) -> void {
        serex::Push(ar, x, y);
    }
};


struct G {
    F f;

    auto Serialize(serex::Archive &ar) -> void {
        serex::Push(ar, f);
    }
};


struct H {
    F f;
    G g;

    auto Serialize(serex::Archive &ar) -> void {
        serex::Push(ar, f, g);
    }
};


TEST(SerializeObjectsTest, NestedSerialization) {
    // Create an object of type H
    auto original = H{};
    original.f.x = 42;
    original.f.y = "Hello";
    original.g.f.x = 84;
    original.g.f.y = "World";

    const auto serialized = serex::Save(original);
    const auto deserialized = serex::Load<H>(serialized);

    // Verify that the deserialized object matches the original
    EXPECT_EQ(deserialized.f.x, original.f.x);
    EXPECT_EQ(deserialized.f.y, original.f.y);
    EXPECT_EQ(deserialized.g.f.x, original.g.f.x);
    EXPECT_EQ(deserialized.g.f.y, original.g.f.y);
}
