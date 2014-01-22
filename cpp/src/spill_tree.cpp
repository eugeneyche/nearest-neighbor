#include "spill_tree.h"


spill_tree_node * build_query_tree(data_set & train, kd_tree_node * root, vector <int> sub_domain, int a)
{
    int index = root->get_index();
    int pivot = root->get_pivot();
    if (root == NULL)
        return new spill_tree_node(sub_domain);
    data_set subset = train.subset(sub_domain);
    vector <double> values;
    for (int i = 0; i < sub_domain.size(); i++)
    {
        values.push_back((*subset[i])[index]);
    }
    double pivot_l = selector(values, (int)(values.size() * (0.5 - a)));
    double pivot_r = selector(values, (int)(values.size() * (0.5 + a)));
    vector <int> l_sub_domain;
    vector <int> r_sub_domain;
    for (int i = 0; i < sub_domain.size(); i++)
    {
        if (values[i] <= pivot)
            l_sub_domain.push_back(sub_domain[i]);
        else
            r_sub_domain.push_back(sub_domain[i]);
    }
    spill_tree_node * result = new spill_tree_node(index, pivot_l, pivot, pivot_r);
    result->left = build_query_tree(train, root->get_left(), l_sub_domain, a);
    result->right = build_query_tree(train, root->get_right(), r_sub_domain, a);
    return result;

}



/*return a set of nearest neighbor generated from kd_tree*/
vector_space query_nn_set(vector_space vector_set, data_set * train_set, euclid_vector * test, spill_tree_node * spill_query_root)
{
    if (spill_query_root->get_left() == NULL && spill_query_root->get_right() == NULL)
    {
        data_set train_leaf = train_set->subset(spill_query_root->get_domain());
        vector_set.push_back(nn(test, train_leaf));
        return vector_set;
    }
    int index = spill_query_root->get_index();
    if (spill_query_root->in_range(test))
    {
        vector_set = query_nn_set(vector_set, train_set, test, spill_query_root->get_left());
        vector_set = query_nn_set(vector_set, train_set, test, spill_query_root->get_right());
        return vector_set;
    }
    else if ((*test)[index] < spill_query_root->get_pivot())
    {
        return query_nn_set(vector_set, train_set, test, spill_query_root->get_left());
    }
    else if ((*test)[index] > spill_query_root->get_pivot())
    {
        return query_nn_set(vector_set, train_set, test, spill_query_root->get_right());
    }
    return vector_set;
}


/*spill_tree_node * spill_tree(int c, double a, int i,
        int dimension, vector<int> domain, data_set & data)
{
    spill_tree_node * result; 
    if (domain.size() > c)
    {
        
    }
    return result;
}*/

spill_tree_node * spill_tree(int c, double a,
        data_set & data)
{
    return build_tree(c, a, data, data.get_domain());
}

spill_tree_node * build_tree(int c, double a, 
        data_set & data, vector <int> sub_domain)
{
    if (sub_domain.size() < c)
        return new spill_tree_node(sub_domain);
    data_set subset = data.subset(sub_domain);
    int mx_variance = max_variance_index(sub_domain.size() / 2, subset);
    vector <double> values;
    for (int i = 0; i < sub_domain.size(); i++)
    {
        values.push_back((*subset[i])[mx_variance]);
    }
    double pivot = selector(values, (int)(values.size() * (0.5)));
    double pivot_l = selector(values, (int)(values.size() * (0.5 - a)));
    double pivot_r = selector(values, (int)(values.size() * (0.5 + a)));
    vector <int> l_sub_domain;
    vector <int> r_sub_domain;
    for (int i = 0; i < sub_domain.size(); i++)
    {
        if (values[i] <= pivot)
            l_sub_domain.push_back(sub_domain[i]);
        else
            r_sub_domain.push_back(sub_domain[i]);
    }
    spill_tree_node * result = new spill_tree_node(mx_variance, pivot_l, pivot, pivot_r);
    result->left = build_tree(c, a, data, l_sub_domain);
    result->right = build_tree(c, a, data, r_sub_domain);
    return result;
}

/* class implementation */

spill_tree_node::spill_tree_node()
{
    index = -1;
    pivot_l = pivot = pivot_r = -1;
    left = right = NULL;
}

spill_tree_node::spill_tree_node(int index, double pivot_l, double pivot, double pivot_r)
{
    this->index = index;
    this->pivot_l = pivot_l;
    this->pivot = pivot;
    this->pivot_r = pivot_r;
    left = right = NULL;
}

spill_tree_node::spill_tree_node(vector <int> sub_domain)
{
    index = -1;
    pivot_l = pivot = pivot_r = -1;
    this->sub_domain = sub_domain;
}

spill_tree_node::~spill_tree_node()
{
    if (left)
        delete left;
    if (right)
        delete right;
}

int spill_tree_node::get_index()
{
    return index;
}

bool spill_tree_node::in_range(euclid_vector * query)
{
    return (pivot_l <= (*query)[index] && (*query)[index] <= pivot_r);
}

double spill_tree_node::get_pivot()
{
    return pivot;
}

spill_tree_node * spill_tree_node::get_left()
{
    return left;
}

spill_tree_node * spill_tree_node::get_right()
{
    return right;
}

vector <int> spill_tree_node::get_domain()
{
    return sub_domain;
}
