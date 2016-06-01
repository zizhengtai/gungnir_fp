#include <memory>
#include <stdexcept>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List reduceRight", "[List][reduceRight]") {

    struct Base {
        Base(int x) noexcept : val(x) {}
        int val;
    };
    struct Derived : Base {
        Derived(double x) noexcept : Base(x), vald(x) {}
        double vald;
    };

    SECTION("empty List") {
        REQUIRE_THROWS_AS(List<int>().reduceRight([](int x, int y) {
            return x + y;
        }), std::out_of_range);
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.reduceRight([](int x, int y) {
            return x + y;
        }) == 123);

        List<Derived> ys(456);
        REQUIRE(ys.reduceRight([](const Derived& x, const Derived& y) {
            return Derived(x.vald + y.vald);
        }).vald == Approx(456));
        REQUIRE(ys.reduceRight([](const Derived& x, const Base& y) {
            return Base(x.val + y.val);
        }).val == 456);
    }
    SECTION("List with multiple elements") {
        List<double> xs(1, 2, 3, 4, 5);
        REQUIRE(xs.reduceRight([](double x, double y) { return y / x; }) ==
                Approx(5.0 / 4 / 3 / 2 / 1));

        List<Derived> ys(5, 4, 3, 2, 1);
        REQUIRE(ys.reduceRight([](const Derived& x, const Derived& y) {
            return Derived(y.vald / x.vald);
        }).vald == Approx(1.0 / 2 / 3 / 4 / 5));
        REQUIRE(ys.reduceRight([](const Derived& x, const Base& y) {
            return Base(y.val - x.val);
        }).val == 1 - 2 - 3 - 4 - 5);
    }
}
