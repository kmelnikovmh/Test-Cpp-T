#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "bot.hpp"
#include "dungeon.hpp"
#include "resource.hpp"

#include <ostream>
#include <vector>

namespace alice_sandbox_calc {

void simulate(
    const std::vector<Action>& actions,
    Dungeon& dungeon,
    Resource demand,
    std::ostream& out);

} // namespace alice_sandbox_calc

#endif
