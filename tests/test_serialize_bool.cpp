#include <gtest/gtest.h>
import serex.serialize;


struct A {
    bool a = false;
    bool b = false;
    bool c = false;

    auto serialize(serex::Archive &ar) -> void {
        serex::push_into_archive(ar, a, b, c);
    }
};


TEST(SerexBool, SerializeDeserialize) {
    auto original = A{};
    original.a = true;
    original.b = false;
    original.c = true;

    const auto serialized = serex::save(original);
    const auto deserialized = serex::load<A>(serialized);

    EXPECT_EQ(original.a, deserialized.a);
    EXPECT_EQ(original.b, deserialized.b);
    EXPECT_EQ(original.c, deserialized.c);
}
