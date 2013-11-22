#ifndef _KD_TREE_NODE_H
#define _KD_TREE_NODE_H

#include "data_set.h"

using namespace std;

class kd_tree_node;

double selector(vector<double> s, int k); //select the kth smallest
vector <double> medians(data_set data); //return all the medians
kd_tree_node build_tree(int i, int size, vector<int> domain, vector<double> medians, data_set data);//help function
kd_tree_node kd_tree(data_set data); // return root node
kd_tree_node search(vector<double> test, kd_tree_node node);
data_set domain(vector<double> test, kd_tree_node root);



class kd_tree_node : public data_set
{
private:
    int depth;
    double pivot;
    data_set * sub;
    kd_tree_node * left;
    kd_tree_node * right;
    
public:
    kd_tree_node(int d, double p);
    kd_tree_node(vector<int> domain);
    ~kd_tree_node();
    int get_depth();
    double get_pivot();
    friend kd_tree_node build_tree(int i, int size, vector<int> domain, vector<double> medians, data_set data);
    friend kd_tree_node search(vector<double> test, kd_tree_node node);
    friend data_set domain(vector<double> test, kd_tree_node root);
    friend class data_set;
};

#endif
