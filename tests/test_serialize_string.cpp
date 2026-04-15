#include <gtest/gtest.h>
import serex;
import std;


struct C {
    std::string a;
    std::wstring b;
    std::u8string c;
    std::u16string d;
    std::u32string e;

    auto Serialize(serex::Archive &ar) -> void {
        serex::Push(ar, a, b, c, d, e);
    }
};


TEST(SerexString, SerializeDeserialize) {
    auto original = C{};
    original.a = "Hello, World!";
    original.b = L"Wide String Example";
    original.c = u8"UTF-8 String Example";
    original.d = u"UTF-16 String Example";
    original.e = U"UTF-32 String Example";

    const auto serialized = serex::Save(original);
    const auto deserialized = serex::Load<C>(serialized);

    EXPECT_EQ(original.a, deserialized.a);
    EXPECT_EQ(original.b, deserialized.b);
    EXPECT_EQ(original.c, deserialized.c);
    EXPECT_EQ(original.d, deserialized.d);
    EXPECT_EQ(original.e, deserialized.e);
}
