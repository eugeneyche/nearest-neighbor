#include <iostream>
#include <cstdio>
#include "vector_math.h"
#include "nn.h"
#include "kd_tree.h"
#include "spill_tree.h"
#include "virtual_spill_tree.h"
#include "data_set.h"
#include "test.h"
using namespace std;

typedef unsigned char byte;

int main() 
{
    Test<byte, byte> mTest ("data/mnist");
    mTest.generate_trees();
}
