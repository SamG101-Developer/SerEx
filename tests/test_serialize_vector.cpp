#include <gtest/gtest.h>
import serex.serialize;
import std;


struct D {
    std::vector<int> values = {};
    std::vector<std::string> strings = {};

    auto serialize(serex::Archive &ar) -> void {
        serex::push_into_archive(ar, values, strings);
    }
};


TEST(SerexVector, SerializeDeserialize) {
    auto original = D{};
    original.values = {1, 2, 3, 4, 5};
    original.strings = {"one", "two", "three", "four", "five"};

    const auto serialized = serex::save(original);
    const auto deserialized = serex::load<D>(serialized);

    EXPECT_EQ(original.values, deserialized.values);
    EXPECT_EQ(original.strings, deserialized.strings);
}
