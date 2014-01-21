#ifndef _KD_TREE_H
#define _KD_TREE_H

#include "data_set.h"
#include <stack>
using namespace std;

class kd_tree;

void print_tree(kd_tree * m_node, int depth);

/* build tree, called in kd_tree */
kd_tree * build_tree(int c, int i, int dimension, vector<int> domain, data_set & data); 
/* return root node */
kd_tree * kd_tree_root(int c, data_set & data, int size);
/* search leaf */
kd_tree * search(euclid_vector * test, kd_tree * node);
/* gets the subdomain of a given subtree */
vector<int> sub_domain(euclid_vector * test, kd_tree * root);

/* saves tree into file */
void save_tree(kd_tree * tree, FILE * out);
/* loads tree from file */
kd_tree * load_tree(FILE * in);

void print_tree(kd_tree * m_node, int depth);

class kd_tree
{
private:
    int index;
    double pivot;
    vector<int> sub_domain;
    kd_tree * left;
    kd_tree * right;
    
public:
    kd_tree();
    kd_tree(int d, double p);
    kd_tree(vector <int> domain);
    ~kd_tree();
    int get_index();
    double get_pivot();
    friend kd_tree * build_tree(int c, int i, int dimension, vector<int> domain, data_set & data);
    friend kd_tree search(euclid_vector & test, kd_tree node);
    friend kd_tree * search(euclid_vector * test, kd_tree * node);
    friend vector<int> sub_domain(euclid_vector * test, kd_tree * root);
    friend class data_set;
    friend void save_tree(kd_tree * tree, FILE * out);
    friend kd_tree * load_tree(FILE * in);
    friend void print_tree(kd_tree * nod, int depth);
};

#endif
