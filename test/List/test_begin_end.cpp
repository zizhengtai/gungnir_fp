#include <iterator>
#include <memory>
#include <type_traits>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List begin and end", "[List][begin][end]") {

    using PI = std::unique_ptr<int>;
    using IT = std::iterator_traits<List<int>::StdIterator>;
    using PIT = std::iterator_traits<List<PI>::StdIterator>;

    SECTION("empty List") {
        List<int> xs;
        for (bool b: {
                std::is_same<int, IT::value_type>::value,
                std::is_same<const int*, IT::pointer>::value,
                std::is_same<const int&, IT::reference>::value}) {
            REQUIRE(b);
        }
        REQUIRE(std::distance(xs.begin(), xs.end()) == 0);
        REQUIRE(xs.begin() == xs.end());

        List<PI> ys;
        for (bool b: {
                std::is_same<PI, PIT::value_type>::value,
                std::is_same<const PI*, PIT::pointer>::value,
                std::is_same<const PI&, PIT::reference>::value}) {
            REQUIRE(b);
        }
        REQUIRE(std::distance(ys.begin(), ys.end()) == 0);
        REQUIRE(ys.begin() == ys.end());
    }
    SECTION("List with one element") {
        List<int> xs(123);
        REQUIRE(std::distance(xs.begin(), xs.end()) == 1);
        REQUIRE(++xs.begin() == xs.end());
        REQUIRE(*xs.begin() == 123);

        List<PI> ys(PI(new int(456)));
        REQUIRE(std::distance(ys.begin(), ys.end()) == 1);
        REQUIRE(++ys.begin() == ys.end());
        REQUIRE(*ys.begin() == ys.head());
    }
    SECTION("List with multiple elements") {
        List<int> xs(1, 2, 3, 4, 5);
        REQUIRE(std::distance(xs.begin(), xs.end()) == 5);
        std::vector<int> v1{1, 2, 3, 4, 5}, v2(xs.begin(), xs.end()), v3;
        v3.reserve(5);
        for (const auto& x: xs) {
            v3.emplace_back(x);
        }
        REQUIRE(v1 == v2);
        REQUIRE(v1 == v3);

        List<PI> ys = xs.map([](int x) { return PI(new int(6 - x)); });
        REQUIRE(std::distance(ys.begin(), ys.end()) == 5);
        std::vector<int*> v4, v5, v6;
        v4.reserve(5);
        v5.reserve(5);
        v6.reserve(5);
        ys.foreach([&v4](const PI& p) { v4.emplace_back(p.get()); });
        std::transform(ys.begin(), ys.end(), std::back_inserter(v5),
                       [](const PI& p) { return p.get(); });
        for (const auto& p: ys) {
            v6.emplace_back(p.get());
        }
        REQUIRE(v4 == v5);
        REQUIRE(v4 == v6);
    }
}
