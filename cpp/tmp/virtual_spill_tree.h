#ifndef _VIRTUAL_SPILL_TREE_H
#define _VIRTUAL_SPILL_TREE_H

#include "data_set.h"
#include <stack>
#include <set>
#include <queue>
using namespace std;

template <class Label, class T>
class VirtualSpillTreeNode;

template <class Label, class T>
class VirtualSpillTree;

template <class Label, class T>
class VirtualSpillTreeNode
{
protected:
    size_t _index;
    T _pivot, _pivot_l, _pivot_r;
    VirtualSpillTreeNode * _left,
                         * _right;
    vector<size_t> _domain;
public:
    VirtualSpillTreeNode();
    VirtualSpillTreeNode(vector<size_t> domain);
    VirtualSpillTreeNode(size_t index, T pivot, 
            T pivot_l, T pivot_r, vector<size_t> domain);
    VirtualSpillTreeNode(const VirtualSpillTreeNode & copy);
    ~VirtualSpillTreeNode();
    bool in_range(vector<T> * query) const;
    T get_pivot() const;
    T get_pivot_l() const;
    T get_pivot_r() const;
    VirtualSpillTreeNode * get_left() const;
    VirtualSpillTreeNode * get_right() const;
    vector<size_t> get_domain() const;
    friend class VirtualSpillTree<Label, T>;
};

template <class Label, class T>
class VirtualSpillTree
{
protected:
    VirtualSpillTreeNode<Label, T> * _root;
    DataSet<Label, T> & _st;
public:
    VirtualSpillTree(size_t c, double a, DataSet<Label, T> & st);
    VirtualSpillTree(istream & in, DataSet<Label, T> & st);
    ~VirtualSpillTree();
    virtual void save(ostream & out) const;
    virtual vector<size_t> subdomain(vector<T> * query);
};

#endif
