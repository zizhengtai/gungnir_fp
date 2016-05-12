#include <memory>
#include <string>

#include "catch.hpp"

#include "gungnir/Option.hpp"
using gungnir::Option;

TEST_CASE("test Option foreach", "[Option][foreach]") {

    using PI = std::unique_ptr<int>;
    using S = std::string;

    SECTION("empty Option") {
        unsigned int count = 0;

        Option<int> x;
        x.foreach([&count](int) { ++count; });
        REQUIRE(count == 0);

        Option<PI> y;
        y.foreach([&count](const PI &) { ++count; });
        REQUIRE(count == 0);

        Option<S> z;
        z.foreach([&count](const S &) { ++count; });
        REQUIRE(count == 0);
    }
    SECTION("non-empty Option") {
        unsigned int count = 0;

        Option<int> x(123);
        x.foreach([&count](int n) { count += n; });
        REQUIRE(count == 123);

        Option<PI> y(new int(456));
        y.foreach([&count](const PI &p) { count += *p; });
        REQUIRE(count == 123 + 456);

        Option<S> z("hello");
        z.foreach([&count](const S &s) { count += s.size(); });
        REQUIRE(count == 123 + 456 + 5);
    }
}
