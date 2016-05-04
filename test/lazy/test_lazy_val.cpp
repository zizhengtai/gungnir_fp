#include <memory>

#include "catch.hpp"

#include "gungnir/lazy.hpp"
using gungnir::lazyVal;

TEST_CASE("test lazyVal", "[lazyVal]") {

    using UP = std::unique_ptr<int>;
    using SP = std::shared_ptr<int>;

    static std::size_t count = 0;

    struct Foo {
        Foo() noexcept { ++count; }
        Foo(UP up) noexcept : up(std::move(up)) { ++count; }
        Foo(const SP &sp) noexcept : sp(std::move(sp)) { ++count; }
        Foo(UP up, const SP &sp) noexcept : up(std::move(up)), sp(std::move(sp)) { ++count; }
        Foo(Foo &&) = default;

        UP up;
        SP sp;
    };

    SECTION("constructor with no arguments") {
        const auto p = lazyVal<UP>();
        REQUIRE(p() == nullptr);

        count = 0;

        const auto foo = lazyVal<Foo>();
        REQUIRE(count == 0);

        foo();
        REQUIRE(count == 1);

        foo();
        REQUIRE(count == 1);
    }
    SECTION("constructor with non-copyable arguments") {
        const auto p = lazyVal<UP>(new int(123));
        REQUIRE(*p() == 123);

        count = 0;

        const auto foo = lazyVal<Foo>(UP(new int(123)));
        REQUIRE(count == 0);

        REQUIRE(*foo().up == 123);
        REQUIRE(count == 1);

        foo();
        REQUIRE(count == 1);
    }
    SECTION("constructor with copyable arguments") {
        count = 0;

        const auto foo = lazyVal<Foo>(SP(new int(123)));
        REQUIRE(count == 0);

        REQUIRE(*foo().sp == 123);
        REQUIRE(count == 1);

        REQUIRE(foo().sp.use_count() == 1);
        REQUIRE(count == 1);

        const auto p = std::make_shared<int>(456);
        const auto bar = lazyVal<Foo>(p);
        REQUIRE(p.use_count() == 2);
        REQUIRE(count == 1);

        REQUIRE(*bar().sp == 456);
        REQUIRE(count == 2);

        REQUIRE(bar().sp.use_count() == 2);
        REQUIRE(count == 2);

        const auto poi = lazyVal<Foo>(std::cref(p));
        REQUIRE(p.use_count() == 2);
        REQUIRE(count == 2);

        REQUIRE(*poi().sp == 456);
        REQUIRE(count == 3);

        REQUIRE(poi().sp.use_count() == 3);
        REQUIRE(count == 3);
    }
    SECTION("constructor with both copyable and non-copyable arguments") {
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
        REQUIRE(sp.use_count() == 2);
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
        REQUIRE(bar().sp.use_count() == 1);
        REQUIRE(count == 1);

        auto poi = std::move(bar());
        REQUIRE(count == 1);

        REQUIRE(bar().up == nullptr);
        REQUIRE(count == 1);

        REQUIRE(bar().sp == nullptr);
        REQUIRE(count == 1);

        REQUIRE(*poi.up == 123);
        REQUIRE(*poi.sp == 456);
        REQUIRE(poi.sp.use_count() == 1);
    }
}
