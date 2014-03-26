#ifndef _KD_TREE_H
#define _KD_TREE_H

#include <queue>
#include <map>
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
    KDTreeNode(ifstream & in);
    ~KDTreeNode();
    virtual void save(ofstream & out) const;
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
    KDTree(ifstream & in, DataSet<Label, T> & st);
    ~KDTree();
    virtual operator KDTreeNode<Label, T> * () const
    { return _root; }
    virtual void save(ofstream & out) const;
    virtual vector<size_t> subdomain(vector<T> * query);
};

#define UNDEF (-1)

template<class Label, class T>
static KDTreeNode<Label, T> * build_tree(size_t c,
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
    vector<size_t> subdomain_l;
    vector<size_t> subdomain_r;
    for (int i = 0; i < domain.size(); i++)
    {
        if (values[i] <= pivot)
            subdomain_l.push_back(domain[i]);
        else
            subdomain_r.push_back(domain[i]);
    }
    KDTreeNode<Label, T> * result = new KDTreeNode<Label, T>
            (mx_var_index, pivot, domain);
    result->_left = build_tree(c, st, subdomain_l);
    result->_right = build_tree(c, st, subdomain_r);
    return result;
}

template<class Label, class T>
KDTreeNode<Label, T>::KDTreeNode(const vector<size_t> domain) :
  _index (UNDEF),
  _pivot (UNDEF),
  _left (NULL),
  _right (NULL),
  _domain (domain)
{ }

template<class Label, class T>
KDTreeNode<Label, T>::KDTreeNode(size_t index, 
        T pivot, vector<size_t> domain) :
  _index (index),
  _pivot (pivot),
  _left (NULL), 
  _right (NULL),
  _domain (domain)
{ }

template<class Label, class T>
KDTreeNode<Label, T>::KDTreeNode(ifstream & in)
{
     in.read((char *)&_index, sizeof(size_t));
     in.read((char *)&_pivot, sizeof(T));
     size_t sz;
     in.read((char *)&sz, sizeof(size_t));
     while (sz--)
     {
         size_t v;
         in.read((char *)&v, sizeof(size_t));
         _domain.push_back(v);
     }
}

template<class Label, class T>
KDTreeNode<Label, T>::~KDTreeNode()
{
    if (_left) delete _left;
    if (_right) delete _right;
}

template<class Label, class T>
void KDTreeNode<Label, T>::save(ofstream & out) const
{
    out.write((char *)&_index, sizeof(size_t)); 
    out.write((char *)&_pivot, sizeof(T)); 
    size_t sz = _domain.size();
    out.write((char *)&sz, sizeof(size_t)); 
    out.write((char *)&_domain[0], 
            sizeof(size_t) * _domain.size());
}

template<class Label, class T>
KDTree<Label, T>::KDTree(DataSet<Label, T> & st) :
  _root (NULL),
  _st (st)
{ }

template<class Label, class T>
KDTree<Label, T>::KDTree(size_t c, DataSet<Label, T> & st) :
  _root (build_tree(c, st, st.get_domain())),
  _st (st)
{ }

template<class Label, class T>
KDTree<Label, T>::KDTree(ifstream & in, DataSet<Label, T> & st) :
  _st (st)
{
    queue<KDTreeNode<Label, T> *&> to_load;
    to_load.push(_root);
    while (!to_load.empty())
    {
        KDTreeNode<Label, T> * cur = to_load.front();
        bool exist;
        in.read((char *)&exist, sizeof(bool));
        if (!exist)
        {
            cur = NULL; 
        }
        *cur = new KDTreeNode<Label, T>(in);
        to_load.push(cur->_left);
        to_load.push(cur->_right);
    }
}

template<class Label, class T>
KDTree<Label, T>::~KDTree()
{
    if (_root) delete _root;
}

template<class Label, class T>
void KDTree<Label, T>::save(ofstream & out) const
{
    queue<KDTreeNode<Label, T> *> to_save;
    to_save.push(_root);
    while (!to_save.empty())
    {
        KDTreeNode<Label, T> * cur = to_save.front();
        to_save.pop();
        bool exists = cur != NULL;
        out.write((char *)&exists, sizeof(bool)); 
        if (exists)
        {
            cur->save(out);
            to_save.push(cur->_left);
            to_save.push(cur->_right);
        }
    }
}

template<class Label, class T>
vector<size_t> KDTree<Label, T>::subdomain(vector<T> * query)
{
    queue<KDTreeNode<Label, T> *> expl;
    expl.push(_root);
    while (!expl.empty())
    {
        KDTreeNode<Label, T> * cur = expl.top();
        expl.pop();
        if (cur->_left && cur->_right)
        {
            if ((*query)[cur->_index] <= cur->_pivot)
                expl.push(cur->_left);
            else
                expl.push(cur->_right);
        }
        else
            return cur->_domain;
    }
    return vector<size_t>();
}
#endif
