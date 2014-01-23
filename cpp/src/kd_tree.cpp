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

kd_tree_node * build_tree(int c, double a, 
        data_set & data, vector <int> sub_domain)
{
    #ifdef DEBUG
    cerr << "[DEBUG: Building kd-tree of size " << sub_domain.size() << "]" << endl;
    #endif
    if (sub_domain.size() < c)
        return new kd_tree_node(sub_domain);
    data_set subset = data.subset(sub_domain);
    int mx_var_index = max_variance_index((int)sub_domain.size() / 2, subset);
    vector <double> values;
    for (int i = 0; i < sub_domain.size(); i++)
    {
        values.push_back((*subset[i])[mx_var_index]);
    }
    double pivot = selector(values, (int)(values.size() * (0.5)));
    double pivot_l = (a <= 0) ? selector(values, (int)(values.size() * (0.5 - a))) : pivot;
    double pivot_r = (a <= 0) ? selector(values, (int)(values.size() * (0.5 + a))) : pivot;
    vector <int> l_sub_domain;
    vector <int> r_sub_domain;
    for (int i = 0; i < sub_domain.size(); i++)
    {
        if (pivot_l < values[i] && values[i] <= pivot_r)
        {
            l_sub_domain.push_back(sub_domain[i]);
            r_sub_domain.push_back(sub_domain[i]);
            continue;
        }
        if (values[i] <= pivot)
            l_sub_domain.push_back(sub_domain[i]);
        else
            r_sub_domain.push_back(sub_domain[i]);
    }
    kd_tree_node * result = new kd_tree_node(mx_var_index, pivot, sub_domain);
    result->left = build_tree(c, a, data, l_sub_domain);
    result->right = build_tree(c, a, data, r_sub_domain);
    return result;
}

kd_tree_node * kd_tree(int c, data_set & data)
{
    return build_tree(c, 0, data, data.get_domain());
}

kd_tree_node * spill_tree(int c, double a, data_set & data)
{
    return build_tree(c, a, data, data.get_domain());
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
    if (left)
        delete left;
    if (right)
        delete right;
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

