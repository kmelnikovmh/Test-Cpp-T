#ifndef PARSER_HPP
#define PARSER_HPP

#include "dungeon.hpp"
#include "resource.hpp"

#include <istream>
#include <optional>
#include <string>
#include <string_view>
#include <variant>

namespace alice_sandbox_calc {

struct ParsedInput {
    Dungeon m_dungeon;
    int m_food;
    Resource m_demand;
};

std::optional<Resource> parse_resource(std::string_view s);
const char* resource_to_string(Resource r);

// variant вместо try/catch для разнообразия
std::variant<ParsedInput, std::string> parse_input(std::istream& in);

} // namespace alice_sandbox_calc

#endif
