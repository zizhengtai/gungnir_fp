#include <memory>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

template<typename T> static bool alwaysTrue(T) { return true; }
template<typename T> static bool alwaysFalse(T) { return false; }
static bool isEven(int x) { return x % 2 == 0; }
static bool isOdd(int x) { return x % 2 != 0; }

TEST_CASE("test List forall", "[List][forall]") {

    using PI = std::unique_ptr<int>;

    SECTION("empty List") {
        List<int> xs;
        REQUIRE(xs.forall(alwaysTrue<int>));
        REQUIRE(xs.forall(alwaysFalse<int>));

        List<PI> ys;
        REQUIRE(ys.forall(alwaysTrue<const PI&>));
        REQUIRE(ys.forall(alwaysFalse<const PI&>));
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.forall(alwaysTrue<int>));
        REQUIRE_FALSE(xs.forall(alwaysFalse<int>));
        REQUIRE_FALSE(xs.forall(isEven));
        REQUIRE(xs.forall(isOdd));

        List<PI> ys(PI(new int(456)));
        REQUIRE(ys.forall(alwaysTrue<const PI&>));
        REQUIRE_FALSE(ys.forall(alwaysFalse<const PI&>));
        REQUIRE(ys.forall([](const PI& p) { return isEven(*p); }));
        REQUIRE_FALSE(ys.forall([](const PI& p) { return isOdd(*p); }));
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3);
        REQUIRE(xs.forall(alwaysTrue<int>));
        REQUIRE_FALSE(xs.forall(alwaysFalse<int>));
        REQUIRE_FALSE(xs.forall(isEven));
        REQUIRE_FALSE(xs.forall(isOdd));
        REQUIRE(xs.forall([](int x) { return x <= 3; }));
        REQUIRE_FALSE(xs.forall([](int x) { return x <= 2; }));

        List<PI> ys(PI(new int(4)), PI(new int(5)), PI(new int(6)));
        REQUIRE(ys.forall(alwaysTrue<const PI&>));
        REQUIRE_FALSE(ys.forall(alwaysFalse<const PI&>));
        REQUIRE_FALSE(ys.forall([](const PI& p) { return isEven(*p); }));
        REQUIRE_FALSE(ys.forall([](const PI& p) { return isOdd(*p); }));
        REQUIRE(ys.forall([](const PI& p) { return *p >= 4; }));
        REQUIRE_FALSE(ys.forall([](const PI& p) { return *p >= 5; }));
    }
}
