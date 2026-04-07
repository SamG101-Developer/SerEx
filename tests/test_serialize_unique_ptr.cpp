#include <gtest/gtest.h>
import serex.serialize;
import std;


struct O : serex::SerializablePolymorphicBase {
    int q;

    auto SerexType() -> std::string override {
        return "O";
    }

    auto Serialize(serex::Archive &ar) -> void  override {
        serex::PushToArchive(ar, q);
    }
};


struct P {
    std::unique_ptr<O> a;

    auto Serialize(serex::Archive &ar) -> void {
        serex::PushToArchive(ar, a);
    }
};


TEST(SerializeUniquePtrTest, UniquePtrSerialization) {
    serex::register_polymorphic_type<O>("O");

    // Create an object of type P
    auto original = P{};
    original.a = std::make_unique<O>();
    original.a->q = 123;

    const auto serialized = serex::Save(original);
    const auto deserialized = serex::Load<P>(serialized);

    // Verify that the deserialized object matches the original
    EXPECT_EQ(deserialized.a->q, original.a->q);
}
