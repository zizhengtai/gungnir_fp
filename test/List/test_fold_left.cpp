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

TEST_CASE("test List foldLeft", "[List][foldLeft]") {

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
        REQUIRE_NOTHROW(xs.foldLeft(123, throwOp<int, int, int>));
        REQUIRE(xs.foldLeft(123, throwOp<int, int, int>));

        List<PI> ys;
        REQUIRE_NOTHROW(ys.foldLeft(PI(new int(456)), throwOp<PI, PI, PIRef>));
        REQUIRE(*ys.foldLeft(PI(new int(456)), throwOp<PI, PI, PIRef>) == 456);
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.foldLeft(321, [](int x, int y) { return x - y; }) == 321 - 123);
        REQUIRE(xs.foldLeft(NoCopy("!"), [](NoCopy x, int y) {
            x.val += std::to_string(y);
            return x;
        }).val == "!123");

        List<PI> ys(PI(new int(456)));
        REQUIRE(*ys.foldLeft(PI(new int(654)), [](PI x, PIRef y) {
            return PI(new int(*x - *y));
        }) == 654 - 456);
        REQUIRE(ys.foldLeft(654, [](int x, PIRef y) {
            return x - *y;
        }) == 654 - 456);
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3, 4, 5);
        REQUIRE(xs.foldLeft(List<int>(0), [](List<int> x, int y) {
            return List<int>(y, std::move(x));
        }) == List<int>(5, 4, 3, 2, 1, 0));

        List<PI> ys(
            PI(new int(5)),
            PI(new int(4)),
            PI(new int(3)),
            PI(new int(2)),
            PI(new int(1))
        );
        REQUIRE(ys.foldLeft(std::string("6"), [](std::string x, PIRef y) {
            return x + std::to_string(*y);
        }) == "654321");
    }
}
