#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List sum", "[List][sum]") {
    SECTION("empty List") {
        REQUIRE(List<int>().sum() == 0);
        REQUIRE(List<double>().sum() == Approx(0));
    }
    SECTION("List with one element") {
        REQUIRE(List<int>(123).sum() == 123);
        REQUIRE(List<double>(456).sum() == Approx(456));
    }
    SECTION("List with multiple elements") {
        REQUIRE(List<int>(1, 2, 3, 4, 5).sum() == (1 + 5) * 5 / 2);
        REQUIRE(List<double>(5, 4, 3, 2, 1).sum() == Approx((5 + 1) * 5 / 2));
    }
}
