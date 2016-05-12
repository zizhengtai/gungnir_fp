#include <memory>
#include <string>

#include "catch.hpp"

#include "gungnir/Option.hpp"
using gungnir::UnownedOption;

TEST_CASE("test UnownedOption foreach", "[UnownedOption][foreach]") {

    using PI = std::unique_ptr<int>;
    using S = std::string;

    SECTION("empty UnownedOption") {
        unsigned int count = 0;

        UnownedOption<int> x;
        x.foreach([&count](int) { ++count; });
        REQUIRE(count == 0);

        UnownedOption<PI> y;
        y.foreach([&count](const PI &) { ++count; });
        REQUIRE(count == 0);

        UnownedOption<S> z;
        z.foreach([&count](const S &) { ++count; });
        REQUIRE(count == 0);
    }
    SECTION("non-empty UnownedOption") {
        unsigned int count = 0;

        int i = 123;
        UnownedOption<int> x(&i);
        x.foreach([&count](int n) { count += n; });
        REQUIRE(count == 123);

        PI p(new int(456));
        UnownedOption<PI> y(&p);
        y.foreach([&count](const PI &p) { count += *p; });
        REQUIRE(count == 123 + 456);

        S s = "hello";
        UnownedOption<S> z(&s);
        z.foreach([&count](const S &s) { count += s.size(); });
        REQUIRE(count == 123 + 456 + 5);
    }
}
