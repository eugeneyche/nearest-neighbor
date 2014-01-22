#ifndef _SPILL_TREE_H
#define _SPILL_TREE_H

#include "nn.h"
#include <stack>
using namespace std;

class spill_tree_node;

/*build query tree*/
spill_tree_node * build_query_tree(data_set * train, kd_tree_node * root, vector <int> sub_domain, int a);

/*search in query tree and return a vector of vectors*/
vector_space query_nn_set(vector_space vector_set, data_set * train_set, euclid_vector * test, spill_tree_node * spill_query_root);

spill_tree_node * spill_tree(int c, double a,
        data_set & data);

spill_tree_node * build_tree(int c, double a,
        data_set & data, vector <int> sub_domain);



class spill_tree_node
{
private:
    int index;
    double pivot_l, pivot, pivot_r;
    spill_tree_node * left, * right;
    vector <int> sub_domain;
public:
    spill_tree_node();
    spill_tree_node(int index, double pivot_l, double pivot, double pivot_r);
    spill_tree_node(vector <int> sub_domain);
    ~spill_tree_node();
    int get_index();
    bool in_range(euclid_vector * query);
    double get_pivot();
    spill_tree_node * get_left();
    spill_tree_node * get_right();
    vector <int> get_domain();
    friend spill_tree_node * build_tree(int c, double a, 
            data_set & data, vector <int> sub_domain);
    friend spill_tree_node * build_query_tree(data_set & train, kd_tree_node * root, vector <int> sub_domain, int a);
};

#endif
