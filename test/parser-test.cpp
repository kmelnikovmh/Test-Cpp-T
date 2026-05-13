#include "parser.hpp"

#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <variant>

using namespace alice_sandbox_calc;

namespace {

std::variant<ParsedInput, std::string> parse(const std::string& content)
{
    std::istringstream ss(content);
    return parse_input(ss);
}

} // namespace

TEST(ParserTest, EmptyInput)
{
    auto r = parse("");
    ASSERT_FALSE(std::holds_alternative<ParsedInput>(r));
}

TEST(ParserTest, ValidShortRoom0)
{
    auto r = parse("1\n0 1\n1 0 5 3 1 10\n4 gold\n");
    ASSERT_TRUE(std::holds_alternative<ParsedInput>(r));
    EXPECT_EQ(std::get<ParsedInput>(r).m_dungeon.get_room(0).m_resources[0], 0);
}

TEST(ParserTest, InvalidRoom0NonzeroResources)
{
    auto r = parse("1\n0 1 5 0 0 0\n1 0 5 3 1 10\n4 iron\n");
    ASSERT_FALSE(std::holds_alternative<ParsedInput>(r));
    EXPECT_EQ(std::get<std::string>(r), "0 1 5 0 0 0");
}

TEST(ResourceTest, ParseValid)
{
    EXPECT_EQ(parse_resource("iron"), Resource::Iron);
    EXPECT_EQ(parse_resource("gold"), Resource::Gold);
    EXPECT_EQ(parse_resource("gems"), Resource::Gems);
    EXPECT_EQ(parse_resource("exp"),  Resource::Exp);
}