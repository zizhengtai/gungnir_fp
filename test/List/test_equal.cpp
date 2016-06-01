#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List equal", "[List][equal]") {
    SECTION("Lists with same size") {
        List<int> xs1;
        REQUIRE(xs1 == xs1);
        REQUIRE(xs1 == List<int>());
        REQUIRE(List<int>() == xs1);
        REQUIRE_FALSE(xs1 != xs1);
        REQUIRE_FALSE(xs1 != List<int>());
        REQUIRE_FALSE(List<int>() != xs1);

        List<int> xs2(123);
        REQUIRE(xs2 == xs2);
        REQUIRE(xs2 == List<int>(123));
        REQUIRE(List<int>(123) == xs2);
        REQUIRE_FALSE(xs2 == List<int>(456));
        REQUIRE_FALSE(List<int>(456) == xs2);
        REQUIRE_FALSE(xs2 != xs2);
        REQUIRE_FALSE(xs2 != List<int>(123));
        REQUIRE_FALSE(List<int>(123) != xs2);
        REQUIRE(xs2 != List<int>(456));
        REQUIRE(List<int>(456) != xs2);

        List<int> xs3(1, 2, 3, 4, 5);
        REQUIRE(xs3 == xs3);
        REQUIRE(xs3 == List<int>(1, 2, 3, 4, 5));
        REQUIRE(List<int>(1, 2, 3, 4, 5) == xs3);
        REQUIRE_FALSE(xs3 == List<int>(1, 2, 3, 4, 6));
        REQUIRE_FALSE(List<int>(1, 2, 3, 4, 6) == xs3);
        REQUIRE_FALSE(xs3 != xs3);
        REQUIRE_FALSE(xs3 != List<int>(1, 2, 3, 4, 5));
        REQUIRE_FALSE(List<int>(1, 2, 3, 4, 5) != xs3);
        REQUIRE(xs3 != List<int>(1, 2, 3, 4, 6));
        REQUIRE(List<int>(1, 2, 3, 4, 6) != xs3);
    }
    SECTION("Lists with different sizes") {
        List<int> xss[] = {
            List<int>(),
            List<int>(1),
            List<int>(2),
            List<int>(1, 2),
            List<int>(1, 3),
            List<int>(1, 2, 3),
            List<int>(1, 2, 2),
            List<int>(2, 2, 3),
            List<int>(1, 0, 3)
        };
        for (const auto& xs: xss) {
            for (const auto& ys: xss) {
                if (&xs == &ys) {
                    REQUIRE(xs == ys);
                    REQUIRE_FALSE(xs != ys);
                } else {
                    REQUIRE_FALSE(xs == ys);
                    REQUIRE(xs != ys);
                }
            }
        }
    }
}
