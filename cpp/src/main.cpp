#include <iostream>
#include <cstdio>
#include "vector_math.h"
#include "nn.h"
#include "kd_tree.h"
#include "kd_spill_tree.h"
#include "kd_virtual_spill_tree.h"
#include "data_set.h"
#include "test.h"
using namespace std;

typedef unsigned char byte;

int main() 
{
    Test<byte, float> mTest ("data/eharmony");
    mTest.generate_kd_tree_data();
    mTest.generate_kd_spill_tree_data();
    mTest.generate_bsp_tree_data();
    mTest.generate_bsp_spill_tree_data();
}
