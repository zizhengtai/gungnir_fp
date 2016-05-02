#include <algorithm>
#include <memory>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List dropRight", "[List][dropRight]") {
    SECTION("empty List") {
        List<int> xs;
        REQUIRE(xs.dropRight(0).isEmpty());
        REQUIRE(xs.dropRight(1).isEmpty());
        REQUIRE(xs.dropRight(2).isEmpty());
        REQUIRE(xs.dropRight(10000).isEmpty());

        List<std::unique_ptr<int>> ys;
        REQUIRE(ys.dropRight(0).isEmpty());
        REQUIRE(ys.dropRight(1).isEmpty());
        REQUIRE(ys.dropRight(2).isEmpty());
        REQUIRE(ys.dropRight(10000).isEmpty());
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.dropRight(0) == List<int>(123));
        REQUIRE(xs.dropRight(1).isEmpty());
        REQUIRE(xs.dropRight(2).isEmpty());
        REQUIRE(xs.dropRight(3).isEmpty());
        REQUIRE(xs.dropRight(10000).isEmpty());

        List<std::unique_ptr<int>> ys(std::unique_ptr<int>(new int(456)));
        REQUIRE(ys.dropRight(0) == ys);
        REQUIRE(ys.dropRight(1).isEmpty());
        REQUIRE(ys.dropRight(2).isEmpty());
        REQUIRE(ys.dropRight(3).isEmpty());
        REQUIRE(ys.dropRight(10000).isEmpty());
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3, 4);
        REQUIRE(xs.dropRight(0) == List<int>(1, 2, 3, 4));
        REQUIRE(xs.dropRight(1) == List<int>(1, 2, 3));
        REQUIRE(xs.dropRight(2) == List<int>(1, 2));
        REQUIRE(xs.dropRight(3) == List<int>(1));
        REQUIRE(xs.dropRight(4) == List<int>());
        REQUIRE(xs.dropRight(5) == List<int>());
        REQUIRE(xs.dropRight(10000) == List<int>());

        List<std::unique_ptr<int>> ys(
            std::unique_ptr<int>(new int(4)),
            std::unique_ptr<int>(new int(3)),
            std::unique_ptr<int>(new int(2)),
            std::unique_ptr<int>(new int(1))
        );
        for (std::size_t i: {0, 1, 2, 3, 4, 5, 10000}) {
            const auto zs = ys.dropRight(i);
            const auto n = std::min(ys.size(), i);
            REQUIRE(zs.size() == ys.size() - n);
            for (std::size_t j = 0; j < ys.size() - n; ++j) {
                REQUIRE(zs[j] == ys[j]);
            }
        }
    }
    SECTION("chained dropRights") {
        List<int> xs(1, 2, 3, 4, 5);
        REQUIRE(xs.dropRight(10000).dropRight(10000).dropRight(3) == List<int>());
        REQUIRE(xs.dropRight(2).dropRight(1) == List<int>(1, 2));
        REQUIRE(xs.dropRight(2).dropRight(3) == List<int>());
        REQUIRE(xs.dropRight(4).dropRight(3) == List<int>());
        REQUIRE(xs.dropRight(1).dropRight(2).dropRight(1) == List<int>(1));
        REQUIRE(xs.dropRight(0).dropRight(1).dropRight(0) == List<int>(1, 2, 3, 4));
    }
}
