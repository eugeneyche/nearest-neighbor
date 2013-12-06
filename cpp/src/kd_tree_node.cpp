#include "kd_tree_node.h"
#include <random>

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


/* help function for building tree
 * 'i' is the index of the info for each data
 * which is equal to the depth of the tree
 * 'size' is the size of each data
 * 'domain' is the index of the data each leaf will point to
 */
kd_tree_node build_tree_help(int c, int i, int size, vector<int> domain, data_set &data)
{
    kd_tree_node internal_node = kd_tree_node(0,0);
    vector<int> left_domain;
    vector<int> right_domain;
    
    data_set sub = data.subset(domain);
    int subsize = sub.size();
    int k = (int)subsize / 2;
    euclid_vector vtr;
    for (int j=0; j<subsize; j++)
    {
        vector<double> h = *sub[j];
        vtr.push_back(h[i]);
    }
    double median;
    median = selector(vtr,k);

    for (int j=0; j < subsize; j++)
    {
        euclid_vector v = *sub[j];
        if(v[i] <= median)
            left_domain.push_back(j);
        else
            right_domain.push_back(j);
    }
    if (domain.size() > c)
    {
        srand(int(time(NULL))); //random seed
        double randomIndex = rand() % size; //random number between 0 and size-1
        *internal_node.left = build_tree_help(c, randomIndex, size, left_domain, data);
        *internal_node.right = build_tree_help(c, randomIndex, size, right_domain, data);
    }
    if (domain.size() == c)
    {
        *internal_node.left = kd_tree_node(left_domain);
        *internal_node.right = kd_tree_node(right_domain);
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
    srand(int(time(NULL))); //random seed
    double randomIndex = rand() % size; //random number between 0 and size-1
    kd_tree_node root = build_tree_help(c, randomIndex, size, domain, data);
    return root;
}


/* search for the data_set*/
kd_tree_node search(euclid_vector & test, kd_tree_node node)
{
    int i = node.get_index();
    if (test[i] > node.get_pivot() && i<test.size())
        return search(test, *node.right);
    else if (test[i] <= node.get_pivot() && i<test.size())
        return search(test, *node.left);
    return node;

}

vector<int> sub_domain(euclid_vector * test, kd_tree_node root)
{
    kd_tree_node leaf = search(*test,root);
    return *leaf.sub;
}


/******* class declaration below *******/

kd_tree_node::kd_tree_node(vector<int> domain)//leaf node
{
    index = 0;
    pivot = 0.0;
    *sub = domain;
    left = NULL;
    right = NULL;
}

kd_tree_node::kd_tree_node(int d, double p)//internal node
{
    index = d;
    pivot = p;
    sub = NULL;
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
