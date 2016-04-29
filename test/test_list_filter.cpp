#include <memory>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List filter", "[filter]") {
    SECTION("empty List") {
        List<int> xs;
        REQUIRE(xs.filter([](int x) { return x % 2 == 0; }).isEmpty());
        REQUIRE(xs.filter([](int x) { return x % 2 != 0; }).isEmpty());
        REQUIRE(xs.isEmpty());

        List<std::unique_ptr<int>> ys;
        REQUIRE(ys.filter([](const std::unique_ptr<int> &p) {
            return *p % 2 == 0;
        }).isEmpty());
        REQUIRE(ys.filter([](const std::unique_ptr<int> &p) {
            return *p % 2 != 0;
        }).isEmpty());
        REQUIRE(ys.isEmpty());
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.filter([](int x) { return x % 2 == 0; }).isEmpty());
        REQUIRE(xs.filter([](int x) { return x % 2 != 0; }) == xs);
        REQUIRE(xs == List<int>(123));

        List<std::unique_ptr<int>> ys(std::unique_ptr<int>(new int(456)));
        REQUIRE(ys.filter([](const std::unique_ptr<int> &p) {
            return *p % 2 == 0;
        }) == ys);
        REQUIRE(ys.filter([](const std::unique_ptr<int> &p) {
            return *p % 2 != 0;
        }).isEmpty());
        REQUIRE(ys.size() == 1);
        REQUIRE(*ys.head() == 456);
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 4, 5, 6);
        REQUIRE(xs.filter([](int x) { return x % 2 == 0; }) == List<int>(2, 4, 6));
        REQUIRE(xs.filter([](int x) { return x % 2 != 0; }) == List<int>(1, 5));
        REQUIRE(xs == List<int>(1, 2, 4, 5, 6));

        List<std::unique_ptr<int>> ys1(
            std::unique_ptr<int>(new int(11)),
            std::unique_ptr<int>(new int(12)),
            std::unique_ptr<int>(new int(14)),
            std::unique_ptr<int>(new int(15)),
            std::unique_ptr<int>(new int(16))
        );
        const auto ys2 = ys1.filter([](const std::unique_ptr<int> &p) {
            return *p % 2 == 0;
        });
        REQUIRE(ys2.size() == 3);
        REQUIRE(*ys2[0] == 12);
        REQUIRE(*ys2[1] == 14);
        REQUIRE(*ys2[2] == 16);
        const auto ys3 = ys1.filter([](const std::unique_ptr<int> &p) {
            return *p % 2 != 0;
        });
        REQUIRE(ys3.size() == 2);
        REQUIRE(*ys3[0] == 11);
        REQUIRE(*ys3[1] == 15);
        const auto ys4 = ys1.filter([](const std::unique_ptr<int> &) {
            return true;
        });
        REQUIRE(ys4 == ys1);
        const auto ys5 = ys1.filter([](const std::unique_ptr<int> &) {
            return false;
        });
        REQUIRE(ys5.isEmpty());
        REQUIRE(ys1.size() == 5);
        REQUIRE(ys1.map([](const std::unique_ptr<int> &p) {
            return *p;
        }) == List<int>(11, 12, 14, 15, 16));
    }
}
