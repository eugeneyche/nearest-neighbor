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
void save_kd_tree(kd_tree_node * tree, FILE * out);
/* loads kd_tree from given file */
kd_tree_node * load_kd_tree(FILE * in);

class kd_tree_node
{
private:
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
    ~kd_tree_node();
    int get_index();
    double get_pivot();
    kd_tree_node * get_left();
    kd_tree_node * get_right();
    vector<int> get_domain();
    friend kd_tree_node * search(euclid_vector * test, kd_tree_node * node);
    friend vector<int> kd_subdomain(euclid_vector * test, kd_tree_node * root);
    friend void save_kd_tree(kd_tree_node * tree, FILE * out);
    friend kd_tree_node * load_kd_tree(FILE * in);
    friend class data_set;
};

#endif
