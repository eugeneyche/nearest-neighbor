#ifndef _SPILL_QUERY_TREE_H
#define _SPILL_QUERY_TREE_H

#include "data_set.h"
#include <stack>
using namespace std;

class spill_query_tree_node;

spill_query_tree_node * spill_query_tree(int c, double a,
        data_set & data);


class spill_query_tree_node
{
private:
    int index;
    double pivot_l, pivot, pivot_r;
    spill_query_tree_node * left, * right;
    vector <int> sub_domain;
public:
    spill_query_tree_node();
    spill_query_tree_node(int index, double pivot_l, double pivot, double pivot_r);
    spill_query_tree_node(vector <int> sub_domain);
    ~spill_query_tree_node();
    int get_index();
    bool in_range(euclid_vector * query);
    double get_pivot();
    friend spill_query_tree_node * build_tree(int c, double a, 
            data_set & data, vector <int> sub_domain);
};

#endif
