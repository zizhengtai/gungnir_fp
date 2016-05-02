#include <memory>
#include <stdexcept>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List init", "[List][init]") {
    SECTION("empty List") {
        List<int> xs;
        REQUIRE_THROWS_AS(xs.init(), std::out_of_range);

        List<std::unique_ptr<int>> ys;
        REQUIRE_THROWS_AS(ys.init(), std::out_of_range);
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(xs.init().isEmpty());

        List<std::unique_ptr<int>> ys(std::unique_ptr<int>(new int(456)));
        REQUIRE(ys.init().isEmpty());
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3, 4, 5);
        REQUIRE(xs.init() == List<int>(1, 2, 3, 4));
        REQUIRE(xs.init().init() == List<int>(1, 2, 3));

        List<std::unique_ptr<int>> ys(
            std::unique_ptr<int>(new int(5)),
            std::unique_ptr<int>(new int(4)),
            std::unique_ptr<int>(new int(3)),
            std::unique_ptr<int>(new int(2)),
            std::unique_ptr<int>(new int(1))
        );
        REQUIRE(ys.init().size() == 4);
        for (int i = 0; i < 4; ++i) {
            REQUIRE(ys.init()[i] == ys[i]);
        }
        REQUIRE(ys.init().init().size() == 3);
        for (int i = 0; i < 3; ++i) {
            REQUIRE(ys.init().init()[i] == ys[i]);
        }
    }
}
