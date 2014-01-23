#include "query_tree.h"

vector <int> query_subdomain(euclid_vector * query, query_tree_node * root)
{
    stack <query_tree_node *> to_explore;
    set <int> domain_st;
    to_explore.push(root);
    while (!to_explore.empty())
    {
        query_tree_node * cur = to_explore.top();
        to_explore.pop();
        if (cur->get_left() && cur->get_right())
        {
            if (cur->in_range(query))
            {
                to_explore.push(cur->get_right());
                to_explore.push(cur->get_left());
                continue;
            }
            if ((*query)[cur->get_index()] <= cur->get_pivot())
                to_explore.push(cur->get_left());
            else
                to_explore.push(cur->get_right());
        }
        else
        {
            vector <int> l_domain = cur->get_domain();
            for (int i = 0; i < l_domain.size(); i++)
            {
                domain_st.insert(l_domain[i]);
            }
        }
    }
    set <int>::iterator st_i;
    vector <int> domain;
    for (st_i = domain_st.begin(); st_i != domain_st.end(); st_i++)
    {
        domain.push_back(*st_i);
    }
    return domain;
}

query_tree_node * build_tree(double a, kd_tree_node * root, data_set & data)
{
    if (root == NULL)
        return NULL;
    if (root->get_left() && root->get_right())
    {
        vector <double> values;
        vector <int> subdomain = root->get_domain();
        int mx_var_index = root->get_index();
        double pivot_l, pivot_r;
        for (int i = 0; i < subdomain.size(); i++)
        {
            values.push_back((*data[subdomain[i]])[mx_var_index]);
        }
        pivot_l = selector(values, (int)(values.size() * (0.5 - a)));
        pivot_r = selector(values, (int)(values.size() * (0.5 + a)));

        query_tree_node * res = new query_tree_node(root, pivot_l, pivot_r);
        res->_left = build_tree(a, root->get_left(), data);
        res->_right = build_tree(a, root->get_right(), data);
        return res;
    }
    return new query_tree_node(root);
}

query_tree_node * query_tree(double a, kd_tree_node * root, data_set & data)
{
    return build_tree(a, root, data);
}

/* class definition */

query_tree_node::query_tree_node()
{
    _kd_node = NULL;
    _pivot_l = _pivot_r = -1;
    _left = _right = NULL;
}

query_tree_node::query_tree_node(kd_tree_node * kd_node)
{
    _kd_node = kd_node;
    _pivot_l = _pivot_r = -1;
    _left = _right = NULL;
}

query_tree_node::query_tree_node(kd_tree_node * kd_node, double pivot_l, double pivot_r)
{
    _kd_node = kd_node;
    _pivot_l = pivot_l;
    _pivot_r = pivot_r;
}

query_tree_node::~query_tree_node()
{
    if (left)
        delete _left;
    if (right)
        delete _right;
}

int query_tree_node::get_index()
{
    return _kd_node->get_index();
}

bool query_tree_node::in_range(euclid_vector * query)
{
    return (_pivot_l <= (*query)[get_index()] && (*query)[get_index()] <= _pivot_r);
}

double query_tree_node::get_pivot()
{
    return _kd_node->get_pivot();
}

query_tree_node * query_tree_node::get_left()
{
    return _left;
}

query_tree_node * query_tree_node::get_right()
{
    return _right;
}

vector <int> query_tree_node::get_domain()
{
    return _kd_node->get_domain();
}
