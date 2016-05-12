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
        Option<int> x2(std::move(x1));
        REQUIRE(x1.isEmpty());
        REQUIRE(x2.isEmpty());
        REQUIRE(x1.ptr() == nullptr);
        REQUIRE(x2.ptr() == nullptr);

        Option<PI> y1;
        REQUIRE(y1.isEmpty());
        Option<PI> y2(std::move(y1));
        REQUIRE(y1.isEmpty());
        REQUIRE(y2.isEmpty());
        REQUIRE(y1.ptr() == nullptr);
        REQUIRE(y2.ptr() == nullptr);

        Option<S> z1;
        REQUIRE(z1.isEmpty());
        Option<S> z2(std::move(z1));
        REQUIRE(z1.isEmpty());
        REQUIRE(z2.isEmpty());
        REQUIRE(z1.ptr() == nullptr);
        REQUIRE(z2.ptr() == nullptr);
    }
    SECTION("non-empty Option") {
        Option<int> x1(123);
        REQUIRE_FALSE(x1.isEmpty());
        REQUIRE(x1.ptr() != nullptr);
        REQUIRE(*x1.ptr() == 123);
        REQUIRE(x1.get() == 123);
        Option<int> x2(std::move(x1));
        REQUIRE(x1.isEmpty());
        REQUIRE(x1.ptr() == nullptr);
        REQUIRE_FALSE(x2.isEmpty());
        REQUIRE(x2.ptr() != nullptr);
        REQUIRE(*x2.ptr() == 123);
        REQUIRE(x2.get() == 123);

        Option<PI> y1(PI{}), y2(new int(456));
        REQUIRE_FALSE(y1.isEmpty());
        REQUIRE_FALSE(y2.isEmpty());
        REQUIRE(y1.ptr() != nullptr);
        REQUIRE(y2.ptr() != nullptr);
        REQUIRE(*y1.ptr() == nullptr);
        REQUIRE(*y2.ptr() != nullptr);
        REQUIRE(y1.get() == nullptr);
        REQUIRE(y2.get() != nullptr);
        REQUIRE(*y2.get() == 456);
        Option<PI> y3(std::move(y1)), y4(std::move(y2));
        REQUIRE(y1.isEmpty());
        REQUIRE(y2.isEmpty());
        REQUIRE(y1.ptr() == nullptr);
        REQUIRE(y2.ptr() == nullptr);
        REQUIRE_FALSE(y3.isEmpty());
        REQUIRE_FALSE(y4.isEmpty());
        REQUIRE(y3.ptr() != nullptr);
        REQUIRE(y4.ptr() != nullptr);
        REQUIRE(*y3.ptr() == nullptr);
        REQUIRE(*y4.ptr() != nullptr);
        REQUIRE(y3.get() == nullptr);
        REQUIRE(y4.get() != nullptr);
        REQUIRE(*y4.get() == 456);

        Option<S> z1("hello");
        REQUIRE_FALSE(z1.isEmpty());
        REQUIRE(z1.ptr() != nullptr);
        REQUIRE(*z1.ptr() == "hello");
        REQUIRE(z1.get() == "hello");
        Option<S> z2(std::move(z1));
        REQUIRE(z1.isEmpty());
        REQUIRE(z1.ptr() == nullptr);
        REQUIRE_FALSE(z2.isEmpty());
        REQUIRE(z2.ptr() != nullptr);
        REQUIRE(*z2.ptr() == "hello");
        REQUIRE(z2.get() == "hello");
    }
}
