#include "query_tree.h"
#include <vector>
#include <map>

using namespace std;

/* gets nearest neightbor of query */
euclid_vector * nn(euclid_vector * query, data_set & data);

/* gets k nearest neighbor of query */
data_set k_nn(int k, euclid_vector * query, data_set & data);
/* return the data_set of all vector within c*distance of the query */
data_set c_approx_nn(double c, euclid_vector * query, data_set & data, euclid_vector * nn);

/* gets nearest neighbor of query through kd_tree */
euclid_vector * kd_tree_nn(euclid_vector * query, data_set & data, kd_tree_node * root);
/* gets k nearest neighbor of query through kd_tree */
data_set kd_tree_k_nn(int k, euclid_vector * query, data_set & data, kd_tree_node * root);
/* gets nearest neighbor of query through query_tree */
euclid_vector * query_tree_nn(euclid_vector * query, data_set & data, query_tree_node * root);
/* gets k nearest neighbor of query through query_tree */
data_set query_tree_k_nn(int k, euclid_vector * query, data_set & data, query_tree_node * root);


