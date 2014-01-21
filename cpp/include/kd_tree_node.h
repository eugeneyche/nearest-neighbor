#ifndef _KD_TREE_NODE_H
#define _KD_TREE_NODE_H

#include "data_set.h"

using namespace std;

class kd_tree_node;

void print_tree(kd_tree_node * m_node, int depth);

/* finds the kth smallest */
double selector(vector<double> s, int k);
/* return the index of the vector with the max variance */
int max_variance_index(int dimension, int k, int subsize, data_set & sub); 

/* build tree, called in kd_tree */
kd_tree_node * build_tree(int c, int i, int dimension, vector<int> domain, data_set & data); 
/* return root node */
kd_tree_node * kd_tree(int c, data_set & data, int size);
/* search leaf */
kd_tree_node search(euclid_vector & test, kd_tree_node node); 
/* gets the subdomain of a given subtree */
vector<int> sub_domain(euclid_vector * test, kd_tree_node root);

/* saves tree into file */
void save_tree(kd_tree_node * tree, FILE * out);
/* loads tree from file */
kd_tree_node * load_tree(FILE * in);

void print_tree(kd_tree_node * m_node, int depth);

class kd_tree_node
{
private:
    int index;
    double pivot;
    vector<int> sub_domain;
    kd_tree_node * left;
    kd_tree_node * right;
    
public:
    kd_tree_node();
    kd_tree_node(int d, double p);
    kd_tree_node(vector<int> domain);
    ~kd_tree_node();
    int get_index();
    double get_pivot();
    friend kd_tree_node * build_tree(int c, int i, int dimension, vector<int> domain, data_set & data);
    friend kd_tree_node search(euclid_vector & test, kd_tree_node node);
    friend vector<int> sub_domain(euclid_vector * test, kd_tree_node root);
    friend class data_set;
    friend void save_tree(kd_tree_node * tree, FILE * out);
    friend kd_tree_node * load_tree(FILE * in);
    friend void print_tree(kd_tree_node * nod, int depth);
};

#endif
