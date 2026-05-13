#ifndef ALICE_BOT_HPP
#define ALICE_BOT_HPP

#include "bot.hpp"

namespace alice_sandbox_calc {

class AliceBot : public IBot {
public:
    std::vector<Action> run(Dungeon& dungeon, int food, Resource demand) override;
};

} // namespace alice_sandbox_calc

#endif
