#ifndef _KD_TREE_H
#define _KD_TREE_H

#include "data_set.h"
#include <stack>
#include <set>
using namespace std;

class kd_tree_node;

/* get subdomain for a given query */
vector<int> kd_subdomain(euclid_vector * query, kd_tree_node * root);

/* generates a kd_tree for given data, with leaf size c */
kd_tree_node * kd_tree(int c, data_set & data);

/* generates a spill_tree for given data, with leaf size c */
kd_tree_node * spill_tree(int c, double a, data_set & data);

/* saves kd_tree into given file */
void save_kd_tree(kd_tree_node * root, FILE * out);

/* loads kd_tree from given file */
kd_tree_node * load_kd_tree(FILE * in);

class kd_tree_node
{
protected:
    int _index;
    double _pivot;
    vector<int> _domain;
    kd_tree_node * _left,
                 * _right;
    friend kd_tree_node * build_tree(int c, double a, 
            data_set & data, vector <int> subdomain);
public:
    kd_tree_node();
    kd_tree_node(vector <int> subdomain);
    kd_tree_node(int d, double p, vector<int> subdomain);
    kd_tree_node(const kd_tree_node & copy);
    ~kd_tree_node();
    int get_index() const;
    double get_pivot() const;
    virtual kd_tree_node * get_left() const;
    virtual kd_tree_node * get_right() const;
    vector <int> get_domain() const;
    friend void save_kd_tree(kd_tree_node * tree, FILE * out);
    friend kd_tree_node * load_kd_tree(FILE * in);
    friend class data_set;
};

#endif
