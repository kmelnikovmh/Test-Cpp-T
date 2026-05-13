#include "simulator.hpp"

#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <vector>

using namespace alice_sandbox_calc;

namespace {

Dungeon make_two_rooms(int iron, int gold, int gems, int exp)
{
    std::vector<Room> rooms(2);
    rooms[0] = {0, {1}, {0, 0, 0, 0}};
    rooms[1] = {1, {0}, {iron, gold, gems, exp}};
    return Dungeon(std::move(rooms));
}

}  // namespace

TEST(SimulatorTest, MultipleCollectsInRoom)
{
    auto dungeon = make_two_rooms(3, 4, 0, 0);
    std::vector<Action> actions = {
        GoAction{1},
        CollectAction{Resource::Gold},
        CollectAction{Resource::Iron},
        GoAction{0},
    };

    std::ostringstream out;
    simulate(actions, dungeon, Resource::Iron, out);

    std::string expected =
        "go 1\n"
        "state 1 3 4 0 0\n"
        "collect gold\n"
        "state 1 3 _ 0 0\n"
        "collect iron\n"
        "state 1 _ _ 0 0\n"
        "go 0\n"
        "result 3 4 0 0 86\n";

    EXPECT_EQ(out.str(), expected);
}

TEST(SimulatorTest, DeathOutputWhenNotReturned)
{
    auto dungeon = make_two_rooms(5, 0, 0, 0);
    std::vector<Action> actions = {
        GoAction{1},
        CollectAction{Resource::Iron},
    };

    std::ostringstream out;
    simulate(actions, dungeon, Resource::Iron, out);

    std::string expected =
        "go 1\n"
        "state 1 5 0 0 0\n"
        "collect iron\n"
        "state 1 _ 0 0 0\n"
        "dead\n"
        "you're lose: 5 0 0 0 70\n";

    EXPECT_EQ(out.str(), expected);
}
