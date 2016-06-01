#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

template<typename A, typename B>
static bool eq(
        const List<std::pair<A, B>>& zipped,
        const List<A>& xs,
        const List<B>& ys)
{
    std::size_t size = std::min(xs.size(), ys.size());
    if (zipped.size() != size) {
        return false;
    }
    while (size-- > 0) {
        if (zipped[size].first != xs[size] ||
            zipped[size].second != ys[size]) {
            return false;
        }
    }
    return true;
}

template<typename A, typename B>
static bool eqRef(
        const List<std::pair<
            std::reference_wrapper<const A>,
            std::reference_wrapper<const B>
        >>& zipped,
        const List<A>& xs,
        const List<B>& ys)
{
    std::size_t size = std::min(xs.size(), ys.size());
    if (zipped.size() != size) {
        return false;
    }
    while (size-- > 0) {
        if (zipped[size].first.get() != xs[size] ||
            zipped[size].second.get() != ys[size]) {
            return false;
        }
    }
    return true;
}

TEST_CASE("test List zip", "[List][zip]") {

    using PI = std::unique_ptr<int>;
    using PS = std::unique_ptr<std::string>;

    SECTION("empty List") {
        List<int> xs1;
        List<std::string> xs2;
        REQUIRE(xs1.zip(xs2).isEmpty());
        REQUIRE(xs2.zip(xs2).isEmpty());

        List<PI> ys1;
        List<PS> ys2;
        REQUIRE(ys1.cref().zip(ys2.cref()).isEmpty());
        REQUIRE(ys2.cref().zip(ys1.cref()).isEmpty());
    }
    SECTION("List with one element") {
        List<int> xs1(123);
        List<std::string> xs2("hello");
        REQUIRE(xs1.zip(List<std::string>()).isEmpty());
        REQUIRE(List<int>().zip(xs2).isEmpty());
        REQUIRE(eq(xs1.zip(xs1), xs1, xs1));
        REQUIRE(eq(xs1.zip(xs2), xs1, xs2));
        REQUIRE(eq(xs2.zip(xs1), xs2, xs1));
        REQUIRE(eq(xs2.zip(xs2), xs2, xs2));

        List<PI> ys1(PI(new int(456)));
        List<PS> ys2(PS(new std::string("world")));
        REQUIRE(ys1.cref().zip(ys2.drop(1000).cref()).isEmpty());
        REQUIRE(ys1.drop(1000).cref().zip(ys2.cref()).isEmpty());
        REQUIRE(eqRef(ys1.cref().zip(ys1.cref()), ys1, ys1));
        REQUIRE(eqRef(ys1.cref().zip(ys2.cref()), ys1, ys2));
        REQUIRE(eqRef(ys2.cref().zip(ys1.cref()), ys2, ys1));
        REQUIRE(eqRef(ys2.cref().zip(ys2.cref()), ys2, ys2));
    }
    SECTION("List with multiple elements") {
        List<int> xs1(1, 2, 3, 4, 5);
        List<std::string> xs2 = xs1.map([](int x) { return std::to_string(x); });
        for (int i: {0, 2, 4, 6}) {
            for (int j: {0, 2, 4, 6}) {
                const auto xs3 = xs1.drop(i);
                const auto xs4 = xs2.drop(j);
                REQUIRE(eq(xs3.zip(xs4), xs3, xs4));
                REQUIRE(eq(xs4.zip(xs3), xs4, xs3));
            }
        }

        List<PI> ys1 = xs1.reverse().map([](int x) { return PI(new int(x)); });
        List<PS> ys2 = xs1.map([](int x) { return PS(new std::string(std::to_string(x))); });
        for (int i: {0, 2, 4, 6}) {
            for (int j: {0, 2, 4, 6}) {
                const auto ys3 = ys1.drop(i);
                const auto ys4 = ys2.drop(j);
                REQUIRE(eqRef(ys3.cref().zip(ys4.cref()), ys3, ys4));
                REQUIRE(eqRef(ys4.cref().zip(ys3.cref()), ys4, ys3));
            }
        }
    }
}
