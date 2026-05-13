#ifndef BOT_HPP
#define BOT_HPP

#include "dungeon.hpp"
#include "resource.hpp"

#include <variant>
#include <vector>

namespace alice_sandbox_calc {

struct GoAction {
    int m_room;
};

struct CollectAction {
    Resource m_resource;
};

using Action = std::variant<GoAction, CollectAction>;

class IBot {
public:
    virtual ~IBot() = default;
    virtual std::vector<Action> run(Dungeon& dungeon, int food, Resource demand) = 0;
};

} // namespace alice_sandbox_calc

#endif
