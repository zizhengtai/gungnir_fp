#include <memory>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List reverse", "[List][reverse]") {
    SECTION("empty List") {
        List<int> xs;
        REQUIRE(xs.reverse() == xs);
        REQUIRE(xs.reverse().reverse() == xs);

        List<std::unique_ptr<int>> ys;
        REQUIRE(ys.reverse() == ys);
        REQUIRE(ys.reverse().reverse() == ys);
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.reverse() == xs);
        REQUIRE(xs.reverse().reverse() == xs);

        List<std::unique_ptr<int>> ys(std::unique_ptr<int>(new int(456)));
        REQUIRE(ys.reverse() == ys);
        REQUIRE(ys.reverse().reverse() == ys);
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3, 4, 5);
        REQUIRE(xs.reverse() == List<int>(5, 4, 3, 2, 1));
        REQUIRE(xs.reverse().reverse() == xs);

        List<std::unique_ptr<int>> ys(
            std::unique_ptr<int>(new int(5)),
            std::unique_ptr<int>(new int(4)),
            std::unique_ptr<int>(new int(3)),
            std::unique_ptr<int>(new int(2)),
            std::unique_ptr<int>(new int(1))
        );
        const auto ysr = ys.reverse();
        REQUIRE(ysr.size() == 5);
        REQUIRE(ysr[0] == ys[4]);
        REQUIRE(ysr[1] == ys[3]);
        REQUIRE(ysr[2] == ys[2]);
        REQUIRE(ysr[3] == ys[1]);
        REQUIRE(ysr[4] == ys[0]);
        REQUIRE(ys.reverse().reverse() == ys);
    }
}
