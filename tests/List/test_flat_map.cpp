#include <memory>
#include <string>
#include <utility>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

static List<std::string> stepsI(int x)
{
    return List<std::string>(
        std::to_string(x),
        std::to_string(x + 1),
        std::to_string(x + 2)
    );
}

static List<std::string> stepsPI(const std::unique_ptr<int> &p)
{
    return List<std::string>(
        std::to_string(*p),
        std::to_string(*p + 1),
        std::to_string(*p + 2)
    );
}

template<typename T>
static auto identity(T &&x) -> decltype(std::forward<T>(x))
{
    return std::forward<T>(x);
}

TEST_CASE("test List flatMap", "[List][flatMap]") {

    using LI = List<int>;
    using LLI = List<List<int>>;
    using PI = std::unique_ptr<int>;
    using LPI = List<PI>;
    using LLPI = List<LPI>;
    using LS = List<std::string>;

    SECTION("empty List") {
        REQUIRE(LI().flatMap(stepsI).isEmpty());
        REQUIRE(LLI().flatMap(identity<const LI &>).isEmpty());

        REQUIRE(LPI().flatMap(stepsPI).isEmpty());
        REQUIRE(LLPI().flatMap(identity<const LPI &>).isEmpty());
    }
    SECTION("List with one element") {
        REQUIRE(LI(1).flatMap([](int) { return LS(); }).isEmpty());
        REQUIRE(LI(1).flatMap(stepsI) == LS("1", "2", "3"));
        REQUIRE(LLI(LI(1, 2, 3)).flatMap([](const LI &xs) {
            return List<std::size_t>(xs.size());
        }) == List<std::size_t>(3));
        REQUIRE(LLI(LI(4, 5, 6)).flatMap(identity<const LI &>) == LI(4, 5, 6));

        REQUIRE(LPI(PI()).flatMap([](const PI &) { return LS(); }).isEmpty());
        REQUIRE(LPI(PI(new int(4))).flatMap(stepsPI) == LS("4", "5", "6"));
        REQUIRE(LLPI(LPI(PI(), PI(), PI())).flatMap([](const LPI &ys) {
            return List<std::size_t>(ys.size());
        }) == List<std::size_t>(3));
        LPI ys(PI(new int(4)), PI(new int(5)), PI(new int(6)));
        REQUIRE(LLPI(ys).flatMap(identity<const LPI &>) == ys);
    }
    SECTION("List with multiple elements") {
        REQUIRE(LI(1, 2, 3, 4, 5).flatMap([](int) { return LS(); }).isEmpty());
        REQUIRE(LI(1, 4).flatMap(stepsI) == LS("1", "2", "3", "4", "5", "6"));
        REQUIRE(LI(1, 2, 3, 4, 5).flatMap([](int x) {
            return x % 2 == 0 ? LS(std::to_string(x), std::to_string(x)) : LS();
        }) == LS("2", "2", "4", "4"));
        REQUIRE(LLI(LI(), LI(1, 2), LI(3), LI(4, 5, 6)).flatMap([](const LI &xs) {
            return List<std::size_t>(xs.size());
        }) == List<std::size_t>(0, 2, 1, 3));

        REQUIRE(LPI(PI(), PI(), PI(), PI(), PI()).flatMap([](const PI &) {
            return LS();
        }).isEmpty());
        REQUIRE(LPI(PI(new int(1)), PI(new int(4))).flatMap(stepsPI) ==
                LS("1", "2", "3", "4", "5", "6"));
        REQUIRE(LPI(
            PI(new int(1)),
            PI(new int(2)),
            PI(new int(3)),
            PI(new int(4)),
            PI(new int(5))
        ).flatMap([](const PI &p) {
            return *p % 2 == 1 ? LS(std::to_string(*p), std::to_string(*p)) : LS();
        }) == LS("1", "1", "3", "3", "5", "5"));
        REQUIRE(LLPI(
            LPI(),
            LPI(PI(new int(1)), PI(new int(2))),
            LPI(PI(new int(3))),
            LPI(PI(), PI(), PI())
        ).flatMap([](const LPI &ys) {
            return List<std::size_t>(ys.size());
        }) == List<std::size_t>(0, 2, 1, 3));
    }
}
