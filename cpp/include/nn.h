#include "data_set.h"
#include "vector_math.h"
#include <vector>
#include <map>

using namespace std;

euclid_vector * nn(euclid_vector * test, data_set & train_st);
double selector(vector <double> s, double k);
data_set k_nn(euclid_vector * test, data_set & train_st, double k);
