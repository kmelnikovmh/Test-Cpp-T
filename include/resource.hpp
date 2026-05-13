#ifndef RESOURCE_HPP
#define RESOURCE_HPP

namespace alice_sandbox_calc {

enum class Resource : int { Iron = 0, Gold = 1, Gems = 2, Exp = 3 };
const int kResourceCount = 4;
const int kBaseValue[kResourceCount] = {7, 11, 23, 1};
const char* const kResourceName[kResourceCount] = {"iron", "gold", "gems", "exp"};

inline int eff_value(Resource r, Resource demand)
{
    int v = kBaseValue[static_cast<int>(r)];
    return r == demand ? v * 2 : v;
}

} // namespace alice_sandbox_calc

#endif
