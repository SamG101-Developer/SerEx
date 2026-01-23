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



struct Ast : serex::SerializablePolymorphicBase {
    auto serex_type() -> std::string override { return "Ast"; }
    auto serialize(serex::Archive &ar) -> void override { }
};

struct ModuleAst : Ast {
    std::vector<std::unique_ptr<Ast>> members;
    auto serex_type() -> std::string override { return "ModuleAst"; }
    auto serialize(serex::Archive &ar) -> void override {
        Ast::serialize(ar);
        serex::push_into_archive(ar, members);
    }
};

struct AnnotationAst : Ast {
    std::string name;
    Ast *ctx = nullptr;
    auto serex_type() -> std::string override { return "AnnotationAst"; }
    auto serialize(serex::Archive &ar) -> void override {
        Ast::serialize(ar);
        serex::push_into_archive(ar, name, ctx);
    }
};

struct FunctionAst : Ast {
    std::string name;
    std::vector<std::unique_ptr<AnnotationAst>> annotations;
    std::vector<std::unique_ptr<Ast>> body;
    auto serex_type() -> std::string override { return "FunctionAst"; }
    auto serialize(serex::Archive &ar) -> void  override {
        Ast::serialize(ar);
        serex::push_into_archive(ar, name, annotations, body);
    }
};

TEST(SerexComplex, Asts) {
    // Registration
    serex::register_polymorphic_type<Ast>("Ast");
    serex::register_polymorphic_type<ModuleAst>("ModuleAst");
    serex::register_polymorphic_type<AnnotationAst>("AnnotationAst");
    serex::register_polymorphic_type<FunctionAst>("FunctionAst");

    // Create an object of type ModuleAst
    auto original = std::make_unique<ModuleAst>();
    auto func = std::make_unique<FunctionAst>();
    func->name = "my_function";
    auto annotation = std::make_unique<AnnotationAst>();
    annotation->name = "test_annotation";
    annotation->ctx = func.get();
    func->annotations.push_back(std::move(annotation));
    original->members.push_back(std::move(func));

    // Serialize and simulate a fresh deserialization environment
    const auto serialized = serex::save_poly(std::move(original));
    serex::pointer_map.clear();
    serex::SerializablePolymorphicBase::next_id = 1;
    auto deserialized = serex::load_poly<Ast>(serialized);

    // Cast to an owning pointer of type ModuleAst
    const auto deserialized_module = serex::poly_owning_cast<ModuleAst>(std::move(deserialized));
    EXPECT_EQ(deserialized_module->members.size(), 1);

    const auto deserialized_function = serex::poly_owning_cast<FunctionAst>(std::move(deserialized_module->members[0]));
    EXPECT_EQ(deserialized_function->name, "my_function");
    EXPECT_EQ(deserialized_function->annotations.size(), 1);
    EXPECT_EQ(deserialized_function->annotations[0]->name, "test_annotation");

    const auto deserialized_annotation = deserialized_function->annotations[0].get();
    EXPECT_EQ(deserialized_annotation->ctx, deserialized_function.get());
}
