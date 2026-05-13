#include "alice-bot.hpp"

#include <gtest/gtest.h>

#include <string>
#include <variant>
#include <vector>

using namespace alice_sandbox_calc;

TEST(AliceBotTest, TwoRoomsMinimal)
{
    std::vector<Room> rooms(2);
    rooms[0] = {0, {1}, {0, 0, 0, 0}};
    rooms[1] = {1, {0}, {0, 0, 5, 0}};

    Dungeon dungeon(std::move(rooms));
    AliceBot bot;
    auto actions = bot.run(dungeon, 2, Resource::Gems);

    ASSERT_EQ(actions.size(), 3);
    EXPECT_EQ(std::get<GoAction>(actions[0]).m_room, 1);
    EXPECT_EQ(std::get<CollectAction>(actions[1]).m_resource, Resource::Gems);
    EXPECT_EQ(std::get<GoAction>(actions[2]).m_room, 0);
}

TEST(AliceBotTest, ReturnsToRoom0)
{
    std::vector<Room> rooms(2);
    rooms[0] = {0, {1}, {0, 0, 0, 0}};
    rooms[1] = {1, {0}, {5, 3, 1, 10}};

    Dungeon dungeon(std::move(rooms));
    AliceBot bot;
    auto actions = bot.run(dungeon, 2, Resource::Iron);

    ASSERT_FALSE(actions.empty());
    auto& last = actions.back();
    ASSERT_TRUE(std::holds_alternative<GoAction>(last));
    EXPECT_EQ(std::get<GoAction>(last).m_room, 0);
}
