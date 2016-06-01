#include <memory>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

template<typename T> static bool alwaysTrue(T) { return true; }
template<typename T> static bool alwaysFalse(T) { return false; }
static bool isEven(int x) { return x % 2 == 0; }
static bool isOdd(int x) { return x % 2 != 0; }
template<int n> static bool gt(int x) { return x > n; }
template<int n> static bool lt(int x) { return x < n; }

TEST_CASE("test List takeWhile", "[List][takeWhile]") {

    using PI = std::unique_ptr<int>;

    SECTION("empty List") {
        List<int> xs;
        REQUIRE(xs.takeWhile(alwaysTrue<int>).isEmpty());
        REQUIRE(xs.takeWhile(alwaysFalse<int>).isEmpty());

        List<PI> ys;
        REQUIRE(ys.takeWhile(alwaysTrue<const PI&>).isEmpty());
        REQUIRE(ys.takeWhile(alwaysFalse<const PI&>).isEmpty());
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.takeWhile(alwaysTrue<int>) == List<int>(123));
        REQUIRE(xs.takeWhile(alwaysFalse<int>).isEmpty());
        REQUIRE(xs.takeWhile(isEven).isEmpty());
        REQUIRE(xs.takeWhile(isOdd) == List<int>(123));

        List<PI> ys(PI(new int(456)));
        REQUIRE(ys.takeWhile(alwaysTrue<const PI&>) == ys);
        REQUIRE(ys.takeWhile(alwaysFalse<const PI&>).isEmpty());
        REQUIRE(ys.takeWhile([](const PI& p) { return isEven(*p); }) == ys);
        REQUIRE(ys.takeWhile([](const PI& p) { return isOdd(*p); }).isEmpty());
    }
    SECTION("List with multiple elements") {
        List<int> xs(2, 4, 1, 5, 3);
        REQUIRE(xs.takeWhile(alwaysTrue<int>) == List<int>(2, 4, 1, 5, 3));
        REQUIRE(xs.takeWhile(alwaysFalse<int>).isEmpty());
        REQUIRE(xs.takeWhile(gt<1>) == List<int>(2, 4));
        REQUIRE(xs.takeWhile(gt<0>) == List<int>(2, 4, 1, 5, 3));
        REQUIRE(xs.takeWhile(lt<5>) == List<int>(2, 4, 1));
        REQUIRE(xs.takeWhile(lt<1>) == List<int>());

        List<PI> ys(
            PI(new int(2)),
            PI(new int(4)),
            PI(new int(1)),
            PI(new int(5)),
            PI(new int(3))
        );
        REQUIRE(ys.takeWhile(alwaysTrue<const PI&>) == ys);
        REQUIRE(ys.takeWhile(alwaysFalse<const PI&>).isEmpty());
        REQUIRE(ys.takeWhile([](const PI& p) { return *p > 1; }) == ys.take(2));
        REQUIRE(ys.takeWhile([](const PI& p) { return *p > 0; }) == ys);
        REQUIRE(ys.takeWhile([](const PI& p) { return *p < 5; }) == ys.take(3));
        REQUIRE(ys.takeWhile([](const PI& p) { return *p < 1; }) == List<PI>());
    }
}
