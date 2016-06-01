#include <memory>
#include <vector>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List foreach", "[List][foreach]") {
    SECTION("empty List") {
        std::vector<int> v;

        List<int> xs;
        xs.foreach([&v](int x) { v.emplace_back(x); });
        REQUIRE(v.empty());
        REQUIRE(xs.isEmpty());

        List<std::unique_ptr<int>> ys;
        ys.foreach([&v](const std::unique_ptr<int>& p) {
            v.emplace_back(*p);
        });
        REQUIRE(v.empty());
        REQUIRE(ys.isEmpty());
    }
    SECTION("List with one element") {
        std::vector<int> v;

        List<int> xs(123);
        xs.foreach([&v](int x) { v.emplace_back(x); });
        REQUIRE(v.size() == 1);
        REQUIRE(v[0] == 123);
        REQUIRE(xs.size() == 1);
        REQUIRE(xs[0] == 123);

        List<std::unique_ptr<int>> ys(std::unique_ptr<int>(new int(456)));
        ys.foreach([&v](const std::unique_ptr<int>& p) {
            v.emplace_back(*p);
        });
        REQUIRE(v.size() == 2);
        REQUIRE(v[0] == 123);
        REQUIRE(v[1] == 456);
        REQUIRE(ys.size() == 1);
        REQUIRE(*ys[0] == 456);
    }
    SECTION("List with multiple elements") {
        std::vector<int> v;

        List<int> xs(1, 2, 3, 4);
        xs.foreach([&v](int x) { v.emplace_back(x); });
        REQUIRE(v.size() == 4);
        REQUIRE(xs.size() == 4);
        for (int i = 0; i < 4; ++i) {
            REQUIRE(v[i] == i + 1);
            REQUIRE(xs[i] == i + 1);
        }

        List<std::unique_ptr<int>> ys(
            std::unique_ptr<int>(new int(5)),
            std::unique_ptr<int>(new int(6)),
            std::unique_ptr<int>(new int(7)),
            std::unique_ptr<int>(new int(8))
        );
        ys.foreach([&v](const std::unique_ptr<int>& p) {
            v.emplace_back(*p);
        });
        REQUIRE(v.size() == 8);
        REQUIRE(ys.size() == 4);
        for (int i = 0; i < 8; ++i) {
            REQUIRE(v[i] == i + 1);
        }
        for (int i = 0; i < 4; ++i) {
            REQUIRE(*ys[i] == i + 5);
        }
    }
}
