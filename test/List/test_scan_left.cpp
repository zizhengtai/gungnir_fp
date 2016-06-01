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

TEST_CASE("test List scanLeft", "[List][scanLeft]") {

    using PI = std::unique_ptr<int>;
    using PIRef = const PI&;

    const auto toInt = [](const PI& p) { return *p; };

    SECTION("empty List") {
        List<int> xs;
        REQUIRE_NOTHROW(xs.scanLeft(123, throwOp<int, int, int>));
        REQUIRE(xs.scanLeft(123, throwOp<int, int, int>) == List<int>(123));

        List<PI> ys;
        REQUIRE_NOTHROW(ys.scanLeft(456, throwOp<int, int, PIRef>));
        REQUIRE(ys.scanLeft(456, throwOp<int, int, PIRef>) == List<int>(456));
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.scanLeft(321, [](int x, int y) { return x - y; }) ==
                List<int>(321, 321 - 123));
        REQUIRE(xs.scanLeft(List<int>(321), [](const List<int>& xs, int x) {
            return xs.prepend(x);
        }) == List<List<int>>(List<int>(321), List<int>(123, 321)));

        List<PI> ys(PI(new int(456)));
        REQUIRE(ys.scanLeft(PI(new int(654)), [](PIRef x, PIRef y) {
            return PI(new int(*x - *y));
        }).map(toInt) == List<int>(654, 654 - 456));
        REQUIRE(ys.scanLeft(List<int>(654), [](const List<int>& xs, PIRef x) {
            return xs.prepend(*x);
        }) == List<List<int>>(List<int>(654), List<int>(456, 654)));
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3, 4, 5);
        REQUIRE(xs.scanLeft(15, [](int x, int y) { return x - y; }) ==
                List<int>(15, 14, 12, 9, 5, 0));
        REQUIRE(xs.scanLeft(List<int>(0), [](const List<int>& xs, int x) {
            return xs.prepend(x);
        }) == List<List<int>>(
            List<int>(0),
            List<int>(1, 0),
            List<int>(2, 1, 0),
            List<int>(3, 2, 1, 0),
            List<int>(4, 3, 2, 1, 0),
            List<int>(5, 4, 3, 2, 1, 0)
        ));

        List<PI> ys(
            PI(new int(5)),
            PI(new int(4)),
            PI(new int(3)),
            PI(new int(2)),
            PI(new int(1))
        );
        REQUIRE(ys.scanLeft(PI(new int(15)), [](PIRef x, PIRef y) {
            return PI(new int(*x - *y));
        }).map(toInt) == List<int>(15, 10, 6, 3, 1, 0));
        REQUIRE(ys.scanLeft(List<PI>(PI(new int(6))), [](const List<PI>& xs, PIRef x) {
            return xs.prepend(PI(new int(*x)));
        }).map([&toInt](const List<PI>& xs) {
            return xs.map(toInt);
        }) == List<List<int>>(
            List<int>(6),
            List<int>(5, 6),
            List<int>(4, 5, 6),
            List<int>(3, 4, 5, 6),
            List<int>(2, 3, 4, 5, 6),
            List<int>(1, 2, 3, 4, 5, 6)
        ));
    }
}
