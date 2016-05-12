#include <memory>
#include <string>

#include "catch.hpp"

#include "gungnir/Option.hpp"
using gungnir::UnownedOption;

TEST_CASE("test UnownedOption map", "[UnownedOption][map]") {

    using PI = std::unique_ptr<int>;
    using S = std::string;

    SECTION("empty UnownedOption") {
        UnownedOption<int> x;
        REQUIRE(x.map([](int) { return 123; }).isEmpty());

        UnownedOption<PI> y;
        REQUIRE(y.map([](const PI &) { return PI(new int(456)); }).isEmpty());

        UnownedOption<S> z;
        REQUIRE(z.map([](const S &) { return S("hello"); }).isEmpty());
    }
    SECTION("non-empty UnownedOption") {
        int x = 123;
        UnownedOption<int> x1(&x);
        const auto x2 = x1.map([](int x) { return std::to_string(-x); });
        REQUIRE_FALSE(x2.isEmpty());
        REQUIRE(x2.get() == "-123");
        REQUIRE(x == 123);

        PI y(new int(456));
        UnownedOption<PI> y1(&y);
        const auto y2 = y1.map([](const PI &p) {
            return std::unique_ptr<double>(new double(*p * 1.1));
        });
        REQUIRE_FALSE(y2.isEmpty());
        REQUIRE(y2.get() != nullptr);
        REQUIRE(*y2.get() == Approx(456 * 1.1));
        REQUIRE(y != nullptr);
        REQUIRE(*y == 456);

        S z = "hello";
        UnownedOption<S> z1(&z);
        const auto z2 = z1.map([](const S &s) { return (s + " world").size(); });
        REQUIRE_FALSE(z2.isEmpty());
        REQUIRE(z2.get() == std::string("hello world").size());
        REQUIRE(z == "hello");
    }
}
