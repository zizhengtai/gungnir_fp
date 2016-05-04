#include <memory>
#include <stdexcept>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List updated", "[List][updated]") {

    using PI = std::unique_ptr<int>;

    SECTION("empty List") {
        List<int> xs;
        REQUIRE_THROWS_AS(xs.updated(0, 123), std::out_of_range);
        REQUIRE_THROWS_AS(xs.updated(1, 123), std::out_of_range);
        REQUIRE(xs.isEmpty());

        List<PI> ys;
        REQUIRE_THROWS_AS(ys.updated(0, PI(new int(456))), std::out_of_range);
        REQUIRE_THROWS_AS(ys.updated(1, PI(new int(456))), std::out_of_range);
        REQUIRE(ys.isEmpty());
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.updated(0, 321) == List<int>(321));
        REQUIRE_THROWS_AS(xs.updated(1, 321), std::out_of_range);
        REQUIRE(xs == List<int>(123));

        List<PI> ys1(PI(new int(456)));
        const auto ys2 = ys1.updated(0, PI(new int(654)));
        REQUIRE(ys2.size() == 1);
        REQUIRE(ys2.head() != nullptr);
        REQUIRE(*ys2.head() == 654);
        REQUIRE_THROWS_AS(ys1.updated(1, PI(new int(654))), std::out_of_range);
        REQUIRE(ys1.size() == 1);
        REQUIRE(ys1.head() != nullptr);
        REQUIRE(*ys1.head() == 456);
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3, 4, 5);
        REQUIRE(xs.updated(0, 123) == List<int>(123, 2, 3, 4, 5));
        REQUIRE(xs.updated(2, 123) == List<int>(1, 2, 123, 4, 5));
        REQUIRE(xs.updated(4, 123) == List<int>(1, 2, 3, 4, 123));
        REQUIRE_THROWS_AS(xs.updated(5, 123), std::out_of_range);
        REQUIRE(xs == List<int>(1, 2, 3, 4, 5));

        List<PI> ys1(
            PI(new int(5)),
            PI(new int(4)),
            PI(new int(3)),
            PI(new int(2)),
            PI(new int(1))
        );
        const auto ys2 = ys1.updated(0, PI(new int(321)));
        const auto ys3 = ys1.updated(2, PI(new int(321)));
        const auto ys4 = ys1.updated(4, PI(new int(321)));
        const auto toInt = [](const PI &p) { return *p; };
        REQUIRE(ys2.map(toInt) == List<int>(321, 4, 3, 2, 1));
        REQUIRE(ys3.map(toInt) == List<int>(5, 4, 321, 2, 1));
        REQUIRE(ys4.map(toInt) == List<int>(5, 4, 3, 2, 321));
        REQUIRE_THROWS_AS(ys1.updated(5, PI(new int(321))), std::out_of_range);
        REQUIRE(ys1.map(toInt) == List<int>(5, 4, 3, 2, 1));
    }
}
