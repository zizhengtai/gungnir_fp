#include <memory>
#include <stdexcept>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List constructors", "[List][constructor]") {
    SECTION("empty List") {
        List<int> xs;
        REQUIRE(xs.isEmpty());
        REQUIRE(xs.size() == 0);
        REQUIRE_THROWS_AS(xs.head(), std::out_of_range);
        REQUIRE_THROWS_AS(xs.tail(), std::out_of_range);

        List<std::unique_ptr<int>> ys;
        REQUIRE(ys.isEmpty());
        REQUIRE(ys.size() == 0);
        REQUIRE_THROWS_AS(ys.head(), std::out_of_range);
        REQUIRE_THROWS_AS(ys.tail(), std::out_of_range);
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE_FALSE(xs.isEmpty());
        REQUIRE(xs.size() == 1);
        REQUIRE(xs.head() == 123);
        REQUIRE(xs.tail().isEmpty());
        REQUIRE(xs.tail().size() == 0);
        REQUIRE_THROWS_AS(xs.tail().head(), std::out_of_range);
        REQUIRE_THROWS_AS(xs.tail().tail(), std::out_of_range);

        List<std::unique_ptr<int>> ys(std::unique_ptr<int>(new int(456)));
        REQUIRE_FALSE(ys.isEmpty());
        REQUIRE(ys.size() == 1);
        REQUIRE(*ys.head() == 456);
        REQUIRE(ys.tail().isEmpty());
        REQUIRE(ys.tail().size() == 0);
        REQUIRE_THROWS_AS(ys.tail().head(), std::out_of_range);
        REQUIRE_THROWS_AS(ys.tail().tail(), std::out_of_range);
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3, 4, 5);
        REQUIRE_FALSE(xs.isEmpty());
        REQUIRE(xs.size() == 5);
        REQUIRE(xs.tail().size() == 4);
        REQUIRE(xs.tail().tail().size() == 3);
        REQUIRE(xs.tail().tail().tail().size() == 2);
        REQUIRE(xs.tail().tail().tail().tail().size() == 1);
        REQUIRE(xs.tail().tail().tail().tail().tail().size() == 0);
        REQUIRE_THROWS_AS(
            xs.tail().tail().tail().tail().tail().head(), std::out_of_range);
        REQUIRE_THROWS_AS(
            xs.tail().tail().tail().tail().tail().tail(), std::out_of_range);
        REQUIRE(xs[0] == 1);
        REQUIRE(xs[1] == 2);
        REQUIRE(xs[2] == 3);
        REQUIRE(xs[3] == 4);
        REQUIRE(xs[4] == 5);
        REQUIRE_THROWS_AS(xs[5], std::out_of_range);

        List<std::unique_ptr<int>> ys(
            std::unique_ptr<int>(new int(5)),
            std::unique_ptr<int>(new int(4)),
            std::unique_ptr<int>(new int(3)),
            std::unique_ptr<int>(new int(2)),
            std::unique_ptr<int>(new int(1))
        );
        REQUIRE_FALSE(ys.isEmpty());
        REQUIRE(ys.size() == 5);
        REQUIRE(ys.tail().size() == 4);
        REQUIRE(ys.tail().tail().size() == 3);
        REQUIRE(ys.tail().tail().tail().size() == 2);
        REQUIRE(ys.tail().tail().tail().tail().size() == 1);
        REQUIRE(ys.tail().tail().tail().tail().tail().size() == 0);
        REQUIRE_THROWS_AS(
            ys.tail().tail().tail().tail().tail().head(), std::out_of_range);
        REQUIRE_THROWS_AS(
            ys.tail().tail().tail().tail().tail().tail(), std::out_of_range);
        REQUIRE(*ys[0] == 5);
        REQUIRE(*ys[1] == 4);
        REQUIRE(*ys[2] == 3);
        REQUIRE(*ys[3] == 2);
        REQUIRE(*ys[4] == 1);
        REQUIRE_THROWS_AS(*ys[5], std::out_of_range);
    }
}
