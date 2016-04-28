#include <memory>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List constructors", "[constructor]") {
    SECTION("empty List") {
        List<int> xs;
        REQUIRE(xs.isEmpty());
        REQUIRE(xs.size() == 0);

        List<std::unique_ptr<int>> ys;
        REQUIRE(ys.isEmpty());
        REQUIRE(ys.size() == 0);
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE_FALSE(xs.isEmpty());
        REQUIRE(xs.size() == 1);
        REQUIRE(xs.head() == 123);
        REQUIRE(xs.tail().isEmpty());
        REQUIRE(xs.tail().size() == 0);

        List<std::unique_ptr<int>> ys(std::unique_ptr<int>(new int(456)));
        REQUIRE_FALSE(ys.isEmpty());
        REQUIRE(ys.size() == 1);
        REQUIRE(*ys.head() == 456);
        REQUIRE(ys.tail().isEmpty());
        REQUIRE(ys.tail().size() == 0);
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3, 4, 5);
        REQUIRE_FALSE(xs.isEmpty());
        REQUIRE(xs.size() == 5);
        REQUIRE(xs.head() == 1);
        REQUIRE_FALSE(xs.tail().isEmpty());
        REQUIRE(xs.tail().size() == 4);
        REQUIRE(xs.tail().head() == 2);
        REQUIRE_FALSE(xs.tail().tail().tail().tail().isEmpty());
        REQUIRE(xs.tail().tail().tail().tail().size() == 1);
        REQUIRE(xs.tail().tail().tail().tail().head() == 5);
        REQUIRE(xs.tail().tail().tail().tail().tail().isEmpty());
        REQUIRE(xs.tail().tail().tail().tail().tail().size() == 0);

        List<std::unique_ptr<int>> ys(
            std::unique_ptr<int>(new int(5)),
            std::unique_ptr<int>(new int(4)),
            std::unique_ptr<int>(new int(3)),
            std::unique_ptr<int>(new int(2)),
            std::unique_ptr<int>(new int(1))
        );
        REQUIRE_FALSE(ys.isEmpty());
        REQUIRE(ys.size() == 5);
        REQUIRE(*ys.head() == 5);
        REQUIRE_FALSE(ys.tail().isEmpty());
        REQUIRE(ys.tail().size() == 4);
        REQUIRE(*ys.tail().head() == 4);
        REQUIRE_FALSE(ys.tail().tail().tail().tail().isEmpty());
        REQUIRE(ys.tail().tail().tail().tail().size() == 1);
        REQUIRE(*ys.tail().tail().tail().tail().head() == 1);
        REQUIRE(ys.tail().tail().tail().tail().tail().isEmpty());
        REQUIRE(ys.tail().tail().tail().tail().tail().size() == 0);
    }
}
