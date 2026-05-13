#include "alice-bot.hpp"

#include <algorithm>
#include <numeric>
#include <vector>

namespace alice_sandbox_calc {

namespace {

int best_resource(const Room& room, Resource demand)
{
    int best = -1;
    int best_val = -1;
    for (int i = 0; i < kResourceCount; ++i) {
        if (room.m_resources[i] <= 0) continue;
        int v = eff_value(static_cast<Resource>(i), demand);
        if (best_val < v) {
            best_val = v;
            best = i;
        }
    }
    return best;
}

std::vector<int> eff_resources(const Room& room, Resource demand)
{
    std::vector<int> indices;
    for (int i = 0; i < kResourceCount; ++i) {
        if (0 < room.m_resources[i]) indices.push_back(i);
    }
    std::sort(indices.begin(), indices.end(), [&](int a, int b) {
        return eff_value(static_cast<Resource>(b), demand)
            < eff_value(static_cast<Resource>(a), demand);
    });
    return indices;
}

} // namespace

std::vector<Action> AliceBot::run(Dungeon& dungeon, int food, Resource demand)
{
    int n = dungeon.room_count();
    auto& rooms = dungeon.mutable_rooms();
    std::vector<Action> actions;
    std::vector<bool> visited(n, false);
    std::vector<bool> first_collect_used(n, false);

    int cur = 0;
    visited[0] = true;
    int food_spent = 0;
    int explore_budget = food / 2;
    while (food_spent < explore_budget) {
        int next = -1;
        for (int nb : rooms[cur].m_neighbors) {
            if (!visited[nb]) {
                next = nb;
                break;
            }
        }

        std::vector<int> path;
        if (0 <= next) {
            path = {next};
        } else {
            path = dungeon.path_to_nearest_unvisited(cur, visited);
            if (path.empty()) break;
        }

        if (explore_budget < food_spent + static_cast<int>(path.size())) break;

        for (int step : path) {
            actions.push_back(GoAction{step});
            ++food_spent;
            cur = step;
            visited[step] = true;

            int best = best_resource(rooms[cur], demand);
            if (0 <= best) {
                actions.push_back(CollectAction{static_cast<Resource>(best)});
                rooms[cur].m_resources[best] = 0;
                first_collect_used[cur] = true;
            }
        }
    }

    int food_left = food - food_spent;
    std::vector<int> ret_path = dungeon.shortest_path(cur, 0, visited);

    for (int i = 0; i < static_cast<int>(ret_path.size()); ++i) {
        int step = ret_path[i];
        bool is_final = (step == 0);

        if (!is_final || 0 < i) {
            int dist = ret_path.size() - i;
            int surplus = food_left - dist;

            auto avail = eff_resources(rooms[cur], demand);
            for (int ri : avail) {
                if (surplus <= 0) break;
                if (rooms[cur].m_resources[ri] <= 0) continue;

                int collect_cost = first_collect_used[cur] ? 1 : 0;
                if (!first_collect_used[cur]) {
                    first_collect_used[cur] = true;
                    collect_cost = 0;
                } else {
                    collect_cost = 1;
                }

                if (surplus < collect_cost) continue;

                actions.push_back(CollectAction{static_cast<Resource>(ri)});
                rooms[cur].m_resources[ri] = 0;
                food_left -= collect_cost;
                surplus -= collect_cost;
            }
        }

        actions.push_back(GoAction{step});
        --food_left;
        cur = step;
    }

    return actions;
}

} // namespace alice_sandbox_calc
