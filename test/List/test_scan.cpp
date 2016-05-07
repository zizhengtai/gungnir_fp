#include <memory>
#include <stdexcept>
#include <string>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

template<typename Ret, typename A, typename B>
static Ret throwOp(A, B)
{
    throw std::runtime_error("");
}

TEST_CASE("test List scan", "[List][scan]") {

    using PI = std::unique_ptr<int>;
    using PIRef = const PI &;

    struct Base {
        Base(int x) noexcept: val(x) {}
        int val;
    };
    struct Derived : Base {
        Derived(int x) noexcept : Base(x) {}
    };

    const auto toInt = [](const PI &p) { return *p; };

    SECTION("empty List") {
        List<int> xs;
        REQUIRE_NOTHROW(xs.scan(123, throwOp<int, int, int>));
        REQUIRE(xs.scan(123, throwOp<int, int, int>) == List<int>(123));

        List<PI> ys;
        REQUIRE_NOTHROW(ys.scan(PI(new int(456)), throwOp<PI, PIRef, PIRef>));
        REQUIRE(ys.scan(PI(new int(456)), throwOp<PI, PIRef, PIRef>).map(toInt) ==
                List<int>(456));
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.scan(321, [](int x, int y) { return x - y; }) ==
                List<int>(321, 321 - 123));

        List<PI> ys(PI(new int(456)));
        REQUIRE(ys.scan(PI(new int(654)), [](PIRef x, PIRef y) {
            return PI(new int(*x - *y));
        }).map(toInt) == List<int>(654, 654 - 456));

        List<Derived> zs(Derived(789));
        REQUIRE(zs.scan(Base(987), [](const Base &x, const Base &y) {
            return Base(x.val - y.val);
        }).map([](const Base &x) {
            return x.val;
        }) == List<int>(987, 987 - 789));
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3, 4, 5);
        REQUIRE(xs.scan(15, [](int x, int y) { return x - y; }) ==
                List<int>(15, 14, 12, 9, 5, 0));

        List<PI> ys(
            PI(new int(5)),
            PI(new int(4)),
            PI(new int(3)),
            PI(new int(2)),
            PI(new int(1))
        );
        REQUIRE(ys.scan(PI(new int(15)), [](PIRef x, PIRef y) {
            return PI(new int(*x - *y));
        }).map(toInt) == List<int>(15, 10, 6, 3, 1, 0));

        List<Derived> zs(
            Derived(1),
            Derived(2),
            Derived(3),
            Derived(4),
            Derived(5)
        );
        REQUIRE(zs.scan(Base(-123), [](const Base &x, const Base &y) {
            return Base(x.val * y.val);
        }).map([](const Base &x) {
            return x.val;
        }) == List<int>(
            -123,
            -123 * 1,
            -123 * 2,
            -123 * 2 * 3,
            -123 * 2 * 3 * 4,
            -123 * 2 * 3 * 4 * 5
        ));
    }
}
