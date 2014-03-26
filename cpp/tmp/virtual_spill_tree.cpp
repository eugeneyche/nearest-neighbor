#include "virtual_spill_tree.h"

#define UNDEF (-1)
#define HALF  (0.5)

template <class Label, class T>
static VirtualSpillTreeNode<Label, T> * build_tree(size_t c, double a, 
        DataSet<Label, T> & st, vector<size_t> domain)
{
    if (domain.size() < c)
        return new VirtualSpillTreeNode<Label, T>(domain);
    DataSet<Label, T> subst = st.subset(domain);
    int mx_var_index = max_variance_index(subst);
    vector<T> values;
    for (int i = 0; i < subst.size(); i++)
    {
        values.push_back((*subst[i])[mx_var_index]);
    }
    double pivot = selector(values, (T)(values.size() * (HALF)));
    double pivot_l, pivot_r;
    if (a > 0)
    {
        pivot_l = selector(values, (size_t)(values.size() * (HALF - a)));
        pivot_r = selector(values, (size_t)(values.size() * (HALF + a)));
    }
    else
    {
        pivot_l = pivot_r = pivot;
    }
    vector<size_t> subdomain_l;
    vector<size_t> subdomain_r;
    for (int i = 0; i < domain.size(); i++)
    {
        if (values[i] <= pivot)
            subdomain_l.push_back(domain[i]);
        else
            subdomain_r.push_back(domain[i]);
    }
    VirtualSpillTreeNode<Label, T> * result = 
            new VirtualSpillTreeNode<Label, T>(mx_var_index, pivot, 
            pivot_l, pivot_r, domain);
    result->_left = build_tree(c, a, st, subdomain_l);
    result->_right = build_tree(c, a, st, subdomain_r);
    return result;
}

template <class Label, class T>
VirtualSpillTree<Label, T>::VirtualSpillTree(size_t c, 
        double a, DataSet<Label, T> & st) :
  _root (build_tree(c, a, st, st.domain())),
  _st (st)
{ }

template <class Label, class T>
VirtualSpillTree<Label, T>::VirtualSpillTree(istream & in, 
        DataSet<Label, T> & st) :
  _st (st)
{
    queue<VirtualSpillTreeNode<Label, T> **> to_load;
    to_load.push(_root);
    while (!to_load.empty())
    {
        VirtualSpillTree<Label, T> ** cur = to_load.front();
        bool exist;
        in.read((char *)&exist, sizeof(bool));
        if (!exist)
        {
            *cur = NULL; 
        }
        *cur = new VirtualSpillTreeNode<Label, T>;
        in.read((char *)&(*cur)->_index, sizeof(T));
        in.read((char *)&(*cur)->_pivot, sizeof(double));
        in.read((char *)&(*cur)->_pivot_l, sizeof(double));
        in.read((char *)&(*cur)->_pivot_r, sizeof(double));
        size_t sz;
        in.read((char *)&sz, sizeof(size_t));
        while (sz--)
        {
            size_t v;
            in.read((char *)&v, sizeof(size_t));
            (*cur)->_domain.push_back(v);
        }
        to_load.push(&((*cur)->left));
        to_load.push(&((*cur)->right));
    }
}
