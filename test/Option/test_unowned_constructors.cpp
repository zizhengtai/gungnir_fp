#include <memory>
#include <string>

#include "catch.hpp"

#include "gungnir/Option.hpp"
using gungnir::UnownedOption;

TEST_CASE("test UnownedOption constructors", "[UnownedOption][constructors]") {

    using PI = std::unique_ptr<int>;
    using S = std::string;

    SECTION("empty UnownedOption") {
        UnownedOption<int> x1;
        REQUIRE(x1.isEmpty());
        REQUIRE(x1.ptr() == nullptr);
        REQUIRE((x1 == nullptr));

        // copy constructor
        UnownedOption<int> x2(x1);
        REQUIRE(x1.isEmpty());
        REQUIRE(x2.isEmpty());
        REQUIRE(x1.ptr() == nullptr);
        REQUIRE(x2.ptr() == nullptr);
        REQUIRE((x1 == nullptr));
        REQUIRE((x2 == nullptr));

        // move constructor
        UnownedOption<int> x3(std::move(x1));
        REQUIRE(x1.isEmpty());
        REQUIRE(x3.isEmpty());
        REQUIRE(x1.ptr() == nullptr);
        REQUIRE(x3.ptr() == nullptr);
        REQUIRE((x1 == nullptr));
        REQUIRE((x3 == nullptr));

        UnownedOption<PI> y1;
        REQUIRE(y1.isEmpty());

        // copy constructor
        UnownedOption<PI> y2(y1);
        REQUIRE(x1.isEmpty());
        REQUIRE(x2.isEmpty());
        REQUIRE(x1.ptr() == nullptr);
        REQUIRE(x2.ptr() == nullptr);
        REQUIRE((x1 == nullptr));
        REQUIRE((x2 == nullptr));

        // move constructor
        UnownedOption<PI> y3(std::move(y1));
        REQUIRE(y1.isEmpty());
        REQUIRE(y3.isEmpty());
        REQUIRE(y1.ptr() == nullptr);
        REQUIRE(y3.ptr() == nullptr);
        REQUIRE((y1 == nullptr));
        REQUIRE((y3 == nullptr));

        UnownedOption<S> z1;
        REQUIRE(z1.isEmpty());

        // copy constructor
        UnownedOption<S> z2(z1);
        REQUIRE(z1.isEmpty());
        REQUIRE(z2.isEmpty());
        REQUIRE(z1.ptr() == nullptr);
        REQUIRE(z2.ptr() == nullptr);
        REQUIRE((z1 == nullptr));
        REQUIRE((z2 == nullptr));

        // move constructor
        UnownedOption<S> z3(std::move(z1));
        REQUIRE(z1.isEmpty());
        REQUIRE(z3.isEmpty());
        REQUIRE(z1.ptr() == nullptr);
        REQUIRE(z3.ptr() == nullptr);
        REQUIRE((z1 == nullptr));
        REQUIRE((z3 == nullptr));
    }
}
