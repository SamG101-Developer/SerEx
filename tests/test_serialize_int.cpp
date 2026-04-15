#include <gtest/gtest.h>
import serex;
import std;


struct B {
    std::int8_t a = 0;
    std::int16_t b = 0;
    std::int32_t c = 0;
    std::int64_t d = 0;
    std::uint8_t e = 0;
    std::uint16_t f = 0;
    std::uint32_t g = 0;
    std::uint64_t h = 0;
    std::float32_t j = 0.0f;
    std::float64_t k = 0.0;

    auto Serialize(serex::Archive &ar) -> void {
        serex::Push(ar, a, b, c, d, e, f, g, h, j, k);
    }
};


TEST(SerexInt, SerializeDeserialize) {
    auto original = B{};
    original.a = -42;
    original.b = 12345;
    original.c = -67890;
    original.d = 12345678901234;
    original.e = 42;
    original.f = 54321;
    original.g = 67890;
    original.h = 43210987654321;
    original.j = 2.718f;
    original.k = 1.618;

    const auto serialized = serex::Save(original);
    const auto deserialized = serex::Load<B>(serialized);

    EXPECT_EQ(original.a, deserialized.a);
    EXPECT_EQ(original.b, deserialized.b);
    EXPECT_EQ(original.c, deserialized.c);
    EXPECT_EQ(original.d, deserialized.d);
    EXPECT_EQ(original.e, deserialized.e);
    EXPECT_EQ(original.f, deserialized.f);
    EXPECT_EQ(original.g, deserialized.g);
    EXPECT_EQ(original.h, deserialized.h);
    EXPECT_EQ(original.j, deserialized.j);
    EXPECT_EQ(original.k, deserialized.k);
}
