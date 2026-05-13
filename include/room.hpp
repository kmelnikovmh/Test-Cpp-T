#ifndef ROOM_HPP
#define ROOM_HPP

#include "resource.hpp"

#include <vector>

namespace alice_sandbox_calc {

struct Room {
    int m_id = -1;
    std::vector<int> m_neighbors;
    int m_resources[kResourceCount] = {};
};

} // namespace alice_sandbox_calc

#endif
