#include <memory>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List flatten", "[List][flatten]") {

    using PI = std::unique_ptr<int>;
    using LI = List<int>;
    using LLI = List<LI>;
    using LLLI = List<LLI>;
    using LPI = List<PI>;
    using LLPI = List<LPI>;
    using LLLPI = List<LLPI>;

    SECTION("empty List") {
        REQUIRE(LLI().flatten() == LI());
        REQUIRE(LLLI().flatten() == LLI());
        REQUIRE(LLPI().flatten() == LPI());
        REQUIRE(LLLPI().flatten() == LLPI());
    }
    SECTION("List with one element") {
        REQUIRE(LLI(LI()) == LI());
        REQUIRE(LLI(LI(123)) == LI(123));
        REQUIRE(LLI(LI(1, 2, 3)) == LI(1, 2, 3));
        REQUIRE(LLLI(LLI()) == LLI());
        REQUIRE(LLLI(LLI(LI(123, 456))) == LLI(LI(123, 456)));

        REQUIRE(LLPI(LPI()) == LPI());
        const auto ys1 = LLPI(LPI(PI(new int(123)))).flatten();
        REQUIRE(ys1.size() == 1);
        REQUIRE(*ys1.head() == 123);
        const auto ys2 =
            LLPI(LPI(PI(new int(1)), PI(new int(2)), PI(new int(3)))).flatten();
        REQUIRE(ys2.size() == 3);
        REQUIRE(*ys2[0] == 1);
        REQUIRE(*ys2[1] == 2);
        REQUIRE(*ys2[2] == 3);
        REQUIRE(LLLPI(LLPI()) == LLPI());
        const auto ys3 =
            LLLPI(LLPI(LPI(PI(new int(123)), PI(new int(456))))).flatten();
        REQUIRE(ys3.size() == 1);
        REQUIRE(ys3.head().size() == 2);
        REQUIRE(*ys3.head()[0] == 123);
        REQUIRE(*ys3.head()[1] == 456);

    }
    SECTION("List with multiple elements") {
        REQUIRE(LLI(LI(), LI(), LI()).flatten() == LI());
        REQUIRE(LLI(LI(), LI(1, 2), LI()).flatten() == LI(1, 2));
        REQUIRE(LLI(LI(), LI(1), LI(2, 3)).flatten() == LI(1, 2, 3));
        REQUIRE(LLI(LI(1, 2, 3), LI(4), LI(), LI(5, 6), LI(), LI(7)).flatten() ==
                LI(1, 2, 3, 4, 5, 6, 7));
        REQUIRE(LLLI(
            LLI(LI(1, 2)),
            LLI(LI(), LI(3)),
            LLI(),
            LLI(LI(4, 5), LI(), LI(6, 7), LI())
        ).flatten().flatten() == LI(1, 2, 3, 4, 5, 6, 7));

        REQUIRE(LLPI(LPI(), LPI(), LPI()).flatten() == LPI());
        const auto ys1 = LLPI(LPI(), LPI(PI(new int(1)), PI(new int(2))), LPI()).flatten();
        REQUIRE(ys1.size() == 2);
        REQUIRE(*ys1[0] == 1);
        REQUIRE(*ys1[1] == 2);
        const auto ys2 =
            LLPI(LPI(), LPI(PI(new int(1))), LPI(PI(new int(2)), PI(new int(3)))).flatten();
        REQUIRE(ys2.size() == 3);
        REQUIRE(*ys2[0] == 1);
        REQUIRE(*ys2[1] == 2);
        REQUIRE(*ys2[2] == 3);
        const auto ys3 = LLPI(
            LPI(PI(new int(1)), PI(new int(2)), PI(new int(3))),
            LPI(PI(new int(4))),
            LPI(),
            LPI(PI(new int(5)), PI(new int(6))),
            LPI(),
            LPI(PI(new int(7)))
        ).flatten();
        REQUIRE(ys3.size() == 7);
        for (int i = 0; i < 7; ++i) {
            REQUIRE(*ys3[i] == i + 1);
        }
        const auto ys4 = LLLPI(
            LLPI(LPI(PI(new int(1)), PI(new int(2)))),
            LLPI(LPI(), LPI(PI(new int(3)))),
            LLPI(),
            LLPI(
                LPI(PI(new int(4)), PI(new int(5))),
                LPI(),
                LPI(PI(new int(6)), PI(new int(7))),
                LPI()
            )
        ).flatten().flatten();
        REQUIRE(ys4.size() == 7);
        for (int i = 0; i < 7; ++i) {
            REQUIRE(*ys4[i] == i + 1);
        }
    }
}
