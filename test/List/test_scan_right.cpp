#include <memory>
#include <stdexcept>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

template<typename Ret, typename A, typename B>
static Ret throwOp(A, B)
{
    throw std::runtime_error("");
}

TEST_CASE("test List scanRight", "[List][scanRight]") {

    using PI = std::unique_ptr<int>;
    using PIRef = const PI &;

    const auto toInt = [](const PI &p) { return *p; };

    SECTION("empty List") {
        List<int> xs;
        REQUIRE_NOTHROW(xs.scanRight(123, throwOp<int, int, int>));
        REQUIRE(xs.scanRight(123, throwOp<int, int, int>) == List<int>(123));

        List<PI> ys;
        REQUIRE_NOTHROW(ys.scanRight(456, throwOp<int, PIRef, int>));
        REQUIRE(ys.scanRight(456, throwOp<int, PIRef, int>) == List<int>(456));
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.scanRight(321, [](int x, int y) { return x - y; }) ==
                List<int>(123 - 321, 321));
        REQUIRE(xs.scanRight(List<int>(321), [](int x, const List<int> &xs) {
            return xs.prepend(x);
        }) == List<List<int>>(List<int>(123, 321), List<int>(321)));

        List<PI> ys(PI(new int(456)));
        REQUIRE(ys.scanRight(PI(new int(654)), [](PIRef x, PIRef y) {
            return PI(new int(*x - *y));
        }).map(toInt) == List<int>(456 - 654, 654));
        REQUIRE(ys.scanRight(List<int>(654), [](PIRef x, const List<int> &xs) {
            return xs.prepend(*x);
        }) == List<List<int>>(List<int>(456, 654), List<int>(654)));
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3, 4, 5);
        REQUIRE(xs.scanRight(15, [](int x, int y) { return x - y; }) ==
                List<int>(-12, 13, -11, 14, -10, 15));
        REQUIRE(xs.scanRight(List<int>(6), [](int x, const List<int> &xs) {
            return xs.prepend(x);
        }) == List<List<int>>(
            List<int>(1, 2, 3, 4, 5, 6),
            List<int>(2, 3, 4, 5, 6),
            List<int>(3, 4, 5, 6),
            List<int>(4, 5, 6),
            List<int>(5, 6),
            List<int>(6)
        ));

        List<PI> ys(
            PI(new int(5)),
            PI(new int(4)),
            PI(new int(3)),
            PI(new int(2)),
            PI(new int(1))
        );
        REQUIRE(ys.scanRight(PI(new int(15)), [](PIRef x, PIRef y) {
            return PI(new int(*x - *y));
        }).map(toInt) == List<int>(-12, 17, -13, 16, -14, 15));
        REQUIRE(ys.scanRight(List<PI>(PI(new int(0))), [](PIRef x, const List<PI> &xs) {
            return xs.prepend(PI(new int(*x)));
        }).map([&toInt](const List<PI> &xs) {
            return xs.map(toInt);
        }) == List<List<int>>(
            List<int>(5, 4, 3, 2, 1, 0),
            List<int>(4, 3, 2, 1, 0),
            List<int>(3, 2, 1, 0),
            List<int>(2, 1, 0),
            List<int>(1, 0),
            List<int>(0)
        ));
    }
}
