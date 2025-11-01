export module serex.polymorphic;
import std;


export namespace serex {
    template <typename Base, typename Derived> requires std::derived_from<Derived, Base>
    auto base_object(Derived *d) -> Base& {
        return *d;
    }
}
