#include "data_set.h"
#include "kd_tree_node.h"
#include "vector_math.h"
#include <vector>

using namespace std;

euclid_vector * nn(euclid_vector * test, data_set & train_set);

euclid_vector * kd_tree_nn(euclid_vector *test, data_set & train_set);