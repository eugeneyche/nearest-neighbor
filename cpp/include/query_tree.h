#ifndef _QUERY_TREE_H
#define _QUERY_TREE_H

#include "kd_tree.h"
#include <set>
#include <stack>
using namespace std;

class query_tree_node;

/* get subdomain for a given query */
vector <int> query_subdomain(euclid_vector * query, query_tree_node * root);

/* generates a query_tree for given data, with range parameter a */
query_tree_node * query_tree(double a, kd_tree_node * root, data_set & data);

class query_tree_node : public kd_tree_node
{
protected:
    double _pivot_l, _pivot_r;
    friend query_tree_node * build_tree(double a, kd_tree_node * root,
            data_set & data);
public:
    query_tree_node();
    query_tree_node(kd_tree_node * kd_node);
    query_tree_node(kd_tree_node * kd_node, double pivot_l, double pivot_r);
    query_tree_node(const query_tree_node & copy);
    ~query_tree_node();
    bool in_range(euclid_vector * query) const;
    query_tree_node * get_left() const;
    query_tree_node * get_right() const;
};

#endif
