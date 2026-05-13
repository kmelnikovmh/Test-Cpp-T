#ifndef DUNGEON_HPP
#define DUNGEON_HPP

#include "room.hpp"

#include <vector>

namespace alice_sandbox_calc {

class Dungeon {
public:
    explicit Dungeon(std::vector<Room> rooms);

    int room_count() const;
    const Room& get_room(int id) const;
    Room& mutable_room(int id);
    std::vector<Room>& mutable_rooms();
    void reset();

    std::vector<int> shortest_path(int from, int to, const std::vector<bool>& allowed) const;
    std::vector<int> path_to_nearest_unvisited(int from, const std::vector<bool>& visited) const;

private:
    std::vector<Room> m_original;
    std::vector<Room> m_rooms;
};

} // namespace alice_sandbox_calc

#endif
