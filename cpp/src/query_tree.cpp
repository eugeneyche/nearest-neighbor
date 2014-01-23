#include "query_tree.h"


query_tree_node * build_query_tree(data_set & train, kd_tree_node * root, vector <int> sub_domain, int a)
{
    int index = root->get_index();
    int pivot = root->get_pivot();
    if (root == NULL)
        return new query_tree_node(sub_domain);
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
    query_tree_node * result = new query_tree_node(index, pivot_l, pivot, pivot_r);
    result->left = build_query_tree(train, root->get_left(), l_sub_domain, a);
    result->right = build_query_tree(train, root->get_right(), r_sub_domain, a);
    return result;

}

/*return a set of nearest neighbor generated from kd_tree*/
vector <int> query_nn_set(vector <int> domain, data_set * train_set, euclid_vector * test, query_tree_node * query_root)
{
    if (query_root->get_left() == NULL && query_root->get_right() == NULL)
    {
        vector <int> leaf_domian = query_root->get_domain();
        for(int i = 0; i < leaf_domian.size(); i++)
            domain.push_back(leaf_domian[i]);
        return domain;
    }
    int index = query_root->get_index();
    if (query_root->in_range(test))
    {
        domain = query_nn_set(domain, train_set, test, query_root->get_left());
        domain = query_nn_set(domain, train_set, test, query_root->get_right());
        return domain;
    }
    else if ((*test)[index] < query_root->get_pivot())
    {
        return query_nn_set(domain, train_set, test, query_root->get_left());
    }
    else if ((*test)[index] > query_root->get_pivot())
    {
        return query_nn_set(domain, train_set, test, query_root->get_right());
    }
    return domain;
}

/* class implementation */

query_tree_node::query_tree_node()
{
    index = -1;
    pivot_l = pivot = pivot_r = -1;
    left = right = NULL;
}

query_tree_node::query_tree_node(int index, double pivot_l, double pivot, double pivot_r)
{
    this->index = index;
    this->pivot_l = pivot_l;
    this->pivot = pivot;
    this->pivot_r = pivot_r;
    left = right = NULL;
}

query_tree_node::query_tree_node(vector <int> sub_domain)
{
    index = -1;
    pivot_l = pivot = pivot_r = -1;
    this->sub_domain = sub_domain;
}

query_tree_node::~query_tree_node()
{
    if (left)
        delete left;
    if (right)
        delete right;
}

int query_tree_node::get_index()
{
    return index;
}

bool query_tree_node::in_range(euclid_vector * query)
{
    return (pivot_l <= (*query)[index] && (*query)[index] <= pivot_r);
}

double query_tree_node::get_pivot()
{
    return pivot;
}

query_tree_node * query_tree_node::get_left()
{
    return left;
}

query_tree_node * query_tree_node::get_right()
{
    return right;
}

vector <int> query_tree_node::get_domain()
{
    return sub_domain;
}
