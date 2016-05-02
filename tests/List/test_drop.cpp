#include <algorithm>
#include <memory>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List drop", "[List][drop]") {
    SECTION("empty List") {
        List<int> xs;
        REQUIRE(xs.drop(0).isEmpty());
        REQUIRE(xs.drop(1).isEmpty());
        REQUIRE(xs.drop(2).isEmpty());
        REQUIRE(xs.drop(10000).isEmpty());

        List<std::unique_ptr<int>> ys;
        REQUIRE(ys.drop(0).isEmpty());
        REQUIRE(ys.drop(1).isEmpty());
        REQUIRE(ys.drop(2).isEmpty());
        REQUIRE(ys.drop(10000).isEmpty());
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.drop(0) == List<int>(123));
        REQUIRE(xs.drop(1).isEmpty());
        REQUIRE(xs.drop(2).isEmpty());
        REQUIRE(xs.drop(3).isEmpty());
        REQUIRE(xs.drop(10000).isEmpty());

        List<std::unique_ptr<int>> ys(std::unique_ptr<int>(new int(456)));
        REQUIRE(ys.drop(0) == ys);
        REQUIRE(ys.drop(1).isEmpty());
        REQUIRE(ys.drop(2).isEmpty());
        REQUIRE(ys.drop(3).isEmpty());
        REQUIRE(ys.drop(10000).isEmpty());
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3, 4);
        REQUIRE(xs.drop(0) == List<int>(1, 2, 3, 4));
        REQUIRE(xs.drop(1) == List<int>(2, 3, 4));
        REQUIRE(xs.drop(2) == List<int>(3, 4));
        REQUIRE(xs.drop(3) == List<int>(4));
        REQUIRE(xs.drop(4) == List<int>());
        REQUIRE(xs.drop(5) == List<int>());
        REQUIRE(xs.drop(10000) == List<int>());

        List<std::unique_ptr<int>> ys(
            std::unique_ptr<int>(new int(4)),
            std::unique_ptr<int>(new int(3)),
            std::unique_ptr<int>(new int(2)),
            std::unique_ptr<int>(new int(1))
        );
        for (std::size_t i: {0, 1, 2, 3, 4, 5, 10000}) {
            const auto zs = ys.drop(i);
            const auto n = std::min(ys.size(), i);
            REQUIRE(zs.size() == ys.size() - n);
            for (std::size_t j = 0; j < ys.size() - n; ++j) {
                REQUIRE(zs[j] == ys[n + j]);
            }
        }
    }
    SECTION("chained drops") {
        List<int> xs(1, 2, 3, 4, 5);
        REQUIRE(xs.drop(10000).drop(10000).drop(3) == List<int>());
        REQUIRE(xs.drop(2).drop(1) == List<int>(4, 5));
        REQUIRE(xs.drop(2).drop(3) == List<int>());
        REQUIRE(xs.drop(4).drop(3) == List<int>());
        REQUIRE(xs.drop(1).drop(2).drop(1) == List<int>(5));
        REQUIRE(xs.drop(0).drop(1).drop(0) == List<int>(2, 3, 4, 5));
    }
}
