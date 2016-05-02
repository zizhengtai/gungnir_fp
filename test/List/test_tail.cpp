#include <memory>
#include <stdexcept>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List tail", "[List][tail]") {
    SECTION("empty List") {
        List<int> xs;
        REQUIRE_THROWS_AS(xs.tail(), std::out_of_range);

        List<std::unique_ptr<int>> ys;
        REQUIRE_THROWS_AS(ys.tail(), std::out_of_range);
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.tail().isEmpty());

        List<std::unique_ptr<int>> ys(std::unique_ptr<int>(new int(456)));
        REQUIRE(ys.tail().isEmpty());
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3, 4, 5);
        REQUIRE(xs.tail() == List<int>(2, 3, 4, 5));
        REQUIRE(xs.tail().tail() == List<int>(3, 4, 5));

        List<std::unique_ptr<int>> ys(
            std::unique_ptr<int>(new int(5)),
            std::unique_ptr<int>(new int(4)),
            std::unique_ptr<int>(new int(3)),
            std::unique_ptr<int>(new int(2)),
            std::unique_ptr<int>(new int(1))
        );
        REQUIRE(ys.tail().size() == 4);
        for (int i = 0; i < 4; ++i) {
            REQUIRE(ys.tail()[i] == ys[i + 1]);
        }
        REQUIRE(ys.tail().tail().size() == 3);
        for (int i = 0; i < 3; ++i) {
            REQUIRE(ys.tail().tail()[i] == ys[i + 2]);
        }
    }
}
