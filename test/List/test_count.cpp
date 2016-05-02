#include <memory>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List count", "[List][count]") {

    using PI = std::unique_ptr<int>;

    SECTION("empty List") {
        List<int> xs;
        REQUIRE(xs.count(0) == 0);
        REQUIRE(xs.count(1) == 0);

        List<PI> ys;
        REQUIRE(ys.count(PI()) == 0);
        REQUIRE(ys.count(PI(new int(123))) == 0);
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.count(122) == 0);
        REQUIRE(xs.count(123) == 1);

        List<PI> ys(PI(new int(456)));
        REQUIRE(ys.count(PI()) == 0);
        REQUIRE(ys.count(PI(new int(456))) == 0);
        REQUIRE(ys.count(ys.head()) == 1);
    }
    SECTION("List with multiple element") {
        List<int> xs(1, 2, 3, 2, 5, 3, 2);
        REQUIRE(xs.count(1) == 1);
        REQUIRE(xs.count(2) == 3);
        REQUIRE(xs.count(3) == 2);
        REQUIRE(xs.count(4) == 0);
        REQUIRE(xs.count(5) == 1);

        List<PI> ys = xs.map([](int x) { return PI(new int(x)); });
        for (std::size_t i = 0; i < ys.size(); ++i) {
            REQUIRE(ys.count(ys[i]) == 1);
            REQUIRE(ys.count(PI(new int(*ys[i]))) == 0);
        }
    }
}
