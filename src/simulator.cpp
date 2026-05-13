#include "simulator.hpp"
#include "parser.hpp"

#include <cstring>
#include <string>
#include <vector>

namespace alice_sandbox_calc {

namespace {

void print_state(std::ostream& out, int room_id, const int* res, const char* collected)
{
    out << "state " << room_id;
    for (int i = 0; i < kResourceCount; ++i) {
        out << ' ';
        if (collected[i]) {
            out << '_';
        } else {
            out << res[i];
        }
    }
    out << '\n';
}

} // namespace

void simulate(
    const std::vector<Action>& actions,
    Dungeon& dungeon,
    Resource demand,
    std::ostream& out)
{
    dungeon.reset();
    int n = dungeon.room_count();
    std::vector<int> amounts(n * kResourceCount);
    std::vector<char> collected(n * kResourceCount, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < kResourceCount; ++j) {
            amounts[i * kResourceCount + j] = dungeon.get_room(i).m_resources[j];
        }
    }

    int inventory[kResourceCount] = {};
    int cur = 0;
    int last_go_zero = -1;
    for (int i = actions.size() - 1; 0 <= i; --i) {
        if (auto* go = std::get_if<GoAction>(&actions[i])) {
            if (go->m_room == 0) {
                last_go_zero = i;
                break;
            }
        }
    }

    for (int idx = 0; idx < static_cast<int>(actions.size()); ++idx) {
        const auto& action = actions[idx];

        if (auto* go = std::get_if<GoAction>(&action)) {
            out << "go " << go->m_room << '\n';
            cur = go->m_room;

            if (idx != last_go_zero) {
                print_state(out, cur, &amounts[cur * kResourceCount], &collected[cur * kResourceCount]);
            }
        } else if (auto* col = std::get_if<CollectAction>(&action)) {
            int ri = static_cast<int>(col->m_resource);
            out << "collect " << resource_to_string(col->m_resource) << '\n';
            inventory[ri] += amounts[cur * kResourceCount + ri];
            collected[cur * kResourceCount + ri] = 1;
            print_state(out, cur, &amounts[cur * kResourceCount], &collected[cur * kResourceCount]);
        }
    }

    int total = 0;
    for (int i = 0; i < kResourceCount; ++i) {
        total += inventory[i] * eff_value(static_cast<Resource>(i), demand);
    }
    if (cur == 0) {
        out << "result";
        for (int i = 0; i < kResourceCount; ++i) {
            out << ' ' << inventory[i];
        }
        out << ' ' << total << '\n';
    } else {
        out << "dead\n";
        out << "you're lose:";
        for (int i = 0; i < kResourceCount; ++i) {
            out << ' ' << inventory[i];
        }
        out << ' ' << total << '\n';
    }
}

} // namespace alice_sandbox_calc
