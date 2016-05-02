#include <memory>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List contains", "[List][contains]") {
    SECTION("empty List") {
        List<int> xs;
        REQUIRE_FALSE(xs.contains(0));
        REQUIRE_FALSE(xs.contains(1));

        List<std::unique_ptr<int>> ys;
        REQUIRE_FALSE(ys.contains(std::unique_ptr<int>()));
        REQUIRE_FALSE(ys.contains(std::unique_ptr<int>(new int(123))));
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.contains(123));
        REQUIRE_FALSE(xs.contains(456));

        List<std::unique_ptr<int>> ys(std::unique_ptr<int>(new int(456)));
        REQUIRE(ys.contains(ys.head()));
        REQUIRE_FALSE(ys.contains(std::unique_ptr<int>(new int(456))));
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3, 4, 5);
        REQUIRE(xs.contains(1));
        REQUIRE(xs.contains(3));
        REQUIRE(xs.contains(5));
        REQUIRE_FALSE(xs.contains(0));
        REQUIRE_FALSE(xs.contains(6));

        List<std::unique_ptr<int>> ys(
            std::unique_ptr<int>(new int(5)),
            std::unique_ptr<int>(new int(4)),
            std::unique_ptr<int>(new int(3)),
            std::unique_ptr<int>(new int(2)),
            std::unique_ptr<int>(new int(1))
        );
        REQUIRE(ys.contains(ys[0]));
        REQUIRE(ys.contains(ys[2]));
        REQUIRE(ys.contains(ys[4]));
        REQUIRE_FALSE(ys.contains(std::unique_ptr<int>(new int(0))));
        REQUIRE_FALSE(ys.contains(std::unique_ptr<int>(new int(3))));
        REQUIRE_FALSE(ys.contains(std::unique_ptr<int>(new int(6))));
    }
}
