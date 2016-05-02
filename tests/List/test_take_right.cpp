#include <algorithm>
#include <memory>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List takeRight", "[List][takeRight]") {
    SECTION("empty List") {
        List<int> xs;
        REQUIRE(xs.takeRight(0).isEmpty());
        REQUIRE(xs.takeRight(1).isEmpty());
        REQUIRE(xs.takeRight(2).isEmpty());
        REQUIRE(xs.takeRight(10000).isEmpty());

        List<std::unique_ptr<int>> ys;
        REQUIRE(ys.takeRight(0).isEmpty());
        REQUIRE(ys.takeRight(1).isEmpty());
        REQUIRE(ys.takeRight(2).isEmpty());
        REQUIRE(ys.takeRight(10000).isEmpty());
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.takeRight(0).isEmpty());
        REQUIRE(xs.takeRight(1) == xs);
        REQUIRE(xs.takeRight(2) == xs);
        REQUIRE(xs.takeRight(3) == xs);
        REQUIRE(xs.takeRight(10000) == xs);

        List<std::unique_ptr<int>> ys(std::unique_ptr<int>(new int(456)));
        REQUIRE(ys.takeRight(0).isEmpty());
        REQUIRE(ys.takeRight(1) == ys);
        REQUIRE(ys.takeRight(2) == ys);
        REQUIRE(ys.takeRight(3) == ys);
        REQUIRE(ys.takeRight(10000) == ys);
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3, 4);
        REQUIRE(xs.takeRight(0) == List<int>());
        REQUIRE(xs.takeRight(1) == List<int>(4));
        REQUIRE(xs.takeRight(2) == List<int>(3, 4));
        REQUIRE(xs.takeRight(3) == List<int>(2, 3, 4));
        REQUIRE(xs.takeRight(4) == List<int>(1, 2, 3, 4));
        REQUIRE(xs.takeRight(5) == List<int>(1, 2, 3, 4));
        REQUIRE(xs.takeRight(10000) == List<int>(1, 2, 3, 4));

        List<std::unique_ptr<int>> ys(
            std::unique_ptr<int>(new int(4)),
            std::unique_ptr<int>(new int(3)),
            std::unique_ptr<int>(new int(2)),
            std::unique_ptr<int>(new int(1))
        );
        for (std::size_t i: {0, 1, 2, 3, 4, 5, 10000}) {
            const auto zs = ys.takeRight(i);
            const auto n = std::min(ys.size(), i);
            REQUIRE(zs.size() == n);
            for (std::size_t j = 0; j < n; ++j) {
                REQUIRE(zs[j] == ys[ys.size() - n + j]);
            }
        }
    }
    SECTION("chained takeRights") {
        List<int> xs(1, 2, 3, 4, 5);
        REQUIRE(xs.takeRight(10000).takeRight(10000).takeRight(3) == List<int>(3, 4, 5));
        REQUIRE(xs.takeRight(4).takeRight(5) == List<int>(2, 3, 4, 5));
        REQUIRE(xs.takeRight(4).takeRight(4) == List<int>(2, 3, 4, 5));
        REQUIRE(xs.takeRight(4).takeRight(3) == List<int>(3, 4, 5));
        REQUIRE(xs.takeRight(4).takeRight(3).takeRight(4) == List<int>(3, 4, 5));
        REQUIRE(xs.takeRight(4).takeRight(10000).takeRight(2) == List<int>(4, 5));
    }
}
