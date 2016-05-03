#include <memory>

#include "catch.hpp"

#include "gungnir/lazy.hpp"
using gungnir::lazyVal;

TEST_CASE("test lazyVal", "[lazyVal]") {

    using UP = std::unique_ptr<int>;
    using SP = std::shared_ptr<int>;

    static std::size_t count = 0;

    class Foo {
    public:
        Foo() noexcept { ++count; }

        Foo(UP up) noexcept : up(std::move(up)) { ++count; }

        Foo(const SP &sp) noexcept : sp(sp) { ++count; }

        Foo(UP up, const SP &sp) noexcept : up(std::move(up)), sp(sp) { ++count; }

        Foo(Foo &&) = default;

        UP up;
        SP sp;
    };

    SECTION("constructor with no arguments") {
        count = 0;

        const auto foo = lazyVal<Foo>();
        REQUIRE(count == 0);

        foo();
        REQUIRE(count == 1);

        foo();
        REQUIRE(count == 1);

        const auto p1 = lazyVal<UP>();
        REQUIRE(p1() == nullptr);

        const auto p2 = lazyVal<UP>(new int(123));
        REQUIRE(*p2() == 123);
    }
    SECTION("constructor with by-value arguments") {
        count = 0;

        const auto foo = lazyVal<Foo>(UP(new int(123)));
        REQUIRE(count == 0);

        REQUIRE(*foo().up == 123);
        REQUIRE(count == 1);

        foo();
        REQUIRE(count == 1);
    }
    SECTION("constructor with by-reference arguments") {
        count = 0;

        const auto foo = lazyVal<Foo>(SP(new int(123)));
        REQUIRE(count == 0);

        REQUIRE(*foo().sp == 123);
        REQUIRE(count == 1);

        REQUIRE(foo().sp.use_count() == 1);
        REQUIRE(count == 1);

        const auto p = std::make_shared<int>(456);
        const auto bar = lazyVal<Foo>(p);
        REQUIRE(count == 1);

        REQUIRE(*bar().sp == 456);
        REQUIRE(count == 2);

        REQUIRE(bar().sp.use_count() == 2);
        REQUIRE(count == 2);
    }
    SECTION("constructor with both by-value and by-reference arguments") {
        count = 0;

        const auto foo = lazyVal<Foo>(UP(new int(123)), SP(new int(456)));
        REQUIRE(count == 0);

        REQUIRE(*foo().up == 123);
        REQUIRE(count == 1);

        REQUIRE(*foo().sp == 456);
        REQUIRE(count == 1);

        REQUIRE(foo().sp.use_count() == 1);
        REQUIRE(count == 1);

        UP up(new int(654));
        SP sp(new int(321));
        const auto bar = lazyVal<Foo>(std::move(up), sp);
        REQUIRE(count == 1);

        REQUIRE(*bar().up == 654);
        REQUIRE(count == 2);

        REQUIRE(*bar().sp == 321);
        REQUIRE(count == 2);

        REQUIRE(bar().sp.use_count() == 2);
        REQUIRE(count == 2);
    }
    SECTION("mutable values") {
        count = 0;

        auto foo = lazyVal<Foo>(UP(new int(123)), SP(new int(456)));
        REQUIRE(count == 0);

        auto bar = std::move(foo);
        REQUIRE(count == 0);

        REQUIRE(*bar().up == 123);
        REQUIRE(count == 1);

        REQUIRE(*bar().sp == 456);
        REQUIRE(count == 1);

        auto poi = std::move(bar());
        REQUIRE(count == 1);

        REQUIRE(bar().up == nullptr);
        REQUIRE(count == 1);

        REQUIRE(bar().sp == nullptr);
        REQUIRE(count == 1);

        REQUIRE(*poi.up == 123);
        REQUIRE(*poi.sp == 456);
    }
}
