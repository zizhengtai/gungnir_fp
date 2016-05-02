#include <algorithm>
#include <memory>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List take", "[List][take]") {
    SECTION("empty List") {
        List<int> xs;
        REQUIRE(xs.take(0).isEmpty());
        REQUIRE(xs.take(1).isEmpty());
        REQUIRE(xs.take(2).isEmpty());
        REQUIRE(xs.take(10000).isEmpty());

        List<std::unique_ptr<int>> ys;
        REQUIRE(ys.take(0).isEmpty());
        REQUIRE(ys.take(1).isEmpty());
        REQUIRE(ys.take(2).isEmpty());
        REQUIRE(ys.take(10000).isEmpty());
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.take(0).isEmpty());
        REQUIRE(xs.take(1) == xs);
        REQUIRE(xs.take(2) == xs);
        REQUIRE(xs.take(3) == xs);
        REQUIRE(xs.take(10000) == xs);

        List<std::unique_ptr<int>> ys(std::unique_ptr<int>(new int(456)));
        REQUIRE(ys.take(0).isEmpty());
        REQUIRE(ys.take(1) == ys);
        REQUIRE(ys.take(2) == ys);
        REQUIRE(ys.take(3) == ys);
        REQUIRE(ys.take(10000) == ys);
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3, 4);
        REQUIRE(xs.take(0) == List<int>());
        REQUIRE(xs.take(1) == List<int>(1));
        REQUIRE(xs.take(2) == List<int>(1, 2));
        REQUIRE(xs.take(3) == List<int>(1, 2, 3));
        REQUIRE(xs.take(4) == List<int>(1, 2, 3, 4));
        REQUIRE(xs.take(5) == List<int>(1, 2, 3, 4));
        REQUIRE(xs.take(10000) == List<int>(1, 2, 3, 4));

        List<std::unique_ptr<int>> ys(
            std::unique_ptr<int>(new int(4)),
            std::unique_ptr<int>(new int(3)),
            std::unique_ptr<int>(new int(2)),
            std::unique_ptr<int>(new int(1))
        );
        for (std::size_t i: {0, 1, 2, 3, 4, 5, 10000}) {
            const auto zs = ys.take(i);
            const auto n = std::min(ys.size(), i);
            REQUIRE(zs.size() == n);
            for (std::size_t j = 0; j < n; ++j) {
                REQUIRE(zs[j] == ys[j]);
            }
        }
    }
    SECTION("chained takes") {
        List<int> xs(1, 2, 3, 4, 5);
        REQUIRE(xs.take(10000).take(10000).take(3) == List<int>(1, 2, 3));
        REQUIRE(xs.take(4).take(5) == List<int>(1, 2, 3, 4));
        REQUIRE(xs.take(4).take(4) == List<int>(1, 2, 3, 4));
        REQUIRE(xs.take(4).take(3) == List<int>(1, 2, 3));
        REQUIRE(xs.take(4).take(3).take(4) == List<int>(1, 2, 3));
        REQUIRE(xs.take(4).take(10000).take(2) == List<int>(1, 2));
    }
}
