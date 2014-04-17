#ifndef _VIRTUAL_SPILL_TREE_H
#define _VIRTUAL_SPILL_TREE_H

#include <map>
#include <queue>
#include <set>
#include <utility>
#include "kd_tree.h"
using namespace std;

template<class Label, class T>
class VirtualSpillTree : public KDTree<Label, T>
{
    typedef pair<T, T> range;
protected:
    map<KDTreeNode<Label, T> *, range> range_mp_;
public:
    VirtualSpillTree(DataSet<Label, T> & st);
    VirtualSpillTree(size_t c, double a, DataSet<Label, T> & st);
    VirtualSpillTree(ifstream & in, DataSet<Label, T> & st);
    virtual void save(ofstream & out) const;
    virtual vector<size_t> subdomain(vector<T> * query);
};

template<class Label, class T>
VirtualSpillTree<Label, T>::VirtualSpillTree(DataSet<Label, T> & st) :
  KDTree<Label, T>(st)
{ }

template<class Label, class T>
VirtualSpillTree<Label, T>::VirtualSpillTree(size_t c, double a, 
        DataSet<Label, T> & st) :
  KDTree<Label, T>(c, st)
{
    queue<KDTreeNode<Label, T> *> to_update;
    to_update.push((this->get_root()));
    while (!to_update.empty())
    {
        KDTreeNode<Label, T> * cur = to_update.front();
        to_update.pop();
        bool exists = cur != NULL;
        if (exists)
        {
            DataSet<Label, T> subst = st.subset(cur->get_domain());
            size_t mx_var_index = cur->get_index();
            vector<T> values;
            for (size_t i = 0; i < subst.size(); i++)
            {
                values.push_back((*subst[i])[mx_var_index]);
            }
            T pivot_l = selector(values, (size_t)(values.size() * (0.5 - a)));
            T pivot_r = selector(values, (size_t)(values.size() * (0.5 + a)));
            range_mp_[cur] = range(pivot_l, pivot_r);
            to_update.push(cur->get_left());
            to_update.push(cur->get_right());
        }
    }
}

template<class Label, class T>
VirtualSpillTree<Label, T>::VirtualSpillTree(ifstream & in, 
        DataSet<Label, T> & st) :
  KDTree<Label, T>(in, st)
{
    queue<KDTreeNode<Label, T> *> to_update;
    to_update.push((this->get_root()));
    while (!to_update.empty())
    {
        KDTreeNode<Label, T> * cur = to_update.front();
        to_update.pop();
        bool exists = cur != NULL;
        if (exists)
        {
            T pivot_l, pivot_r;
            in.read((char *)&pivot_l, sizeof(T));
            in.read((char *)&pivot_r, sizeof(T));
            range_mp_[cur] = range(pivot_l, pivot_r);
            to_update.push(cur->get_left());
            to_update.push(cur->get_right());
        }
    }
}

template<class Label, class T>
void VirtualSpillTree<Label, T>::save(ofstream & out) const
{
    this->KDTree<Label, T>::save(out);
    queue<KDTreeNode<Label, T> *> to_save;
    to_save.push((this->get_root()));
    while (!to_save.empty())
    {
        KDTreeNode<Label, T> * cur = to_save.front();
        to_save.pop();
        bool exists = cur != NULL;
        if (exists)
        {
            range cur_range = range_mp_.at(cur);
            out.write((char *)&cur_range.first, sizeof(T));
            out.write((char *)&cur_range.second, sizeof(T));
            to_save.push(cur->get_left());
            to_save.push(cur->get_right());
        }
    }
}


template<class Label, class T>
vector<size_t> VirtualSpillTree<Label, T>::subdomain(vector<T> * query)
{
    queue<KDTreeNode<Label, T> *> to_explore;
    set<size_t> domain_st;
    to_explore.push(this->get_root());
    while (!to_explore.empty())
    {
        KDTreeNode<Label, T> * cur = to_explore.front();
        to_explore.pop();
        bool exists = cur != NULL;
        if (exists)
        {
            if (cur->get_left() || cur->get_right())
            {
                range cur_range = range_mp_.at(cur);
                if (cur_range.first < (*query)[cur->get_index()] &&
                        (*query)[cur->get_index()] <= cur_range.second)
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
                vector<size_t> l_domain = cur->get_domain();
                for (size_t i = 0; i < l_domain.size(); i++)
                {
                    domain_st.insert(l_domain[i]);
                }
            }
        }
    }
    set<size_t>::iterator st_i;
    vector<size_t> domain;
    for (st_i = domain_st.begin(); st_i != domain_st.end(); st_i++)
    {
        domain.push_back(*st_i);
    }
    return domain;
}

#endif
