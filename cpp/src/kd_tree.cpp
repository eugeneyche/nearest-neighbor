#include "kd_tree.h"

void print_tree(kd_tree_node * m_node, int depth)
{
    if (m_node == NULL)
        return;
    for (int i = 0; i < depth; i++)
    {
        cout << " ";
    }
    cout << m_node->index << " : ";
    cout << m_node->domain.size();
    cout << endl;
    print_tree(m_node->left, depth + 1); 
    print_tree(m_node->right, depth + 1);
}

/* 'c' is the size of the leaf
 * 'i' is the index of the info for each data
 * 'dimension' is the # of entries for each vector
 * 'domain' is the index of the data each leaf will point to
 */
kd_tree_node * build_tree(int c, int i, int dimension, vector<int> domain, data_set & data)
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
    
    kd_tree_node * internal_node = new kd_tree_node(i,median, domain);

    for (int j=0; j < subsize; j++)
    {
        euclid_vector v = *sub[j];
        if(v[i] <= median)
            left_domain.push_back(domain[j]);
        else
            right_domain.push_back(domain[j]);
    }
    
    int index = max_variance_index(k, sub);
    if (left_domain.size() > c){
        kd_tree_node * left_int = build_tree(c, index, dimension, left_domain, data);
        internal_node->left = left_int;
    }
    else{
        kd_tree_node * left_leaf = new kd_tree_node(left_domain);
        internal_node->left = left_leaf;
    }
    if (right_domain.size() > c){
        kd_tree_node * right_int = build_tree(c, index, dimension, right_domain, data);
        internal_node->right = right_int;
    }
    else{
        kd_tree_node * right_leaf = new kd_tree_node(right_domain);
        internal_node->right = right_leaf;
    }
    return internal_node;
}


/* build kd_tree
 * return the root
 */
kd_tree_node * kd_tree_root(int c, data_set & data, int size)
{
    vector<int> domain;
    for (int i = 0; i < size; i++)
    {
        domain.push_back(i);
    }
    int dimension = (int)data[0]->size();
    int k = (int)size/2;
    int index = max_variance_index(k, data);
    kd_tree_node * root = build_tree(c, index, dimension, domain, data);
    return root;
}


/* saves the tree */
void save_tree(kd_tree_node * tree, FILE * out)
{
    stack <kd_tree_node *> to_build;
    to_build.push(tree);
    while (!to_build.empty())
    {
        kd_tree_node * curr = to_build.top();
        to_build.pop();
        bool exists = curr != NULL;
        fwrite(&exists, sizeof(bool), 1, out); 
        if (exists)
        {
            fwrite(&curr->index, sizeof(int), 1, out); 
            fwrite(&curr->pivot, sizeof(double), 1, out); 
            size_t sz = curr->domain.size();
            fwrite(&sz, sizeof(size_t), 1, out); 
            fwrite(&curr->domain[0], sizeof(int), curr->domain.size(), out);
            to_build.push(curr->right);
            to_build.push(curr->left);
        }
    }
}


/* loads the tree */
kd_tree_node * load_tree(FILE * in)
{
    bool exist;
    fread(&exist, sizeof(bool), 1, in);
    if (!exist)
    {
        return NULL;
    }
    kd_tree_node * res = new kd_tree_node();
    fread(&res->index, sizeof(int), 1, in);
    fread(&res->pivot, sizeof(double), 1, in);
    size_t sz;
    fread(&sz, sizeof(size_t), 1, in);
    while (sz--)
    {
        int v;
        fread(&v, sizeof(int), 1, in);
        res->domain.push_back(v);
    }
    res->left = load_tree(in);
    res->right = load_tree(in);
    return res;
}

/* search for the data_set*/
kd_tree_node * search(euclid_vector * query, kd_tree_node * node)
{
    if (node->left || node->right)
    {
        int i = node->get_index();
        if ((*query)[i] > node->get_pivot() && node->right != NULL)
            return search(query, node->right);
        else if ((*query)[i] <= node->get_pivot() && node->left != NULL)
            return search(query, node->left);
    }
    return node;
}

/* search for the data_set*/
vector<int> sub_domain(euclid_vector * test, kd_tree_node * root)
{
    kd_tree_node * leaf = search(test, root);
    return leaf->domain;
}


/******* class declaration below *******/
kd_tree_node::kd_tree_node()//leaf node
{
    index = -1;
    pivot = -1;
    left = NULL;
    right = NULL;
}

kd_tree_node::kd_tree_node(vector<int> sub_domain)//leaf node
{
    index = -1;
    pivot = -1;
    domain = sub_domain;
    left = NULL;
    right = NULL;
}

kd_tree_node::kd_tree_node(int d, double p, vector<int> sub_domain)//internal node
{
    index = d;
    pivot = p;
    domain = sub_domain;
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

vector<int> kd_tree_node::get_domain()
{
    return domain;
}

kd_tree_node * kd_tree_node::get_left()
{
    return left;
}

kd_tree_node * kd_tree_node::get_right()
{
    return right;
}

