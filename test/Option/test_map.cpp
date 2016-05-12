#include <memory>
#include <string>

#include "catch.hpp"

#include "gungnir/Option.hpp"
using gungnir::Option;

TEST_CASE("test Option map", "[Option][map]") {

    using PI = std::unique_ptr<int>;
    using S = std::string;

    SECTION("empty Option") {
        Option<int> x;
        REQUIRE(x.map([](int) { return 123; }).isEmpty());

        Option<PI> y;
        REQUIRE(y.map([](const PI &) { return PI(new int(456)); }).isEmpty());

        Option<S> z;
        REQUIRE(z.map([](const S &) { return S("hello"); }).isEmpty());
    }
    SECTION("non-empty Option") {
        Option<int> x1(123);
        const auto x2 = x1.map([](int x) { return std::to_string(-x); });
        REQUIRE_FALSE(x2.isEmpty());
        REQUIRE(x2.get() == "-123");

        Option<PI> y1(new int(456));
        const auto y2 = y1.map([](const PI &p) {
            return std::unique_ptr<double>(new double(*p * 1.1));
        });
        REQUIRE_FALSE(y2.isEmpty());
        REQUIRE(y2.get() != nullptr);
        REQUIRE(*y2.get() == Approx(456 * 1.1));

        Option<S> z1("hello");
        const auto z2 = z1.map([](const S &s) { return (s + " world").size(); });
        REQUIRE_FALSE(z2.isEmpty());
        REQUIRE(z2.get() == std::string("hello world").size());
    }
}
