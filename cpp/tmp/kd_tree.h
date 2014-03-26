#ifndef _KD_TREE_H
#define _KD_TREE_H

#include "data_set.h"
#include <stack>
#include <set>
#include <queue>
using namespace std;

template <class Label, class T>
class KDTreeNode;

template <class Label, class T>
class KDTree;

template <class Label, class T>
class KDTreeNode
{
protected:
    size_t _index;
    T _pivot;
    KDTreeNode * _left,
               * _right;
    vector<size_t> _domain;
public:
    KDTreeNode();
    KDTreeNode(vector<size_t> domain);
    KDTreeNode(size_t index, T pivot, vector<size_t> domain);
    KDTreeNode(const KDTreeNode & copy);
    ~KDTreeNode();
    size_t get_index() const;
    T get_pivot() const;
    KDTreeNode * get_left() const;
    KDTreeNode * get_right() const;
    vector<size_t> get_domain() const;
    friend class KDTree<Label, T>;
};

template <class Label, class T>
class KDTree
{
protected:
    KDTreeNode<Label, T> * _root;
    DataSet<Label, T> & _st;
public:
    KDTree(size_t c, DataSet<Label, T> & st);
    KDTree(size_t c, double a, DataSet<Label, T> & st);
    KDTree(istream & in, DataSet<Label, T> & st);
    ~KDTree();
    KDTreeNode<Label, T> * operator 
            KDTreeNode<Label, T> * ();
    void save(ostream & out) const;
    vector<size_t> subdomain(vector<T> * query);
};

#endif
