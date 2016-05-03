#include <memory>

#include "catch.hpp"

#include "gungnir/lazy.hpp"
using gungnir::lazyEval;

static std::size_t count = 0;

static int foo0(std::unique_ptr<int> up)
{
    ++count;
    return *up;
}

static int foo1(std::shared_ptr<int> sp)
{
    ++count;
    return *sp * 2;
}

static int foo2(const std::shared_ptr<int> &sp)
{
    ++count;
    return *sp * 3;
}

static int foo3(std::unique_ptr<int> up, const std::shared_ptr<int> &sp)
{
    ++count;
    return *up + *sp;
}

TEST_CASE("test lazyEval", "[lazyEval]") {

    using UP = std::unique_ptr<int>;

    SECTION("function taking by-value arguments") {
        count = 0;

        auto f1 = lazyEval(foo0, UP(new int(123)));
        REQUIRE(count == 0);

        REQUIRE(f1() == 123);
        REQUIRE(count == 1);

        UP up(new int(456));
        auto f2 = lazyEval(foo0, std::move(up));
        REQUIRE(count == 1);

        REQUIRE(f2() == 456);
        REQUIRE(count == 2);

        const auto sp = std::make_shared<int>(123);
        auto f3 = lazyEval(foo1, sp);
        REQUIRE(sp.use_count() == 1);
        REQUIRE(count == 2);

        REQUIRE(f3() == 123 * 2);
        REQUIRE(sp.use_count() == 1);
        REQUIRE(count == 3);

        REQUIRE(f3() == 123 * 2);
        REQUIRE(sp.use_count() == 1);
        REQUIRE(count == 4);

        auto f4 = lazyEval(foo1, std::make_shared<int>(456));
        REQUIRE(count == 4);

        REQUIRE(f4() == 456 * 2);
        REQUIRE(count == 5);
    }
    SECTION("function taking by-reference arguments") {
        count = 0;

        const auto sp = std::make_shared<int>(123);
        auto f1 = lazyEval(foo2, sp);
        REQUIRE(sp.use_count() == 1);
        REQUIRE(count == 0);

        REQUIRE(f1() == 123 * 3);
        REQUIRE(sp.use_count() == 1);
        REQUIRE(count == 1);

        auto f2 = lazyEval(foo2, std::make_shared<int>(456));
        REQUIRE(count == 1);

        REQUIRE(f2() == 456 * 3);
        REQUIRE(count == 2);
    }
    SECTION("function taking both by-value and by-reference arguments") {
        count = 0;

        auto f1 = lazyEval(foo3, UP(new int(123)), std::make_shared<int>(456));
        REQUIRE(count == 0);

        REQUIRE(f1() == 123 + 456);
        REQUIRE(count == 1);

        auto up = UP(new int(654));
        const auto sp = std::make_shared<int>(321);
        auto f2 = lazyEval(foo3, std::move(up), sp);
        REQUIRE(sp.use_count() == 1);
        REQUIRE(count == 1);

        REQUIRE(f2() == 654 + 321);
        REQUIRE(sp.use_count() == 1);
        REQUIRE(count == 2);
    }
}
