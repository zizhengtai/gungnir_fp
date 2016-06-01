#include <memory>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

template<typename T> static bool alwaysTrue(T) { return true; }
template<typename T> static bool alwaysFalse(T) { return false; }
static bool isEven(int x) { return x % 2 == 0; }
static bool isOdd(int x) { return x % 2 != 0; }

TEST_CASE("test List count", "[List][count]") {

    using PI = std::unique_ptr<int>;

    SECTION("empty List") {
        List<int> xs;
        REQUIRE(xs.count(0) == 0);
        REQUIRE(xs.count(1) == 0);
        REQUIRE(xs.count(alwaysTrue<int>) == 0);
        REQUIRE(xs.count(alwaysFalse<int>) == 0);

        List<PI> ys;
        REQUIRE(ys.count(PI()) == 0);
        REQUIRE(ys.count(PI(new int(123))) == 0);
        REQUIRE(ys.count(alwaysTrue<const PI &>) == 0);
        REQUIRE(ys.count(alwaysFalse<const PI &>) == 0);
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.count(122) == 0);
        REQUIRE(xs.count(123) == 1);
        REQUIRE(xs.count(alwaysTrue<int>) == 1);
        REQUIRE(xs.count(alwaysFalse<int>) == 0);
        REQUIRE(xs.count(isEven) == 0);
        REQUIRE(xs.count(isOdd) == 1);

        List<PI> ys(PI(new int(456)));
        REQUIRE(ys.count(PI()) == 0);
        REQUIRE(ys.count(PI(new int(456))) == 0);
        REQUIRE(ys.count(ys.head()) == 1);
        REQUIRE(ys.count(alwaysTrue<const PI&>) == 1);
        REQUIRE(ys.count(alwaysFalse<const PI&>) == 0);
        REQUIRE(ys.count([](const PI& p) { return isEven(*p); }) == 1);
        REQUIRE(ys.count([](const PI& p) { return isOdd(*p); }) == 0);
        REQUIRE(ys.count([](const PI& p) { return *p == 456; }) == 1);
    }
    SECTION("List with multiple element") {
        List<int> xs(1, 2, 3, 2, 5, 3, 2);
        REQUIRE(xs.count(1) == 1);
        REQUIRE(xs.count(2) == 3);
        REQUIRE(xs.count(3) == 2);
        REQUIRE(xs.count(4) == 0);
        REQUIRE(xs.count(5) == 1);
        REQUIRE(xs.count(alwaysTrue<int>) == 7);
        REQUIRE(xs.count(alwaysFalse<int>) == 0);
        REQUIRE(xs.count(isEven) == 3);
        REQUIRE(xs.count(isOdd) == 4);

        List<PI> ys = xs.map([](int x) { return PI(new int(x)); });
        for (std::size_t i = 0; i < ys.size(); ++i) {
            REQUIRE(ys.count(ys[i]) == 1);
            REQUIRE(ys.count(PI(new int(*ys[i]))) == 0);
        }
        REQUIRE(ys.count(alwaysTrue<const PI&>) == 7);
        REQUIRE(ys.count(alwaysFalse<const PI&>) == 0);
        REQUIRE(ys.count([](const PI& p) { return isEven(*p); }) == 3);
        REQUIRE(ys.count([](const PI& p) { return isOdd(*p); }) == 4);
        REQUIRE(ys.count([](const PI& p) { return *p == 1; }) == 1);
        REQUIRE(ys.count([](const PI& p) { return *p == 2; }) == 3);
        REQUIRE(ys.count([](const PI& p) { return *p == 3; }) == 2);
        REQUIRE(ys.count([](const PI& p) { return *p == 4; }) == 0);
        REQUIRE(ys.count([](const PI& p) { return *p == 5; }) == 1);
    }
}
