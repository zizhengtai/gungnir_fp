#include <memory>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List cref", "[List][cref]") {

    using PI = std::unique_ptr<int>;

    using CRI = std::reference_wrapper<const int>;
    using CRPI = std::reference_wrapper<const PI>;

    SECTION("empty List") {
        REQUIRE(List<int>().cref().isEmpty());

        REQUIRE(List<PI>().cref().isEmpty());
    }
    SECTION("List with one element") {
        List<int> xs1(123);
        const auto xs2 = xs1.cref();
        REQUIRE(xs2.size() == 1);
        REQUIRE(xs2.map([](CRI x) { return &x.get(); }) ==
                xs1.map([](const int &x) { return &x; }));

        List<PI> ys1(PI(new int(456)));
        const auto ys2 = ys1.cref();
        REQUIRE(ys2.size() == 1);
        REQUIRE(ys2.map([](CRPI y) { return &y.get(); }) ==
                ys1.map([](const PI &y) { return &y; }));
    }
    SECTION("List with multiple elements") {
        List<int> xs1(1, 2, 3, 4, 5);
        const auto xs2 = xs1.cref();
        REQUIRE(xs2.size() == 5);
        REQUIRE(xs2.map([](CRI x) { return &x.get(); }) ==
                xs1.map([](const int &x) { return &x; }));

        List<PI> ys1(
            PI(new int(5)),
            PI(new int(4)),
            PI(new int(3)),
            PI(new int(2)),
            PI(new int(1))
        );
        const auto ys2 = ys1.cref();
        REQUIRE(ys2.size() == 5);
        REQUIRE(ys2.map([](CRPI y) { return &y.get(); }) ==
                ys1.map([](const PI &y) { return &y; }));
    }
}
