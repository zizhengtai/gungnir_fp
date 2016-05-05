#include <memory>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List sorted", "[List][sorted]") {

    using PI = std::unique_ptr<int>;

    static const auto ltP = [](const PI &x, const PI &y) { return *x < *y; };
    static const auto gtI = [](int x, int y) { return x > y; };
    static const auto gtP = [](const PI &x, const PI &y) { return *x > *y; };
    static const auto toInt = [](const PI &p) { return *p; };

    SECTION("empty List") {
        List<int> xs;
        REQUIRE(xs.sorted().isEmpty());
        REQUIRE(xs.sorted(gtI).isEmpty());

        List<PI> ys;
        REQUIRE(ys.sorted(ltP).isEmpty());
        REQUIRE(ys.sorted(gtP).isEmpty());
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.sorted() == List<int>(123));
        REQUIRE(xs.sorted(gtI) == List<int>(123));

        List<PI> ys(PI(new int(456)));;
        REQUIRE(ys.sorted(ltP) == ys);
        REQUIRE(ys.sorted(gtP) == ys);

    }
    SECTION("List with multiple element") {
        List<List<int>> xss1(
            List<int>(7, 1, 0, 6, 4, 3, 9, 2, 8, 5),
            List<int>(0, 9, 5, 2, 3, 1, 7, 8, 4, 6),
            List<int>(2, 0, 8, 1, 5, 4, 9, 7, 3, 6),
            List<int>(4, 7, 3, 8, 1, 6, 0, 2, 5, 9)
        );
        REQUIRE(xss1.forall([](const List<int> &xs) {
            return xs.sorted() == List<int>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
        }));
        REQUIRE(xss1.forall([](const List<int> &xs) {
            return xs.sorted(gtI) == List<int>(9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
        }));

        List<List<int>> xss2(
            List<int>(4, 0, 1, 3, 1, 4, 0, 2, 2, 3),
            List<int>(1, 1, 2, 2, 3, 0, 0, 4, 3, 4),
            List<int>(2, 0, 2, 3, 4, 4, 0, 3, 1, 1),
            List<int>(1, 0, 3, 4, 2, 2, 0, 4, 1, 3)
        );
        REQUIRE(xss2.forall([](const List<int> &xs) {
            return xs.sorted() == List<int>(0, 0, 1, 1, 2, 2, 3, 3, 4, 4);
        }));
        REQUIRE(xss2.forall([](const List<int> &xs) {
            return xs.sorted(gtI) == List<int>(4, 4, 3, 3, 2, 2, 1, 1, 0, 0);
        }));

        const auto yss1 = xss1.map([](const List<int> &xs) {
            return xs.map([](int x) { return PI(new int(x)); });
        });
        REQUIRE(yss1.forall([](const List<PI> &ys) {
            return ys.sorted(ltP).map(toInt) == List<int>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
        }));
        REQUIRE(yss1.forall([](const List<PI> &ys) {
            return ys.sorted(gtP).map(toInt) == List<int>(9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
        }));


        const auto yss2 = xss2.map([](const List<int> &xs) {
            return xs.map([](int x) { return PI(new int(x)); });
        });
        REQUIRE(yss2.forall([](const List<PI> &ys) {
            return ys.sorted(ltP).map(toInt) == List<int>(0, 0, 1, 1, 2, 2, 3, 3, 4, 4);
        }));
        REQUIRE(yss2.forall([](const List<PI> &ys) {
            return ys.sorted(gtP).map(toInt) == List<int>(4, 4, 3, 3, 2, 2, 1, 1, 0, 0);
        }));
    }
    SECTION("sort is stable") {
        struct Foo {
            Foo(int val) noexcept : val_(val) {}
            int val() const { return val_; }
            bool operator<(const Foo &) const { return false; }
        private:
            const int val_;
        };

        List<List<int>> xss(
            List<int>(7, 1, 0, 6, 4, 3, 9, 2, 8, 5),
            List<int>(0, 9, 5, 2, 3, 1, 7, 8, 4, 6),
            List<int>(2, 0, 8, 1, 5, 4, 9, 7, 3, 6),
            List<int>(4, 7, 3, 8, 1, 6, 0, 2, 5, 9)
        );
        const auto yss = xss.map([](const List<int> &xs) {
            return xs.map([](int x) { return Foo(x); });
        });
        REQUIRE(yss.size() == 4);
        for (std::size_t i = 0; i < yss.size(); ++i) {
            REQUIRE(yss[i].sorted().map(&Foo::val) == xss[i]);
        }
    }
}
