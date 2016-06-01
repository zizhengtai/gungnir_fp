#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List map", "[List][map]") {
    class Person {
    public:
        Person(std::string name, std::uint8_t age) noexcept
            : name_(std::move(name)), age_(age)
        {}
        const std::string& name() const { return name_; }
        std::uint8_t age() const { return age_; }
    private:
        const std::string name_;
        std::uint8_t age_;
    };

    SECTION("empty List") {
        List<int> xs1;
        List<std::string> xs2 = xs1.map([](int x) { return std::to_string(x); });
        REQUIRE(xs1.isEmpty());
        REQUIRE(xs2.isEmpty());

        List<std::unique_ptr<int>> ys1;
        List<int> ys2 = ys1.map([](const std::unique_ptr<int>& p) { return *p; });
        REQUIRE(ys1.isEmpty());
        REQUIRE(ys2.isEmpty());
    }
    SECTION("List with one element") {
        List<int> xs1(123);
        List<std::string> xs2 = xs1.map([](int x) { return std::to_string(x); });
        REQUIRE(xs1.size() == 1);
        REQUIRE(xs1.head() == 123);
        REQUIRE(xs2.size() == 1);
        REQUIRE(xs2.head() == "123");

        List<std::unique_ptr<int>> ys1(std::unique_ptr<int>(new int(456)));
        List<int> ys2 = ys1.map([](const std::unique_ptr<int>& p) { return *p; });
        REQUIRE(ys1.size() == 1);
        REQUIRE(*ys1.head() == 456);
        REQUIRE(ys2.size() == 1);
        REQUIRE(ys2.head() == 456);

    }
    SECTION("List with multiple element") {
        List<int> xs1(1, 2, 3, 4, 5);
        List<std::string> xs2 = xs1.map([](int x) { return std::to_string(x); });
        REQUIRE(xs1.size() == 5);
        REQUIRE(xs2.size() == 5);
        for (int i = 0; i < 5; ++i) {
            REQUIRE(xs1[i] == i + 1);
            REQUIRE(xs2[i] == std::to_string(i + 1));
        }

        List<std::unique_ptr<int>> ys1(
            std::unique_ptr<int>(new int(5)),
            std::unique_ptr<int>(new int(4)),
            std::unique_ptr<int>(new int(3)),
            std::unique_ptr<int>(new int(2)),
            std::unique_ptr<int>(new int(1))
        );
        List<int> ys2 = ys1.map([](const std::unique_ptr<int>& p) { return *p; });
        REQUIRE(ys1.size() == 5);
        REQUIRE(ys2.size() == 5);
        for (int i = 0; i < 5; ++i) {
            REQUIRE(*ys1[i] == 5 - i);
            REQUIRE(ys2[i] == 5 - i);
        }
    }
    SECTION("map with getter") {
        List<Person> xs1(
            Person("P1", 5),
            Person("P2", 4),
            Person("P3", 3),
            Person("P4", 2),
            Person("P5", 1)
        );
        List<std::string> xs2 = xs1.map(&Person::name);
        List<std::uint8_t> xs3 = xs1.map(&Person::age);
        REQUIRE(xs1.size() == 5);
        REQUIRE(xs2.size() == 5);
        REQUIRE(xs3.size() == 5);
        for (int i = 0; i < 5; ++i) {
            const auto name = "P" + std::to_string(i + 1);
            std::uint8_t age = 5 - i;

            REQUIRE(xs1[i].name() == name);
            REQUIRE(xs1[i].age() == age);
            REQUIRE(xs2[i] == name);
            REQUIRE(xs3[i] == age);
        }
    }
    SECTION("chained maps") {
        List<std::unique_ptr<std::vector<Person>>> xs1(
            std::unique_ptr<std::vector<Person>>(new std::vector<Person>{
                Person("P1", 5),
                Person("P2", 4)
            }),
            std::unique_ptr<std::vector<Person>>(new std::vector<Person>{
                Person("P3", 3),
                Person("P4", 2),
                Person("P5", 1)
            })
        );

        List<std::string> xs2 = xs1
            .map([](const std::unique_ptr<std::vector<Person>>& p) {
                return p->at(1);
            })
            .map(&Person::age)
            .map([](std::uint8_t x) {
                return x * 11;
            })
            .map([](int x) {
                return std::to_string(x) + "!";
            });

        REQUIRE(xs1.size() == 2);
        REQUIRE(xs2.size() == 2);
        REQUIRE(xs2[0] == "44!");
        REQUIRE(xs2[1] == "22!");
    }
}
