#include <iostream>
#include <cstdio>
#include "vector_math.h"
#include "nn.h"
#include "kd_tree.h"
#include "kd_spill_tree.h"
#include "kd_virtual_spill_tree.h"
#include "data_set.h"
#include "test.h"
#include "cifar.h"
using namespace std;

typedef unsigned char byte;

int main() 
{
    //cifar_generate();
    //Test<byte,float> mTest();
    Test<byte, byte> mTest ("/Users/janetzhai/Desktop/nn-xcode/nn-xcode/mnist");//"/Users/zhen/Desktop/nn-code/mnist");
    //mTest.generate_kd_trees();
    //mTest.generate_kd_tree_data("/Users/janetzhai/Desktop/nn-xcode/nn-xcode/mnist");//"/Users/zhen/Desktop/nn-code/mnist");
    //mTest.generate_multi_kd_trees();
    //mTest.generate_multi_kd_tree_data("/Users/janetzhai/Desktop/nn-xcode/nn-xcode/cifar");
    //mTest.generate_kd_spill_trees();
    //mTest.generate_kd_spill_tree_data("/Users/janetzhai/Desktop/nn-xcode/nn-xcode/mnist");//"/Users/zhen/Desktop/nn-code/mnist");
    //mTest.generate_bsp_trees();
    //mTest.generate_bsp_tree_data("/Users/janetzhai/Desktop/nn-xcode/nn-xcode/cifar");
    mTest.generate_bsp_spill_trees();
    mTest.generate_bsp_spill_tree_data("/Users/janetzhai/Desktop/nn-xcode/nn-xcode/mnist");
    //mTest.generate_kd_v_spill_trees();
    //mTest.generate_kd_v_spill_tree_data("/Users/janetzhai/Desktop/nn-xcode/nn-xcode/mnist");//"/Users/zhen/Desktop/nn-code/mnist");
}
