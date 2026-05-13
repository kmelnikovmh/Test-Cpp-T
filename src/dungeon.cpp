#include "dungeon.hpp"

#include <algorithm>
#include <queue>

namespace alice_sandbox_calc {

Dungeon::Dungeon(std::vector<Room> rooms) : m_original(rooms), m_rooms(std::move(rooms)) {}

int Dungeon::room_count() const
{
    return static_cast<int>(m_rooms.size());
}

const Room& Dungeon::get_room(int id) const
{
    return m_rooms[id];
}

Room& Dungeon::mutable_room(int id)
{
    return m_rooms[id];
}

std::vector<Room>& Dungeon::mutable_rooms()
{
    return m_rooms;
}

void Dungeon::reset()
{
    m_rooms = m_original;
}

std::vector<int> Dungeon::shortest_path(int from, int to, const std::vector<bool>& allowed) const
{
    if (from == to) return {};

    int n = static_cast<int>(m_rooms.size());
    std::vector<int> prev(n, -1);
    std::vector<bool> seen(n, false);
    std::queue<int> q;

    seen[from] = true;
    q.push(from);
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        for (int nb : m_rooms[cur].m_neighbors) {
            if (seen[nb] || !allowed[nb]) {
                continue;
            }
            prev[nb] = cur;
            if (nb == to) {
                std::vector<int> path;
                for (int v = to; v != from; v = prev[v]) {
                    path.push_back(v);
                }
                std::reverse(path.begin(), path.end());
                return path;
            }
            seen[nb] = true;
            q.push(nb);
        }
    }
    return {};
}

std::vector<int> Dungeon::path_to_nearest_unvisited(
    int from, const std::vector<bool>& visited) const
{
    int n = static_cast<int>(m_rooms.size());
    std::vector<int> prev(n, -1);
    std::vector<bool> seen(n, false);
    std::queue<int> q;
    seen[from] = true;
    q.push(from);
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        if (cur != from && !visited[cur]) {
            std::vector<int> path;
            for (int v = cur; v != from; v = prev[v]) {
                path.push_back(v);
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (int nb : m_rooms[cur].m_neighbors) {
            if (seen[nb]) continue;
            prev[nb] = cur;
            seen[nb] = true;
            q.push(nb);
        }
    }
    return {};
}

} // namespace alice_sandbox_calc
