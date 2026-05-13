#include "resource.hpp"

#include <gtest/gtest.h>

using namespace alice_sandbox_calc;

TEST(ResourceTest, DoubledTarget)
{
    EXPECT_EQ(eff_value(Resource::Iron, Resource::Iron), 14);
    EXPECT_EQ(eff_value(Resource::Gold, Resource::Gold), 22);
    EXPECT_EQ(eff_value(Resource::Gems, Resource::Gems), 46);
    EXPECT_EQ(eff_value(Resource::Exp,  Resource::Exp),  2);
}
