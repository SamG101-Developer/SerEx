#include <gtest/gtest.h>
import serex.serialize;
import std;


struct O : serex::SerializablePolymorphicBase {
    int q;

    auto serex_type() -> std::string override {
        return "O";
    }

    auto serialize(serex::Archive &ar) -> void  override {
        serex::push_into_archive(ar, q);
    }
};


struct P {
    std::unique_ptr<O> a;
    O* b;

    auto serialize(serex::Archive &ar) -> void {
        serex::push_into_archive(ar, a, b);
    }
};


TEST(SerializeUniquePtrTest, UniquePtrSerialization) {
    serex::register_polymorphic_type<O>("O");

    // Create an object of type P
    auto original = P{};
    original.a = std::make_unique<O>();
    original.a->q = 123;
    original.b = original.a.get();

    const auto serialized = serex::save(original);
    const auto deserialized = serex::load<P>(serialized);

    // Verify that the deserialized object matches the original
    EXPECT_EQ(deserialized.a->q, original.a->q);
    EXPECT_EQ(deserialized.b->q, original.b->q);
    EXPECT_EQ(deserialized.b, deserialized.a.get());
    EXPECT_EQ(deserialized.b, original.b);
    EXPECT_EQ(deserialized.b, original.a.get());
}
