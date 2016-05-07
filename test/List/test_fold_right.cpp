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

TEST_CASE("test List foldRight", "[List][foldRight]") {

    using PI = std::unique_ptr<int>;
    using PIRef = const PI &;

    struct NoCopy {
        NoCopy(std::string s) : val(std::move(s)) {}
        NoCopy(const NoCopy &) = delete;
        NoCopy(NoCopy &&) = default;
        NoCopy & operator=(const NoCopy &) = delete;
        NoCopy & operator=(NoCopy &&) = default;

        std::string val;
    };

    SECTION("empty List") {
        List<int> xs;
        REQUIRE_NOTHROW(xs.foldRight(123, throwOp<int, int, int>));
        REQUIRE(xs.foldRight(123, throwOp<int, int, int>) == 123);

        List<PI> ys;
        REQUIRE_NOTHROW(ys.foldRight(PI(new int(456)), throwOp<PI, PIRef, PI>));
        REQUIRE(*ys.foldRight(PI(new int(456)), throwOp<PI, PIRef, PI>) == 456);
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.foldRight(321, [](int x, int y) { return x - y; }) == 123 - 321);
        REQUIRE(xs.foldRight(NoCopy("!"), [](int x, NoCopy y) {
            y.val = std::to_string(x) + std::move(y.val);
            return y;
        }).val == "123!");

        List<PI> ys(PI(new int(456)));
        REQUIRE(*ys.foldRight(PI(new int(654)), [](PIRef x, PI y) {
            return PI(new int(*x - *y));
        }) == 456 - 654);
        REQUIRE(ys.foldRight(654, [](PIRef x, int y) {
            return *x - y;
        }) == 456 - 654);
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3, 4, 5);
        REQUIRE(xs.foldRight(List<int>(6), [](int x, List<int> y) {
            return List<int>(x, std::move(y));
        }) == List<int>(1, 2, 3, 4, 5, 6));

        List<PI> ys(
            PI(new int(5)),
            PI(new int(4)),
            PI(new int(3)),
            PI(new int(2)),
            PI(new int(1))
        );
        REQUIRE(ys.foldRight(std::string("0"), [](PIRef x, std::string y) {
            return std::to_string(*x) + std::move(y);
        }) == "543210");
    }
}
