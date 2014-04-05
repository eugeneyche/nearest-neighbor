#ifndef _SPILL_TREE_H
#define _SPILL_TREE_H

#include "kd_tree.h"

template<class Label, class T>
class SpillTree;

template<class Label, class T>
class SpillTree : public KDTree<Label, T>
{
private:
    static KDTreeNode<Label, T> * build_tree(size_t c, double a,
            DataSet<Label, T> & st, vector<size_t> domain);
public:
    SpillTree(DataSet<Label, T> & st);
    SpillTree(size_t c, double a, DataSet<Label, T> & st);
    SpillTree(ifstream & in, DataSet<Label, T> & st);
};

template<class Label, class T>
KDTreeNode<Label, T> * SpillTree<Label, T>::build_tree(size_t c, double a,
        DataSet<Label, T> & st, vector<size_t> domain)
{
    #ifdef DEBUG
        cerr << "[DEBUG: Building tree of size " << domain.size() << "]" << endl;
    #endif
    if (domain.size() < c)
        return new KDTreeNode<Label, T>(domain);
    DataSet<Label, T> subst = st.subset(domain);
    size_t mx_var_index = max_variance_index(subst);
    vector<T> values;
    for (size_t i = 0; i < subst.size(); i++)
    {
        values.push_back((*subst[i])[mx_var_index]);
    }
    T pivot = selector(values, (size_t)(values.size() * 0.5));
    T pivot_l = selector(values, (size_t)(values.size() * (0.5 - a)));
    T pivot_r = selector(values, (size_t)(values.size() * (0.5 + a)));
    vector<size_t> subdomain_l;
    size_t subdomain_l_lim = (size_t)(values.size() * (0.5 + a));
    vector<size_t> subdomain_r;
    for (size_t i = 0; i < domain.size(); i++)
    {
        if (pivot_l <= values[i] && values[i] <= pivot_r)
        {
            if (subdomain_l.size() < subdomain_l_lim)
                subdomain_l.push_back(domain[i]);
            subdomain_r.push_back(domain[i]);
            continue;
        }
        if (subdomain_l.size() < subdomain_l_lim && 
                values[i] <= pivot)
            subdomain_l.push_back(domain[i]);
        else
            subdomain_r.push_back(domain[i]);
    }
    KDTreeNode<Label, T> * result = new KDTreeNode<Label, T>
            (mx_var_index, pivot, domain);
    result->set_left(build_tree(c, a, st, subdomain_l));
    result->set_right(build_tree(c, a, st, subdomain_r));
    return result;
}
template<class Label, class T>
SpillTree<Label, T>::SpillTree(DataSet<Label, T> & st) :
  KDTree<Label, T>(st)
{ }

template<class Label, class T>
SpillTree<Label, T>::SpillTree(size_t c, double a, DataSet<Label, T> & st) :
  KDTree<Label, T>(st)
{ this->set_root(build_tree(c, a, st, st.get_domain())); }

template<class Label, class T>
SpillTree<Label, T>::SpillTree(ifstream & in, DataSet<Label, T> & st) :
  KDTree<Label, T>(in, st)
{ }

#endif
