#include <memory>
#include <stdexcept>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List last", "[List][last]") {
    SECTION("empty List") {
        List<int> xs;
        REQUIRE_THROWS_AS(xs.last(), std::out_of_range);

        List<std::unique_ptr<int>> ys;
        REQUIRE_THROWS_AS(ys.last(), std::out_of_range);
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.last() == 123);

        List<std::unique_ptr<int>> ys(std::unique_ptr<int>(new int(456)));
        REQUIRE(*ys.last() == 456);
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3, 4, 5);
        REQUIRE(xs.last() == 5);

        List<std::unique_ptr<int>> ys(
            std::unique_ptr<int>(new int(5)),
            std::unique_ptr<int>(new int(4)),
            std::unique_ptr<int>(new int(3)),
            std::unique_ptr<int>(new int(2)),
            std::unique_ptr<int>(new int(1))
        );
        REQUIRE(*ys.last() == 1);
    }
}
