#include <memory>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

template<typename T> static bool alwaysTrue(T) { return true; }
template<typename T> static bool alwaysFalse(T) { return false; }
static bool isEven(int x) { return x % 2 == 0; }
static bool isOdd(int x) { return x % 2 != 0; }

TEST_CASE("test List exists", "[List][exists]") {

    using PI = std::unique_ptr<int>;

    SECTION("empty List") {
        List<int> xs;
        REQUIRE_FALSE(xs.exists(alwaysTrue<int>));
        REQUIRE_FALSE(xs.exists(alwaysFalse<int>));

        List<PI> ys;
        REQUIRE_FALSE(ys.exists(alwaysTrue<const PI&>));
        REQUIRE_FALSE(ys.exists(alwaysFalse<const PI&>));
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.exists(alwaysTrue<int>));
        REQUIRE_FALSE(xs.exists(alwaysFalse<int>));
        REQUIRE_FALSE(xs.exists(isEven));
        REQUIRE(xs.exists(isOdd));

        List<PI> ys(PI(new int(456)));
        REQUIRE(ys.exists(alwaysTrue<const PI&>));
        REQUIRE_FALSE(ys.exists(alwaysFalse<const PI&>));
        REQUIRE(ys.exists([](const PI& p) { return isEven(*p); }));
        REQUIRE_FALSE(ys.exists([](const PI& p) { return isOdd(*p); }));
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3);
        REQUIRE(xs.exists(alwaysTrue<int>));
        REQUIRE_FALSE(xs.exists(alwaysFalse<int>));
        REQUIRE(xs.exists(isEven));
        REQUIRE(xs.exists(isOdd));
        REQUIRE(xs.exists([](int x) { return x >= 3; }));
        REQUIRE_FALSE(xs.exists([](int x) { return x >= 4; }));

        List<PI> ys(PI(new int(4)), PI(new int(5)), PI(new int(6)));
        REQUIRE(ys.exists(alwaysTrue<const PI&>));
        REQUIRE_FALSE(ys.exists(alwaysFalse<const PI&>));
        REQUIRE(ys.exists([](const PI& p) { return isEven(*p); }));
        REQUIRE(ys.exists([](const PI& p) { return isOdd(*p); }));
        REQUIRE(ys.exists([](const PI& p) { return *p <= 4; }));
        REQUIRE_FALSE(ys.exists([](const PI& p) { return *p <= 3; }));
    }
}
