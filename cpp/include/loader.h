#ifndef _LOADER_H
#define _LOADER_H
#include "data_set.h"
#include <fstream>
using namespace std;

data_set & load(ifstream & in);
#endif
