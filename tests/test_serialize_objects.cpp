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


struct I {
    int a;
    int b;

    auto Save(serex::OArchive &ar) const -> void {
        serex::Push(ar, a, b);
    }

    auto Load(serex::IArchive &ar) -> void {
        serex::Push(ar, a, b);
        a += 1;
        b += 1;
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

    // Const version.
    const auto original_const = original;
    const auto serialized_const = serex::Save(original_const);
    const auto deserialized_const = serex::Load<H>(serialized_const);
    EXPECT_EQ(deserialized_const.f.x, original_const.f.x);
    EXPECT_EQ(deserialized_const.f.y, original_const.f.y);
    EXPECT_EQ(deserialized_const.g.f.x, original_const.g.f.x);
    EXPECT_EQ(deserialized_const.g.f.y, original_const.g.f.y);

    // Individual save and load functions.
    auto i = I{1, 2};
    const auto serialized_i = serex::Save(i);
    const auto deserialized_i = serex::Load<I>(serialized_i);
    EXPECT_EQ(deserialized_i.a, i.a + 1);
    EXPECT_EQ(deserialized_i.b, i.b + 1);
}
