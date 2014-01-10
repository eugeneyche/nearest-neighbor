#include "data_set.h"
#include "kd_tree_node.h"
#include "vector_math.h"
#include <vector>
#include <map>

using namespace std;

euclid_vector * nn(euclid_vector * test, data_set & train_set);
euclid_vector * kd_tree_nn(euclid_vector *test, data_set & train_set, int c, kd_tree_node * root);
data_set k_nn(euclid_vector * test, data_set & train_st, double k); /* return the nearest neighbor that is generated from kd-tree */
data_set c_approx_nn(euclid_vector * test, data_set & train_st, euclid_vector * nn, double c); /* return the data_set of all vector within c*distance of the query */