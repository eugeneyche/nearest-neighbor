#ifndef _SPILL_TREE_H
#define _SPILL_TREE_H

#include "kd_tree.h"
using namespace std;

template<class Label, class T>
class SpillTree : public KDTree<Label, T>
{
    using KDTree<Label, T>::_root;
public:
    SpillTree(DataSet<Label, T> st);    
    SpillTree(size_t c, double a, DataSet<Label, T> st);    
};

template<class Label, class T>
static KDTreeNode<Label, T> * build_tree(size_t c, double a,
        DataSet<Label, T> & st, vector<size_t> domain)
{
    if (domain.size() < c)
        return new KDTreeNode<Label, T>(domain);
    DataSet<Label, T> subst = st.subset(domain);
    int mx_var_index = max_variance_index(subst);
    vector<T> values;
    for (int i = 0; i < subst.size(); i++)
    {
        values.push_back((*subst[i])[mx_var_index]);
    }
    double pivot = selector(values, (T)(values.size() * 0.5));
    double pivot_l, pivot_r;
    if (a > 0)
    {
        pivot_l = selector(values, (size_t)(values.size() * (0.5 - a)));
        pivot_r = selector(values, (size_t)(values.size() * (0.5 + a)));
    }
    else
    {
        pivot_l = pivot_r = pivot;
    }
    vector<size_t> subdomain_l;
    vector<size_t> subdomain_r;
    for (int i = 0; i < domain.size(); i++)
    {
        if (a > 0 && pivot_l < values[i] && values[i] <= pivot_r)
        {
            subdomain_l.push_back(domain[i]);
            subdomain_r.push_back(domain[i]);
            continue;
        }
        if (values[i] <= pivot)
            subdomain_l.push_back(domain[i]);
        else
            subdomain_r.push_back(domain[i]);
    }
    KDTreeNode<Label, T> * result = new KDTreeNode<Label, T>
            (mx_var_index, pivot, domain);
    result->_left = build_tree(c, a, st, subdomain_l);
    result->_right = build_tree(c, a, st, subdomain_r);
    return result;
}

template<class Label, class T>
SpillTree<Label, T>::SpillTree(DataSet<Label, T> st) :
  KDTree<Label, T> (st)
{ }

template<class Label, class T>
SpillTree<Label, T>::SpillTree(size_t c, double a, DataSet<Label, T> st) :
  KDTree<Label, T> (st)
{
    _root = build_tree(c, a, st, st.get_domain());
}
#endif
