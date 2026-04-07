#include <gtest/gtest.h>
import serex.serialize;
import std;


struct K : serex::SerializablePolymorphicBase {
    int m = 0;

    auto SerexType() -> std::string override {
        return "K";
    }

    auto Serialize(serex::Archive &ar) -> void override {
        serex::PushToArchive(ar, m);
    }
};


struct L : K {
    std::string n;

    auto SerexType() -> std::string override {
        return "L";
    }

    auto Serialize(serex::Archive &ar) -> void override {
        K::Serialize(ar);
        serex::PushToArchive(ar, n);
    }
};


TEST(SerializeInheritPolymorphicTest, PolymorphicSerialization) {
    // Registration
    serex::register_polymorphic_type<K>("K");
    serex::register_polymorphic_type<L>("L");

    // Create an object of type L
    const auto original = std::make_unique<L>();
    original->m = 100;
    original->n = "Polymorphic Inheritance";

    const auto serialized = serex::Save(original);
    auto deserialized = serex::Load<K*>(serialized);

    // Cast to an owning pointer of type L
    const auto deserialized_l = serex::poly_owning_cast<L>(std::move(deserialized));
    EXPECT_EQ(original->m, deserialized_l->m);
    EXPECT_EQ(original->n, deserialized_l->n);
}
