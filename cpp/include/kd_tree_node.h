#ifndef _KD_TREE_NODE_H
#define _KD_TREE_NODE_H

#include "data_set.h"

using namespace std;

class kd_tree_node;

double selector(vector<double> s, int k); //select the kth smallest
int max_variance_index(int dimension, int k, int subsize, data_set & sub); //return the index of the vector with the max variance
kd_tree_node * build_tree(int c, int i, int dimension, vector<int> domain, data_set & data); //build tree, called in kd_tree
kd_tree_node * kd_tree(int c, data_set & data); //return root node
kd_tree_node search(euclid_vector & test, kd_tree_node node); //search leaf
vector<int> sub_domain(euclid_vector * test, kd_tree_node root);
void save_tree(kd_tree_node * tree, FILE * out);
kd_tree_node * load_tree(FILE * in);

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
};

#endif
