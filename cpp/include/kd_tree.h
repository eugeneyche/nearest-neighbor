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
    size_t get_index() const
    { return _index; }
    T get_pivot() const
    { return _pivot; }
    KDTreeNode * get_left() const
    { return _left; }
    KDTreeNode * get_right() const
    { return _right; }
    vector<size_t> get_domain() const
    { return _domain; }
    void set_left(KDTreeNode * left)
    { _left = left; };
    void set_right(KDTreeNode * right)
    { _right = right; };
    virtual void save(ofstream & out) const;
    friend class KDTree<Label, T>;
};

template<class Label, class T>
class KDTree
{
private:
    static KDTreeNode<Label, T> * build_tree(size_t c,
            DataSet<Label, T> & st, vector<size_t> domain);
protected:
    KDTreeNode<Label, T> * _root;
    DataSet<Label, T> & _st;
public:
    KDTree(DataSet<Label, T> & st);
    KDTree(size_t c, DataSet<Label, T> & st);
    KDTree(ifstream & in, DataSet<Label, T> & st);
    ~KDTree();
    KDTreeNode<Label, T> * get_root() const
    { return _root; }
    DataSet<Label, T> & get_st() const
    { return _st; }
    void set_root(KDTreeNode<Label, T> * root)
    { _root = root; }
    virtual void save(ofstream & out) const;
    virtual vector<size_t> subdomain(vector<T> * query);
};

#define UNDEF 0

template<class Label, class T>
KDTreeNode<Label, T> * KDTree<Label, T>::build_tree(size_t c,
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
    vector<size_t> subdomain_l;
    vector<size_t> subdomain_r;
    for (size_t i = 0; i < domain.size(); i++)
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
     #ifdef DEBUG
     cerr << "[DEBUG: Building node with index " << _index <<
            " and pivot " << _pivot << "]" << endl;
     #endif
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
     #ifdef DEBUG
     cerr << "[DEBUG: Saving node with index " << _index <<
            " and pivot " << _pivot << "]" << endl;
     #endif
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
    queue<KDTreeNode<Label, T> **> to_load;
    to_load.push(&_root);
    while (!to_load.empty())
    {
        KDTreeNode<Label, T> ** cur = to_load.front();
        to_load.pop();
        bool exist;
        in.read((char *)&exist, sizeof(bool));
        if (!exist)
        {
            *cur = NULL; 
            continue;
        }
        *cur = new KDTreeNode<Label, T>(in);
        to_load.push(&(*cur)->_left);
        to_load.push(&(*cur)->_right);
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
        KDTreeNode<Label, T> * cur = expl.front();
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
