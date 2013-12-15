#include "kd_tree_node.h"
#include <random>
#include <iostream>

/* select the kth smallest value in the vector */
double selector(vector<double> s, int k)
{
	srand(int(time(NULL))); //random seed
	int size = int(s.size());
	double randomIndex = rand() % size; //random number between 0 and size-1
    
	vector<double> left; //data that are smaller than v
	vector<double> right; // data that are larger than v
	vector<double> v; //data that are equal to v
    
    /* iterate through to push the smaller to left, larger to right, the rest to v */
	for (vector <double>::iterator itr = s.begin(); itr != s.end(); itr++)
	{
		if(*itr == s[randomIndex]){
			v.push_back(*itr);
		}
		else if(*itr < s[randomIndex]){
			left.push_back(*itr);
		}
		else{
			right.push_back(*itr);
		}
	}
    
    /* three conditions */
	if (left.size() >= k)
    {// the kth smallest on the left
		return selector(left, k);
	}
	else if(left.size()+v.size() >= k)
    { // the kth smallest found
		return s[randomIndex];
	}
	else
    { // the kth smallest on the right
		return selector(right, int (k-left.size()-v.size()));
	}
}


/* variance function
 * 'dimension' is the size of each vector
 * 'k' would be the kth smallest number from selector()
 * 'subsize' is the size of data_set sub
 */
int max_variance_index(int dimension, int k, int subsize, data_set & sub){
    vector <double> var;
    vector <double> vtr;
    for (int i = 0; i<dimension; i++){
        for (int j = 0; j<subsize; j++){
            vtr.push_back((*sub[j])[i]);
        }
        double mean = selector(vtr, k);
        double variance = 0.0;
        for (int j = 0; j<subsize; j++){
            vtr.push_back((*sub[j])[i]);
            variance += ((*sub[j])[i]-mean)*((*sub[j])[i]-mean)/subsize;
        }
        var.push_back(variance);
    }
    int minIndex = 0;
    for (int i = 1; i<dimension; i++){
        if (var[i] < minIndex)
            minIndex = i;
    }
    return minIndex;
}



/* help function for building tree
 * 'c' is the size of the leaf
 * 'i' is the index of the info for each data
 * 'dimension' is the # of entries for each vector
 * 'domain' is the index of the data each leaf will point to
 */
kd_tree_node build_tree_help(int c, int i, int dimension, vector<int> domain, data_set & data)
{
    vector<int> left_domain;
    vector<int> right_domain;
    
    data_set sub = data.subset(domain);
    int subsize = sub.size();
    int k = (int)subsize / 2;
    vector <double> vtr;
    for (int j=0; j<subsize; j++)
    {
        vtr.push_back((*sub[j])[i]);
    }
    double median;
    median = selector(vtr,k);
    
    kd_tree_node internal_node = kd_tree_node(i,median);

    for (int j=0; j < subsize; j++)
    {
        euclid_vector v = *sub[j];
        if(v[i] <= median)
            left_domain.push_back(j);
        else
            right_domain.push_back(j);
    }
    
    int index = max_variance_index(dimension, k, subsize, sub);
    if (left_domain.size() > c){
        kd_tree_node left_int = build_tree_help(c, index, dimension, left_domain, data);
        internal_node.left = & left_int;
    }
    else{
        kd_tree_node left_leaf = kd_tree_node(left_domain);
        internal_node.left = &left_leaf;
    }
    if (right_domain.size() > c){
        kd_tree_node right_int = build_tree_help(c, index, dimension, right_domain, data);
        internal_node.right = & right_int;
    }
    else{
        kd_tree_node right_leaf = kd_tree_node(right_domain);
        internal_node.right = &right_leaf;
    }
    return internal_node;
}


/* build kd_tree
 * return the root
 */
kd_tree_node kd_tree(int c, data_set &data)
{
    int size = data.size();
    vector<int> domain;
    for (int i = 0; i < size; i++)
    {
        domain.push_back(i);
    }
    int dimension = (int)data[0]->size();
    int k = (int)size/2;
    int index = max_variance_index(dimension, k, size, data);
    kd_tree_node root = build_tree_help(c, index, dimension, domain, data);
    return root;
}


/* search for the data_set*/
kd_tree_node search(euclid_vector & test, kd_tree_node node)
{
    int i = node.get_index();
    if (test[i] > node.get_pivot() && node.right != NULL)
        return search(test, *node.right);
    else if (test[i] <= node.get_pivot() && node.left != NULL)
        return search(test, *node.left);
    return node;

}

vector<int> sub_domain(euclid_vector * test, kd_tree_node root)
{
    kd_tree_node leaf = search(*test,root);
    return leaf.sub_domain;
}


/******* class declaration below *******/

kd_tree_node::kd_tree_node(vector<int> domain)//leaf node
{
    index = -1;
    pivot = -1;
    sub_domain = domain;
    left = NULL;
    right = NULL;
}

kd_tree_node::kd_tree_node(int d, double p)//internal node
{
    index = d;
    pivot = p;
    left = NULL;
    right = NULL;
}

kd_tree_node::~kd_tree_node()
{
    //destructor
}

int kd_tree_node::get_index(){
    return index;
}

double kd_tree_node::get_pivot()
{
    return pivot;
}
