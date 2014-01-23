#ifndef _QUERY_TREE_H
#define _QUERY_TREE_H

#include "kd_tree.h"
#include <stack>
using namespace std;

class query_tree_node;

/*build query tree*/
query_tree_node * build_query_tree(data_set & train, kd_tree_node * root, vector <int> sub_domain, int a);
/*search in query tree and return a vector of vectors*/
vector <int> query_nn_set(vector <int> domain, data_set * train_set, euclid_vector * test, query_tree_node * query_query_root);

class query_tree_node
{
private:
    int index;
    double pivot_l, pivot, pivot_r;
    query_tree_node * left, * right;
    vector <int> sub_domain;
public:
    query_tree_node();
    query_tree_node(int index, double pivot_l, double pivot, double pivot_r);
    query_tree_node(vector <int> sub_domain);
    ~query_tree_node();
    int get_index();
    bool in_range(euclid_vector * query);
    double get_pivot();
    query_tree_node * get_left();
    query_tree_node * get_right();
    vector <int> get_domain();
    friend query_tree_node * build_query_tree(data_set & train, kd_tree_node * root, vector <int> sub_domain, int a);
    friend vector <int> query_nn_set(vector <int> domain, data_set * train_set, euclid_vector * test, query_tree_node * query_query_root);
};

#endif
