#include <algorithm>
#include <memory>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List slice", "[List][slice]") {

    using PI = std::unique_ptr<int>;

    SECTION("empty List") {
        List<int> xs;
        REQUIRE(xs.slice(0, 0).isEmpty());
        REQUIRE(xs.slice(0, 1).isEmpty());
        REQUIRE(xs.slice(1, 1).isEmpty());
        REQUIRE(xs.slice(1, 0).isEmpty());

        List<PI> ys;
        REQUIRE(ys.slice(0, 0).isEmpty());
        REQUIRE(ys.slice(0, 1).isEmpty());
        REQUIRE(ys.slice(1, 0).isEmpty());
        REQUIRE(ys.slice(1, 1).isEmpty());
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.slice(0, 0).isEmpty());
        REQUIRE(xs.slice(0, 1) == List<int>(123));
        REQUIRE(xs.slice(0, 2) == List<int>(123));
        REQUIRE(xs.slice(1, 0).isEmpty());
        REQUIRE(xs.slice(1, 1).isEmpty());
        REQUIRE(xs.slice(1, 2).isEmpty());

        List<PI> ys(PI(new int(456)));
        REQUIRE(ys.slice(0, 0).isEmpty());
        REQUIRE(ys.slice(0, 1) == ys);
        REQUIRE(ys.slice(0, 2) == ys);
        REQUIRE(ys.slice(1, 0).isEmpty());
        REQUIRE(ys.slice(1, 1).isEmpty());
        REQUIRE(ys.slice(1, 2).isEmpty());
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3, 4, 5);
        List<PI> ys(
            PI(new int(5)),
            PI(new int(4)),
            PI(new int(3)),
            PI(new int(2)),
            PI(new int(1))
        );
        for (int i: {0, 2, 4, 6}) {
            for (int j: {std::max(0, i - 2), i, i + 2}) {
                REQUIRE(xs.slice(i, j) == xs.drop(i).take(std::max(0, j - i)));
                REQUIRE(ys.slice(i, j) == ys.drop(i).take(std::max(0, j - i)));
            }
        }
        REQUIRE(xs == List<int>(1, 2, 3, 4, 5));
        REQUIRE(ys.map([](const PI &p) { return *p; }) == List<int>(5, 4, 3, 2, 1));
    }
}
