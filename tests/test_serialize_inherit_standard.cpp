#include <gtest/gtest.h>
import serex.serialize;
import std;


struct I {
    int a;
    std::string b;

    auto serialize(serex::Archive &ar) -> void {
        serex::push_into_archive(ar, a, b);
    }
};


struct J : I {
    double c;

    auto serialize(serex::Archive &ar) -> void {
        I::serialize(ar);
        serex::push_into_archive(ar, c);
    }
};


TEST(SerializeInheritStandardTest, InheritanceSerialization) {
    // Create an object of type J
    auto original = J{};
    original.a = 42;
    original.b = "Hello Inheritance";
    original.c = 3.14159;

    const auto serialized = serex::save(original);
    const auto deserialized = serex::load<J>(serialized);

    // Verify that the deserialized object matches the original
    EXPECT_EQ(deserialized.a, original.a);
    EXPECT_EQ(deserialized.b, original.b);
    EXPECT_EQ(deserialized.c, original.c);
}
