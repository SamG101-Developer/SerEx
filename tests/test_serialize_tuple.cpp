#include <gtest/gtest.h>
import serex.serialize;
import std;


struct E {
    std::tuple<int, std::string, float, bool> data = {};
    std::tuple<std::tuple<int, int>, std::tuple<std::string, std::string>> nested_data = {};
    std::tuple<std::vector<int>, std::vector<std::string>> vector_data = {};

    auto serialize(serex::Archive &ar) -> void {
        serex::push_into_archive(ar, data, nested_data, vector_data);
    }
};


TEST(SerexTuple, SerializeDeserialize) {
    auto original = E{};
    original.data = std::make_tuple(42, "Hello, Tuple!", 3.14f, true);
    original.nested_data = std::make_tuple(std::make_tuple(1, 2), std::make_tuple("foo", "bar"));
    original.vector_data = std::make_tuple(std::vector<int>{10, 20, 30}, std::vector<std::string>{"alpha", "beta", "gamma"});

    const auto serialized = serex::save(original);
    const auto deserialized = serex::load<E>(serialized);

    EXPECT_EQ(original.data, deserialized.data);
    EXPECT_EQ(original.nested_data, deserialized.nested_data);
    EXPECT_EQ(original.vector_data, deserialized.vector_data);
}
