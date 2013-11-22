#include "kd_tree_node.h"



/* select the kth smallest value in the vector */
double selector(vector<double> s, int k)
{
	srand(time(NULL)); //random seed
	int size = s.size();
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
	if (left.size() >= k){// the kth smallest on the left
		return selector(left, k);
	}
	else if(left.size() < k && left.size()+v.size() >= k){ // the kth smallest found
		return s[randomIndex];
	}
	else{ // the kth smallest on the right
		return selector(right, int(k-left.size()-v.size()));
	}
    return 0;
}


/*get all the median from the data_set*/
vector <double> medians(data_set data)
{
    vector <double> medi;
    int size = data.size();
    int k = size / 2;
    vector <double> data1 = *data[0];
    for( int i=0; i< int(data1.size()); i++)
    {
        vector <double> vtr;
        for (int j=0; j<size; j++)
        {
            vector<double> h = *data[j];
            vtr.push_back(h[i]);
        }
        medi.push_back(selector(vtr,k));
    }
    return medi;
}


/*recursive function for building tree
 * 'i' is the index of the info for each data
 * which is equal to the depth of the tree
 * 'size' is the size of each data
 * 'domain' is the index of the data each leaf will point to
 */
kd_tree_node build_tree(int i, int size, vector<int> domain, vector<double> medians, data_set data)
{
    kd_tree_node internal_node = *new kd_tree_node(0,0);
    vector<int> left_domain;
    vector<int> right_domain;
    data_set sub = data.subset(domain);
    for (int j=0; j < sub.size(); j++)
    {
        vector<double> v = *sub[j];
        if(v[i] <= medians[i])
            left_domain.push_back(j);
        else
            right_domain.push_back(j);
    }
    if (i < size-1)
    {
        *internal_node.left = build_tree(i+1, size, left_domain, medians, data);
        *internal_node.right = build_tree(i+1, size, right_domain, medians, data);
    }
    if (i == size-1)
    {
        *internal_node.left = *new kd_tree_node(left_domain);
        *internal_node.right = *new kd_tree_node(right_domain);
    }
    return internal_node;
}


/*build the kd_tree*/
kd_tree_node kd_tree(data_set data)
{
    int size = data.size();
    vector <double> median = medians(data);
    vector<int> domain;
    for (int i=0; i<size; i++)
    {
        domain.push_back(i);
    }
    kd_tree_node root = build_tree(0, size, domain, median, data);
    return root;
}


/* search for the data_set*/
kd_tree_node search(vector<double> test, kd_tree_node node)
{
    int i = node.get_depth();
    if (test[i] > node.get_pivot() && i<test.size())
        return search(test, *node.right);
    else if (test[i] <= node.get_pivot() && i<test.size())
        return search(test, *node.left);
    return node;

}

data_set domain(vector<double> test, kd_tree_node root)
{
    kd_tree_node leaf = search(test,root);
    return *leaf.sub;
}


/******* class declaration below *******/

kd_tree_node::kd_tree_node(vector<int> domain)//leaf node
{
    depth = 0;
    pivot = 0.0;
    *sub = subset(domain);
    left = NULL;
    right = NULL;
}

kd_tree_node::kd_tree_node(int d, double p)//internal node
{
    depth = d;
    pivot = p;
    sub = NULL;
    left = NULL;
    right = NULL;
}

kd_tree_node::~kd_tree_node()
{
    //destructor
}

int kd_tree_node::get_depth(){
    return depth;
}

double kd_tree_node::get_pivot()
{
    return pivot;
}
