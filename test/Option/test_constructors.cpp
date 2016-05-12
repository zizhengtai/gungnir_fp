#include <memory>
#include <string>

#include "catch.hpp"

#include "gungnir/Option.hpp"
using gungnir::Option;

TEST_CASE("test Option constructors", "[Option][constructors]") {

    using PI = std::unique_ptr<int>;
    using S = std::string;

    SECTION("empty Option") {
        Option<int> x1;
        REQUIRE(x1.isEmpty());
        REQUIRE(x1.ptr() == nullptr);
        REQUIRE((x1 == nullptr));

        // move constructor
        Option<int> x2(std::move(x1));
        REQUIRE(x1.isEmpty());
        REQUIRE(x2.isEmpty());
        REQUIRE(x1.ptr() == nullptr);
        REQUIRE(x2.ptr() == nullptr);
        REQUIRE((x1 == nullptr));
        REQUIRE((x2 == nullptr));

        Option<PI> y1;
        REQUIRE(y1.isEmpty());

        // move constructor
        Option<PI> y2(std::move(y1));
        REQUIRE(y1.isEmpty());
        REQUIRE(y2.isEmpty());
        REQUIRE(y1.ptr() == nullptr);
        REQUIRE(y2.ptr() == nullptr);
        REQUIRE((y1 == nullptr));
        REQUIRE((y2 == nullptr));

        Option<S> z1;
        REQUIRE(z1.isEmpty());

        // move constructor
        Option<S> z2(std::move(z1));
        REQUIRE(z1.isEmpty());
        REQUIRE(z2.isEmpty());
        REQUIRE(z1.ptr() == nullptr);
        REQUIRE(z2.ptr() == nullptr);
        REQUIRE((z1 == nullptr));
        REQUIRE((z2 == nullptr));
    }
    SECTION("non-empty Option") {
        Option<int> x1(123);
        REQUIRE_FALSE(x1.isEmpty());
        REQUIRE(x1.ptr() != nullptr);
        REQUIRE((x1 != nullptr));
        REQUIRE(*x1.ptr() == 123);
        REQUIRE(*x1 == 123);
        REQUIRE(x1.get() == 123);

        // move constructor
        Option<int> x2(std::move(x1));
        REQUIRE(x1.isEmpty());
        REQUIRE(x1.ptr() == nullptr);
        REQUIRE((x1 == nullptr));
        REQUIRE_FALSE(x2.isEmpty());
        REQUIRE(x2.ptr() != nullptr);
        REQUIRE((x2 != nullptr));
        REQUIRE(*x2.ptr() == 123);
        REQUIRE(*x2 == 123);
        REQUIRE(x2.get() == 123);

        Option<PI> y1(PI{});
        REQUIRE_FALSE(y1.isEmpty());
        REQUIRE(y1.ptr() != nullptr);
        REQUIRE((y1 != nullptr));
        REQUIRE(*y1.ptr() == nullptr);
        REQUIRE(*y1 == nullptr);
        REQUIRE((*y1).get() == nullptr);
        REQUIRE(y1->get() == nullptr);

        // move constructor
        Option<PI> y2(std::move(y1));
        REQUIRE(y1.isEmpty());
        REQUIRE(y1.ptr() == nullptr);
        REQUIRE((y1 == nullptr));
        REQUIRE_FALSE(y2.isEmpty());
        REQUIRE(y2.ptr() != nullptr);
        REQUIRE((y2 != nullptr));
        REQUIRE(*y2.ptr() == nullptr);
        REQUIRE(*y2 == nullptr);
        REQUIRE((*y2).get() == nullptr);
        REQUIRE(y2->get() == nullptr);

        Option<PI> y3(new int(456));
        REQUIRE_FALSE(y3.isEmpty());
        REQUIRE(y3.ptr() != nullptr);
        REQUIRE((y3 != nullptr));
        REQUIRE(*y3.ptr() != nullptr);
        REQUIRE(*y3 != nullptr);
        REQUIRE((*y3).get() != nullptr);
        REQUIRE(y3->get() != nullptr);
        REQUIRE(**y3 == 456);
        REQUIRE(*y3.get() == 456);

        // move constructor
        Option<PI> y4(std::move(y3));
        REQUIRE(y3.isEmpty());
        REQUIRE(y3.ptr() == nullptr);
        REQUIRE((y3 == nullptr));
        REQUIRE_FALSE(y4.isEmpty());
        REQUIRE(y4.ptr() != nullptr);
        REQUIRE((y4 != nullptr));
        REQUIRE(*y4.ptr() != nullptr);
        REQUIRE(*y4 != nullptr);
        REQUIRE((*y4).get() != nullptr);
        REQUIRE(y4->get() != nullptr);
        REQUIRE(**y4 == 456);
        REQUIRE(*y4.get() == 456);

        Option<S> z1("hello");
        REQUIRE_FALSE(z1.isEmpty());
        REQUIRE(z1.ptr() != nullptr);
        REQUIRE((z1 != nullptr));
        REQUIRE(*z1.ptr() == "hello");
        REQUIRE(*z1 == "hello");
        REQUIRE(z1.get() == "hello");

        // move constructor
        Option<S> z2(std::move(z1));
        REQUIRE(z1.isEmpty());
        REQUIRE(z1.ptr() == nullptr);
        REQUIRE((z1 == nullptr));
        REQUIRE_FALSE(z2.isEmpty());
        REQUIRE(z2.ptr() != nullptr);
        REQUIRE((z2 != nullptr));
        REQUIRE(*z2.ptr() == "hello");
        REQUIRE(*z2 == "hello");
        REQUIRE(z2.get() == "hello");
    }
}
