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

TEST_CASE("test List fold", "[List][fold]") {

    using PI = std::unique_ptr<int>;
    using PIRef = const PI&;

    struct Base {
        Base(int x) noexcept: val(x) {}
        int val;
    };
    struct Derived : Base {
        Derived(int x) noexcept : Base(x) {}
    };

    SECTION("empty List") {
        List<int> xs;
        REQUIRE_NOTHROW(xs.fold(123, throwOp<int, int, int>));
        REQUIRE(xs.fold(123, throwOp<int, int, int>) == 123);

        List<PI> ys;
        REQUIRE_NOTHROW(ys.fold(PI(new int(456)), throwOp<PI, PIRef, PIRef>));
        REQUIRE(*ys.fold(PI(new int(456)), throwOp<PI, PIRef, PIRef>) == 456);
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.fold(0, [](int x, int y) { return x + y; }) == 123);

        List<PI> ys(PI(new int(456)));
        REQUIRE(*ys.fold(PI(new int(0)), [](PIRef x, PIRef y) {
            return PI(new int(*x + *y));
        }) == 456);

        List<Derived> zs(Derived(789));
        REQUIRE(zs.fold(Base(0), [](const Base& x, const Base& y) {
            return Base(x.val + y.val);
        }).val == 789);
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3, 4, 5);
        REQUIRE(xs.fold(0, [](int x, int y) {
            return x + y;
        }) == (1 + 5) * 5 / 2);

        List<PI> ys(
            PI(new int(5)),
            PI(new int(4)),
            PI(new int(3)),
            PI(new int(2)),
            PI(new int(1))
        );
        REQUIRE(*ys.fold(PI(new int(0)), [](PIRef x, PIRef y) {
            return PI(new int(*x + *y));
        }) == (5 + 1) * 5 / 2);

        List<Derived> zs(
            Derived(1),
            Derived(2),
            Derived(3),
            Derived(4),
            Derived(5)
        );
        REQUIRE(zs.fold(Base(1), [](const Base& x, const Base& y) {
            return Base(x.val * y.val);
        }).val == 1 * 2 * 3 * 4 * 5);
    }
}
