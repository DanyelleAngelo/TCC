#include "gtest/gtest.h"
#include <sdsl/int_vector.hpp>
#include "../rmm_tree_classic_v2.hpp"

const int b=4;

TEST(RMMTree, localNodeExcess){
    int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};

    RMMTree t(v,b);
    t.buildingTree();

    EXPECT_EQ(t.tree[17].excess,4);
}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
