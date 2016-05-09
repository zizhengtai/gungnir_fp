#include <memory>
#include <stdexcept>
#include <tuple>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List uncons", "[List][uncons]") {

    using PI = std::unique_ptr<int>;

    SECTION("empty List") {
        List<int> xs;
        REQUIRE_THROWS_AS(xs.uncons(), std::out_of_range);
        REQUIRE(xs.isEmpty());
        REQUIRE(xs.size() == 0);

        List<PI> ys;
        REQUIRE_THROWS_AS(ys.uncons(), std::out_of_range);
        REQUIRE(ys.isEmpty());
        REQUIRE(ys.size() == 0);
    }
    SECTION("List with one element") {
        List<int> xs1(123);
        REQUIRE_NOTHROW(xs1.uncons());
        int x;
        List<int> xs2;
        std::tie(x, xs2) = xs1.uncons();
        REQUIRE(x == 123);
        REQUIRE(xs2.isEmpty());
        REQUIRE(xs2.size() == 0);
        REQUIRE_THROWS_AS(xs2.uncons(), std::out_of_range);
        REQUIRE(!xs1.isEmpty());
        REQUIRE(xs1 == List<int>(123));

        List<PI> ys(PI(new int(456)));
        REQUIRE_NOTHROW(ys.uncons());
        REQUIRE(*ys.uncons().first.get() == 456);
        REQUIRE(ys.uncons().second.isEmpty());
        REQUIRE(ys.uncons().second.size() == 0);
        REQUIRE_THROWS_AS(ys.uncons().second.uncons(), std::out_of_range);
        REQUIRE(!ys.isEmpty());
        REQUIRE(ys.size() == 1);
        REQUIRE(ys.head() != nullptr);
        REQUIRE(*ys.head() == 456);
    }
    SECTION("List with multiple elements") {
        List<int> xs1(1, 2, 3, 4, 5);
        int x, i = 1;
        List<int> xs2 = xs1;
        do {
            std::tie(x, xs2) = xs2.uncons();
            REQUIRE(x == i);
            REQUIRE(xs2.size() == 5 - i);
            ++i;
        } while (!xs2.isEmpty());
        REQUIRE(xs2.isEmpty());
        REQUIRE(xs2.size() == 0);
        REQUIRE_THROWS_AS(xs2.uncons(), std::out_of_range);
        REQUIRE(!xs1.isEmpty());
        REQUIRE(xs1.size() == 5);
        REQUIRE(xs1 == List<int>(1, 2, 3, 4, 5));

        List<PI> ys1 = xs1.map([](int x) { return PI(new int(6 - x)); });
        List<PI> ys2 = ys1;
        const PI tmp(new int(-1));
        std::reference_wrapper<const PI> y(tmp);
        i = 5;
        do {
            std::tie(y, ys2) = ys2.uncons();
            REQUIRE(*y.get() == i);
            REQUIRE(ys2.size() == i - 1);
            --i;
        } while (!ys2.isEmpty());
        REQUIRE(ys2.isEmpty());
        REQUIRE(ys2.size() == 0);
        REQUIRE(!ys1.isEmpty());
        REQUIRE(ys1.size() == 5);
        for (i = 0; i < 5; ++i) {
            REQUIRE(ys1[i] != nullptr);
            REQUIRE(*ys1[i] == 5 - i);
        }
    }
}
