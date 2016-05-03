#include <memory>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List concat", "[List][concat]") {

    using LI = List<int>;
    using PI = std::unique_ptr<int>;
    using LPI = List<PI>;

    SECTION("empty Lists") {
        REQUIRE(LI().concat(LI()).isEmpty());

        REQUIRE(LPI().concat(LPI()).isEmpty());
    }
    SECTION("Lists with one element") {
        REQUIRE(LI(123).concat(LI()) == LI(123));
        REQUIRE(LI().concat(LI(321)) == LI(321));
        REQUIRE(LI(123).concat(LI(321)) == LI(123, 321));

        const auto ys1 = LPI(PI(new int(456))).concat(LPI());
        REQUIRE(ys1.size() == 1);
        REQUIRE(*ys1.head() == 456);
        const auto ys2 = LPI().concat(LPI(PI(new int(654))));
        REQUIRE(ys2.size() == 1);
        REQUIRE(*ys2.head() == 654);
        const auto ys3 = ys1.concat(ys2);
        REQUIRE(ys3.size() == 2);
        REQUIRE(*ys3.head() == 456);
        REQUIRE(*ys3.tail().head() == 654);
    }
    SECTION("Lists with multiple elements") {
        REQUIRE(LI(1, 2, 3).concat(LI()) == LI(1, 2, 3));
        REQUIRE(LI().concat(LI(3, 2, 1)) == LI(3, 2, 1));
        REQUIRE(LI(1, 2, 3).concat(LI(4, 5 ,6)) == LI(1, 2, 3, 4, 5, 6));

        const auto ys1 =
            LPI(PI(new int(6)), PI(new int(5)), PI(new int(4)));
        const auto ys2 =
            LPI(PI(new int(3)), PI(new int(2)), PI(new int(1)));
        REQUIRE(ys1.concat(LPI()) == ys1);
        REQUIRE(LPI().concat(ys2) == ys2);
        const auto ys3 = ys1.concat(ys2);
        REQUIRE(ys3.size() == 6);
        for (int i = 0; i < 6; ++i) {
            REQUIRE(*ys3[i] == 6 - i);
        }
    }
}
