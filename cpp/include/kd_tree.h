#ifndef _KD_TREE_H
#define _KD_TREE_H

#include "data_set.h"
#include <stack>
using namespace std;

class kd_tree_node;

void print_tree(kd_tree_node * m_node, int depth);
vector<int> sub_domain(euclid_vector * test, kd_tree_node * root);

kd_tree_node * kd_tree(int c, data_set & data);
kd_tree_node * spill_tree(int c, double a, data_set & data);

void save_tree(kd_tree_node * tree, FILE * out);
kd_tree_node * load_tree(FILE * in);

void print_tree(kd_tree_node * m_node, int depth);

class kd_tree_node
{
private:
    int index;
    double pivot;
    vector<int> domain;
    kd_tree_node * left;
    kd_tree_node * right;
    friend kd_tree_node * build_tree(int c, double a, 
            data_set & data, vector <int> sub_domain);
    
public:
    kd_tree_node();
    kd_tree_node(int d, double p, vector<int> sub_domain);
    kd_tree_node(vector <int> sub_domain);
    ~kd_tree_node();
    int get_index();
    double get_pivot();
    kd_tree_node * get_left();
    kd_tree_node * get_right();
    vector<int> get_domain();
    friend kd_tree_node * search(euclid_vector * test, kd_tree_node * node);
    friend vector<int> sub_domain(euclid_vector * test, kd_tree_node * root);
    friend void save_tree(kd_tree_node * tree, FILE * out);
    friend kd_tree_node * load_tree(FILE * in);
    friend void print_tree(kd_tree_node * nod, int depth);
    friend class data_set;
};

#endif
