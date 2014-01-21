#include "data_set.h"
#include "kd_tree_node.h"
#include "vector_math.h"
#include <vector>
#include <map>

using namespace std;

/* gets nearest neightbor of query (test) */
euclid_vector * nn(euclid_vector * query, data_set & train_set);

/* get the k nearest neighbor of query */
data_set k_nn(euclid_vector * query, data_set & train_st, double k);

/* gets nearest neighbor of query through kd_tree*/
euclid_vector * kd_tree_nn(euclid_vector * query, data_set & train_set, int c, kd_tree_node * root);

/* return the data_set of all vector within c*distance of the query */
data_set c_approx_nn(euclid_vector * query, data_set & train_st, euclid_vector * nn, double c);

euclid_vector spill_query_nn(euclid_vector * train, data_set & train_set, kd_tree_node * root_p, double a);

