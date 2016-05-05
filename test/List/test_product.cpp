#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List product", "[List][product]") {
    SECTION("empty List") {
        REQUIRE(List<int>().product() == 1);
        REQUIRE(List<double>().product() == Approx(1));
    }
    SECTION("List with one element") {
        REQUIRE(List<int>(123).product() == 123);
        REQUIRE(List<double>(456).product() == Approx(456));
    }
    SECTION("List with multiple elements") {
        REQUIRE(List<int>(1, 2, 3, 4, 5).product() == 1 * 2 * 3 * 4 * 5);
        REQUIRE(List<double>(5, 4, 3, 2, 1).product() == Approx(5 * 4 * 3 * 2 * 1));
    }
}
