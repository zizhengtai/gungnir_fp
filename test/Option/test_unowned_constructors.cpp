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
        REQUIRE(y1.ptr() == nullptr);
        REQUIRE((y1 == nullptr));

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
        REQUIRE(z1.ptr() == nullptr);
        REQUIRE((z1 == nullptr));

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

        PI y(new int(456));

        UnownedOption<PI> y1(&y);
        REQUIRE_FALSE(y1.isEmpty());
        REQUIRE(y1.ptr() == &y);
        REQUIRE((y1 == &y));
        REQUIRE(*y1.ptr() == y);
        REQUIRE(*y1 == y);
        REQUIRE(y1.get() == y);
        REQUIRE(**y1.ptr() == 456);
        REQUIRE(**y1 == 456);
        REQUIRE(*(*y1).get() == 456);
        REQUIRE(*y1->get() == 456);

        // copy constructor
        UnownedOption<PI> y2(y1);
        REQUIRE_FALSE(y1.isEmpty());
        REQUIRE_FALSE(y2.isEmpty());
        REQUIRE(y1.ptr() == &y);
        REQUIRE(y2.ptr() == &y);
        REQUIRE((y1 == &y));
        REQUIRE((y2 == &y));
        REQUIRE(*y1.ptr() == y);
        REQUIRE(*y2.ptr() == y);
        REQUIRE(*y1 == y);
        REQUIRE(*y2 == y);
        REQUIRE(y1.get() == y);
        REQUIRE(y2.get() == y);
        REQUIRE(**y1.ptr() == 456);
        REQUIRE(**y2.ptr() == 456);
        REQUIRE(**y1 == 456);
        REQUIRE(**y2 == 456);
        REQUIRE(*(*y1).get() == 456);
        REQUIRE(*(*y2).get() == 456);
        REQUIRE(*y1->get() == 456);
        REQUIRE(*y2->get() == 456);

        // move constructor
        UnownedOption<PI> y3(std::move(y1));
        REQUIRE(y1.isEmpty());
        REQUIRE(y1.ptr() == nullptr);
        REQUIRE((y1 == nullptr));
        REQUIRE_FALSE(y3.isEmpty());
        REQUIRE(y3.ptr() == &y);
        REQUIRE((y3 == &y));
        REQUIRE(*y3.ptr() == y);
        REQUIRE(*y3 == y);
        REQUIRE(y3.get() == y);
        REQUIRE(**y3.ptr() == 456);
        REQUIRE(**y3 == 456);
        REQUIRE(*(*y3).get() == 456);
        REQUIRE(*y3->get() == 456);

        S z("hello");

        UnownedOption<S> z1(&z);
        REQUIRE_FALSE(z1.isEmpty());
        REQUIRE(z1.ptr() == &z);
        REQUIRE((z1 == &z));
        REQUIRE(*z1.ptr() == "hello");
        REQUIRE(*z1 == "hello");
        REQUIRE(z1.get() == "hello");
        REQUIRE(z1->size() == 5);

        // copy constructor
        UnownedOption<S> z2(z1);
        REQUIRE_FALSE(z1.isEmpty());
        REQUIRE_FALSE(z2.isEmpty());
        REQUIRE(z1.ptr() == &z);
        REQUIRE(z2.ptr() == &z);
        REQUIRE((z1 == &z));
        REQUIRE((z2 == &z));
        REQUIRE(*z1.ptr() == "hello");
        REQUIRE(*z2.ptr() == "hello");
        REQUIRE(*z1 == "hello");
        REQUIRE(*z2 == "hello");
        REQUIRE(z1.get() == "hello");
        REQUIRE(z2.get() == "hello");
        REQUIRE(z1->size() == 5);
        REQUIRE(z2->size() == 5);

        // move constructor
        UnownedOption<S> z3(std::move(z1));
        REQUIRE(z1.isEmpty());
        REQUIRE(z1.ptr() == nullptr);
        REQUIRE((z1 == nullptr));
        REQUIRE_FALSE(z3.isEmpty());
        REQUIRE(z3.ptr() == &z);
        REQUIRE((z3 == &z));
        REQUIRE(*z3.ptr() == "hello");
        REQUIRE(*z3 == "hello");
        REQUIRE(z3.get() == "hello");
        REQUIRE(z3->size() == 5);
    }
}
