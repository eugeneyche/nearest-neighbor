#ifndef _VIRTUAL_SPILL_TREE_H
#define _VIRTUAL_SPILL_TREE_H

#include "kd_tree.h"
#include <map>
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
    VirtualSpillTree(istream & in, DataSet<Label, T> & st);
    virtual void save(ostream & out) const;
    virtual vector<size_t> subdomain(vector<T> * query);
};

#endif
