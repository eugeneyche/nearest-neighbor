#ifndef _KD_TREE_H
#define _KD_TREE_H

#include "data_set.h"
using namespace std;

template<class Label, class T>
class KDTreeNode;

template<class Label, class T>
class KDTree;

template<class Label, class T>
class KDTreeNode
{
protected:
    size_t _index;
    T _pivot;
    KDTreeNode * _left, * _right;
    vector<size_t> _domain;
public:
    KDTreeNode(const vector<size_t> domain);
    KDTreeNode(size_t index, T pivot, vector<size_t> domain);
    KDTreeNode(istream & in);
    ~KDTreeNode();
    virtual void save(ostream & out) const;
    virtual size_t get_index() const
    { return _index; }
    virtual T get_pivot() const
    { return _pivot; }
    virtual KDTreeNode * get_left() const
    { return _left; }
    virtual KDTreeNode * get_right() const
    { return _right; }
    vector<size_t> get_domain() const
    { return _domain; }
    friend class KDTree<Label, T>;
};

template<class Label, class T>
class KDTree
{
protected:
    KDTreeNode<Label, T> * _root;
    DataSet<Label, T> & _st;
public:
    KDTree(DataSet<Label, T> & st);
    KDTree(size_t c, DataSet<Label, T> & st);
    KDTree(istream & in, DataSet<Label, T> & st);
    ~KDTree();
    virtual operator KDTreeNode<Label, T> * () const
    { return _root; }
    virtual void save(ostream & out) const;
    virtual vector<size_t> subdomain(vector<T> * query);
};

#endif
