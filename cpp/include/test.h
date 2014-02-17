#ifndef _TEST_H
#define _TEST_H

#include "nn.h"
#include <cstdio>
#include <cstring>
using namespace std;

data_set & get_train_set(); 
data_set & get_test_set(); 

void test_setup(char * path);

#endif
