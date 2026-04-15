#include <gtest/gtest.h>
import serex;
import std;


struct I {
    int a = 0;
    std::string b;

    auto Serialize(serex::Archive &ar) -> void {
        serex::Push(ar, a, b);
    }
};


struct J : I {
    double c = 0.0;

    auto Serialize(serex::Archive &ar) -> void {
        I::Serialize(ar);
        serex::Push(ar, c);
    }
};


TEST(SerializeInheritStandardTest, InheritanceSerialization) {
    // Create an object of type J
    auto original = J{};
    original.a = 42;
    original.b = "Hello Inheritance";
    original.c = 3.14159;

    const auto serialized = serex::Save(original);
    const auto deserialized = serex::Load<J>(serialized);

    // Verify that the deserialized object matches the original
    EXPECT_EQ(deserialized.a, original.a);
    EXPECT_EQ(deserialized.b, original.b);
    EXPECT_EQ(deserialized.c, original.c);
}
