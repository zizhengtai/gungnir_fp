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
    SECTION("non-empty UnownedOption") {
        int x = 123;

        UnownedOption<int> x1(&x);
        REQUIRE_FALSE(x1.isEmpty());
        REQUIRE(x1.ptr() == &x);
        REQUIRE((x1 == &x));
        REQUIRE(*x1.ptr() == 123);
        REQUIRE(*x1 == 123);
        REQUIRE(x1.get() == 123);

        // copy constructor
        UnownedOption<int> x2(x1);
        REQUIRE_FALSE(x1.isEmpty());
        REQUIRE_FALSE(x2.isEmpty());
        REQUIRE(x1.ptr() == &x);
        REQUIRE(x2.ptr() == &x);
        REQUIRE((x1 == &x));
        REQUIRE((x2 == &x));
        REQUIRE(*x1.ptr() == 123);
        REQUIRE(*x2.ptr() == 123);
        REQUIRE(*x1 == 123);
        REQUIRE(*x2 == 123);
        REQUIRE(x1.get() == 123);
        REQUIRE(x2.get() == 123);

        // move constructor
        UnownedOption<int> x3(std::move(x1));
        REQUIRE(x1.isEmpty());
        REQUIRE(x1.ptr() == nullptr);
        REQUIRE((x1 == nullptr));
        REQUIRE_FALSE(x3.isEmpty());
        REQUIRE(x3.ptr() == &x);
        REQUIRE((x3 == &x));
        REQUIRE(*x3.ptr() == 123);
        REQUIRE(*x3 == 123);
        REQUIRE(x3.get() == 123);
    }
}
