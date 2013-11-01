#ifndef _NN_H
#define _NN_H

#include "bundle.h"
#include <fstream>
#include <vector>
using namespace std;

int read_bytes(int n_bytes, ifstream & in);
bool load_set(vector <Bundle> & out_set, ifstream & label_in, ifstream & vtr_in);

int nn1(Bundle test_set, vector <Bundle> train_set, vector <int> domain);

#endif
