#ifndef _NN_H
#define _NN_H

#include "bundle.h"
#include <fstream>
#include <vector>
using namespace std;

int read_bytes(ifstream & in, int n_bytes);
bool load_set(vector <Bundle> & out_vtr, ifstream & label_in, ifstream & vtr_in);

int nn(const Bundle & test, const vector <Bundle> & train_set, vector <int> domain);

#endif
