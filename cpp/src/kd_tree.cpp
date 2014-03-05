#include "kd_tree.h"

vector <int> kd_subdomain(euclid_vector * query, kd_tree_node * root)
{
    stack <kd_tree_node *> to_explore;
    to_explore.push(root);
    while (!to_explore.empty())
    {
        kd_tree_node * cur = to_explore.top();
        to_explore.pop();
        if (cur->get_left() && cur->get_right())
        {
            if ((*query)[cur->get_index()] <= cur->get_pivot())
                to_explore.push(cur->get_left());
            else
                to_explore.push(cur->get_right());
        }
        else
        {
            return cur->get_domain();
        }
    }
    return vector <int> ();
}

kd_tree_node * build_tree(int c, double a, 
        data_set & data, vector <int> subdomain)
{
    if (subdomain.size() < c)
        return new kd_tree_node(subdomain);
    data_set subset = data.subset(subdomain);
    int mx_var_index = max_variance_index(subset);
    vector <double> values;
    for (int i = 0; i < subset.size(); i++)
    {
        values.push_back((*subset[i])[mx_var_index]);
    }
    double pivot = selector(values, (int)(values.size() * (0.5)));
    double pivot_l, pivot_r;
    if (a > 0)
    {
        pivot_l = selector(values, (int)(values.size() * (0.5 - a)));
        pivot_r = selector(values, (int)(values.size() * (0.5 + a)));
    }
    vector <int> l_subdomain;
    vector <int> r_subdomain;
    for (int i = 0; i < subdomain.size(); i++)
    {
        if (a > 0 && pivot_l < values[i] && values[i] <= pivot_r)
        {
            l_subdomain.push_back(subdomain[i]);
            r_subdomain.push_back(subdomain[i]);
            continue;
        }
        if (values[i] <= pivot)
            l_subdomain.push_back(subdomain[i]);
        else
            r_subdomain.push_back(subdomain[i]);
    }
    kd_tree_node * result = new kd_tree_node(mx_var_index, pivot, subdomain);
    result->_left = build_tree(c, a, data, l_subdomain);
    result->_right = build_tree(c, a, data, r_subdomain);
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

void save_kd_tree(kd_tree_node * root, ofstream & out)
{
    stack <kd_tree_node *> to_build;
    to_build.push(root);
    while (!to_build.empty())
    {
        kd_tree_node * curr = to_build.top();
        to_build.pop();
        bool exists = curr != NULL;
        out.write((char *)&exists, sizeof(bool)); 
        if (exists)
        {
            out.write((char *)&curr->_index, sizeof(int)); 
            out.write((char *)&curr->_pivot, sizeof(double)); 
            size_t sz = curr->_domain.size();
            out.write((char *)&sz, sizeof(size_t)); 
            out.write((char *)&curr->_domain[0], sizeof(int) * curr->_domain.size());
            to_build.push(curr->_right);
            to_build.push(curr->_left);
        }
    }
}

kd_tree_node * load_kd_tree(ifstream & in)
{
    bool exist;
    in.read((char *)&exist, sizeof(bool));
    if (!exist)
    {
        return NULL;
    }
    kd_tree_node * res = new kd_tree_node();
    in.read((char *)&res->_index, sizeof(int));
    in.read((char *)&res->_pivot, sizeof(double));
    size_t sz;
    in.read((char *)&sz, sizeof(size_t));
    while (sz--)
    {
        int v;
        in.read((char *)&v, sizeof(int));
        res->_domain.push_back(v);
    }
    res->_left = load_kd_tree(in);
    res->_right = load_kd_tree(in);
    return res;
}

/* class definition */

kd_tree_node::kd_tree_node()
{
    _index = -1;
    _pivot = -1;
    _left = NULL;
    _right = NULL;
}

kd_tree_node::kd_tree_node(vector <int> subdomain)
{
    _index = -1;
    _pivot = -1;
    _domain = subdomain;
    _left = NULL;
    _right = NULL;
}

kd_tree_node::kd_tree_node(int index, double pivot, vector<int> subdomain)
{
    _index = index;
    _pivot = pivot;
    _domain = subdomain;
    _left = NULL;
    _right = NULL;
}

kd_tree_node::kd_tree_node(const kd_tree_node & copy)
{
    _index = copy.get_index();
    _pivot = copy.get_pivot();
    _domain = copy.get_domain();
    _left = copy.get_left();
    _right = copy.get_right();
}

kd_tree_node::~kd_tree_node()
{
    if (_left)
        delete _left;
    if (_right)
        delete _right;
}

int kd_tree_node::get_index() const
{
    return _index;
}

double kd_tree_node::get_pivot() const
{
    return _pivot;
}

vector <int> kd_tree_node::get_domain() const
{
    return _domain;
}

kd_tree_node * kd_tree_node::get_left() const
{
    return _left;
}

kd_tree_node * kd_tree_node::get_right() const
{
    return _right;
}

