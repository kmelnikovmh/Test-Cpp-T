#include "dungeon.hpp"

#include <gtest/gtest.h>
#include <vector>

using namespace alice_sandbox_calc;

namespace {

Dungeon make_sample()
{
    std::vector<Room> rooms(6);
    rooms[0] = {0, {1, 2}, {}};
    rooms[1] = {1, {0, 3}, {}};
    rooms[2] = {2, {0, 4}, {}};
    rooms[3] = {3, {1, 4, 5}, {}};
    rooms[4] = {4, {2, 3, 5}, {}};
    rooms[5] = {5, {3, 4}, {}};
    return Dungeon(std::move(rooms));
}

} // namespace

TEST(DungeonTest, ShortestPathMultiHop)
{
    auto d = make_sample();
    std::vector<bool> all(6, true);
    auto path = d.shortest_path(0, 5, all);
    EXPECT_EQ(path, (std::vector<int>{1, 3, 5}));
}

TEST(DungeonTest, NearestUnvisitedFarAway)
{
    auto d = make_sample();
    std::vector<bool> visited = {true, true, true, true, true, false};
    auto path = d.path_to_nearest_unvisited(0, visited);
    EXPECT_EQ(path, (std::vector<int>{1, 3, 5}));
}

TEST(DungeonTest, ResetRestoresOriginal)
{
    auto d = make_sample();
    d.mutable_room(1).m_resources[0] = 99;
    EXPECT_EQ(d.get_room(1).m_resources[0], 99);
    d.reset();
    EXPECT_EQ(d.get_room(1).m_resources[0], 0);
}
