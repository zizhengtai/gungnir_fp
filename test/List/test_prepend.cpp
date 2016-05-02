#include <cstddef>
#include <memory>
#include <string>

#include "catch.hpp"

#include "gungnir/List.hpp"
using gungnir::List;

TEST_CASE("test List prepend", "[List][prepend]") {
    class Person {
    public:
        Person(std::string name, std::uint8_t age) noexcept
            : name_(std::move(name)), age_(age)
        {}

        const std::string & name() const
        {
            return name_;
        }

        std::uint8_t age() const
        {
            return age_;
        }

    private:
        const std::string name_;
        const std::uint8_t age_;
    };

    using PI = std::unique_ptr<int>;

    SECTION("empty List") {
        REQUIRE(List<int>().prepend(123) == List<int>(123));

        const auto ys = List<PI>().prepend(new int(456));
        REQUIRE(ys.size() == 1);
        REQUIRE(ys.head() != nullptr);
        REQUIRE(*ys.head() == 456);

        const auto zs = List<Person>().prepend("Zizheng", 20);
        REQUIRE(zs.size() == 1);
        REQUIRE(zs.head().name() == "Zizheng");
        REQUIRE(zs.head().age() == 20);
    }
    SECTION("List with one element") {
        REQUIRE(List<int>(123).prepend(456) == List<int>(456, 123));

        const auto ys = List<PI>(PI(new int(456))).prepend(new int(123));
        REQUIRE(ys.size() == 2);
        REQUIRE(ys[0] != nullptr);
        REQUIRE(ys[1] != nullptr);
        REQUIRE(*ys[0] == 123);
        REQUIRE(*ys[1] == 456);

        const auto zs = List<Person>(Person("Z1", 20)).prepend("Z2", 21);
        REQUIRE(zs.size() == 2);
        REQUIRE(zs[0].name() == "Z2");
        REQUIRE(zs[0].age() == 21);
        REQUIRE(zs[1].name() == "Z1");
        REQUIRE(zs[1].age() == 20);
    }
    SECTION("List with multiple element") {
        REQUIRE(List<int>(2, 3, 4, 5).prepend(1) == List<int>(1, 2, 3, 4, 5));

        const auto ys = List<PI>(
            PI(new int(4)),
            PI(new int(3)),
            PI(new int(2)),
            PI(new int(1))
        ).prepend(new int(5));
        REQUIRE(ys.size() == 5);
        for (int i = 0; i < 5; ++i) {
            REQUIRE(ys[i] != nullptr);
            REQUIRE(*ys[i] == 5 - i);
        }

        const auto zs = List<Person>(
            Person("Z1", 21),
            Person("Z2", 22),
            Person("Z3", 23),
            Person("Z4", 24)
        ).prepend("Z0", 20);
        REQUIRE(zs.size() == 5);
        for (int i = 0; i < 5; ++i) {
            REQUIRE(zs[i].name() == "Z" + std::to_string(i));
            REQUIRE(zs[i].age() == 20 + i);
        }
    }
}
