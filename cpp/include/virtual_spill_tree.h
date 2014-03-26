#ifndef _VIRTUAL_SPILL_TREE_H
#define _VIRTUAL_SPILL_TREE_H

#include "kd_tree.h"
#include <map>
#include <set>
#include <utility>
using namespace std;

template <class Label, class T>
class VirtualSpillTree : public KDTree<Label, T>
{
    using KDTree<Label, T>::_root;
    using KDTree<Label, T>::_st;
    typedef pair<T, T> range;
protected:
    map<KDTreeNode<Label, T> *, range> _range_mp;
public:
    VirtualSpillTree(DataSet<Label, T> & st);
    VirtualSpillTree(size_t c, double a, DataSet<Label, T> & st);
    VirtualSpillTree(ifstream & in, DataSet<Label, T> & st);
    virtual void save(ofstream & out) const;
    virtual vector<size_t> subdomain(vector<T> * query);
};

template<class Label, class T>
VirtualSpillTree<Label, T>::VirtualSpillTree
        (DataSet<Label, T> &st) :
  KDTree<Label, T>(st)
{ }

template<class Label, class T> 
VirtualSpillTree<Label, T>::VirtualSpillTree
        (size_t c, double a, DataSet<Label, T> & st) :
  KDTree<Label, T>(c, st)
{ 
    queue<KDTreeNode<Label, T> *> to_load;
    to_load.push(_root);
    while (!to_load.empty())
    {
        KDTreeNode<Label, T> * cur = to_load.front();
        DataSet<Label, T> subst = _st.subset(cur->get_domain());
        int mx_var_index = max_variance_index(subst);
        vector<T> values;
        for (int i = 0; i < subst.size(); i++)
        {
            values.push_back((*subst[i])[mx_var_index]);
        }
        T pivot_l, pivot_r;
        if (a > 0)
        {
            pivot_l = selector(values, (size_t)(values.size() * (0.5 - a)));
            pivot_r = selector(values, (size_t)(values.size() * (0.5 + a)));
        }
        _range_mp[cur] = range(pivot_l, pivot_r);
        to_load.push(cur->left);
        to_load.push(cur->right);
    }
}

template<class Label, class T>
VirtualSpillTree<Label, T>::VirtualSpillTree
        (ifstream & in, DataSet<Label, T> & st) :
    KDTree<Label, T>(in, st)
{
    queue<KDTreeNode<Label, T> *> to_load;
    to_load.push(_root);
    while (!to_load.empty())
    {
        KDTreeNode<Label, T> * cur = to_load.front();
        to_load.pop();
        bool exists = cur != NULL;
        if (exists)
        {
            DataSet<Label, T> subst = _st.subset(cur->get_domain());
            T pivot_l, pivot_r;
            in.read((char *)&pivot_l, sizeof(T));
            in.read((char *)&pivot_r, sizeof(T));
            _range_mp[cur] = range(pivot_l, pivot_r);
            to_load.push(cur->left);
            to_load.push(cur->right);
        }
    }
}

template<class Label, class T>
void VirtualSpillTree<Label, T>::save(ofstream & out) const
{
    KDTree<Label, T>::save(out);
    queue<KDTreeNode<Label, T> *> to_save;
    to_save.push(_root);
    while (!to_save.empty())
    {
        KDTreeNode<Label, T> * cur = to_save.front();
        to_save.pop();
        bool exists = cur != NULL;
        if (exists)
        {
            range cur_range = _range_mp[cur];
            out.write((char *)&cur_range->first, sizeof(T));
            out.write((char *)&cur_range->second, sizeof(T));
            if (cur->right)
                to_save.push(cur->_right);
            if (cur->left)
                to_save.push(cur->_left);
        }
    }
}

template<class Label, class T>
vector<size_t> VirtualSpillTree<Label, T>::subdomain(vector<T> * query)
{
    queue<KDTreeNode<Label, T> *> to_explore;
    set<size_t> domain_st;
    to_explore.push(_root);
    while (!to_explore.empty())
    {
        KDTreeNode<Label, T> * cur = to_explore.top();
        to_explore.pop();
        if (cur->get_left() && cur->get_right())
        {
            range cur_range = _range_mp[cur];
            if (cur_range->first <= query[cur->get_index()] &&
                query[cur->get_index()] <= cur_range->second)
            {
                to_explore.push(cur->get_right());
                to_explore.push(cur->get_left());
            }
            else if ((*query)[cur->get_index()] <= cur->get_pivot())
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
    set <size_t>::iterator st_i;
    vector <size_t> domain;
    for (st_i = domain_st.begin(); st_i != domain_st.end(); st_i++)
    {
        domain.push_back(*st_i);
    }
    return domain;
}

#endif
