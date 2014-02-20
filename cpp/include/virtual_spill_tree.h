#ifndef _VIRTUAL_SPILL_TREE_H
#define _VIRTUAL_SPILL_TREE_H

#include "kd_tree.h"
#include <set>
#include <stack>
using namespace std;

class virtual_spill_tree_node;

/* get subdomain for a given query */
vector <int> virtual_spill_subdomain(euclid_vector * query, virtual_spill_tree_node * root);

/* generates a virtual_spill_tree for given data, with range parameter a */
virtual_spill_tree_node * virtual_spill_tree(double a, kd_tree_node * root, data_set & data);

class virtual_spill_tree_node : public kd_tree_node
{
protected:
    double _pivot_l, _pivot_r;
    friend virtual_spill_tree_node * build_virtual_tree(double a, kd_tree_node * root,
            data_set & data);
public:
    virtual_spill_tree_node();
    virtual_spill_tree_node(kd_tree_node * kd_node);
    virtual_spill_tree_node(kd_tree_node * kd_node, double pivot_l, double pivot_r);
    virtual_spill_tree_node(const virtual_spill_tree_node & copy);
    ~virtual_spill_tree_node();
    bool in_range(euclid_vector * query) const;
    double get_pivot_l() const;
    double get_pivot_r() const;
    virtual_spill_tree_node * get_left() const;
    virtual_spill_tree_node * get_right() const;
};

#endif
