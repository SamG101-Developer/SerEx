#include <gtest/gtest.h>
import serex;

enum class E {
    A = 1,
    B = 2,
    C = 3,
};

struct A {
    E a;
    E b;
    E c;

    auto Serialize(serex::Archive &ar) -> void {
        serex::Push(ar, a, b, c);
    }
};


TEST(SerexEnum, SerializeDeserialize) {
    const auto a = A{E::A, E::B, E::C};
    const auto serialized = serex::Save(a);
    const auto deserialized = serex::Load<A>(serialized);

    ASSERT_EQ(a.a, deserialized.a);
    ASSERT_EQ(a.b, deserialized.b);
    ASSERT_EQ(a.c, deserialized.c);

    ASSERT_EQ(deserialized.a, E::A);
    ASSERT_EQ(deserialized.b, E::B);
    ASSERT_EQ(deserialized.c, E::C);
}
