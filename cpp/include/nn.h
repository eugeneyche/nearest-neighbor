#include "data_set.h"
#include "kd_tree_node.h"
#include "vector_math.h"
#include <vector>
#include <map>

using namespace std;

euclid_vector * nn(euclid_vector * test, data_set & train_set);
euclid_vector * kd_tree_nn(euclid_vector *test, data_set & train_set, int c);
data_set k_nn(euclid_vector * test, data_set & train_st, double k);