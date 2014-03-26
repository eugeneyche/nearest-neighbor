#include "kd_tree.h"

#define UNDEF (-1)
#define HALF  (0.5)

template <class Label, class T>
static KDTreeNode<Label, T> * build_tree(size_t c, double a, 
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
        if (a > 0 && pivot_l < values[i] && values[i] <= pivot_r)
        {
            subdomain_l.push_back(domain[i]);
            subdomain_r.push_back(domain[i]);
            continue;
        }
        if (values[i] <= pivot)
            subdomain_l.push_back(domain[i]);
        else
            subdomain_r.push_back(domain[i]);
    }
    KDTreeNode<Label, T> * result = new KDTreeNode<Label, T>(mx_var_index, pivot, domain);
    result->_left = build_tree(c, a, st, subdomain_l);
    result->_right = build_tree(c, a, st, subdomain_r);
    return result;
}

template <class Label, class T>
KDTree<Label, T>::KDTree(size_t c, DataSet<Label, T> & st) :
  _root (build_tree(c, 0, st, st.get_domain())),
  _st (st)
{ }

template <class Label, class T>
KDTree<Label, T>::KDTree(size_t c, double a, DataSet<Label, T> & st) :
  _root (build_tree(c, a, st, st.get_domain())),
  _st (st)
{ }

template <class Label, class T>
KDTree<Label, T>::KDTree(istream & in, DataSet<Label, T> & st) :
  _st (st)
{
    queue<KDTreeNode<Label, T> **> to_load;
    to_load.push(&_root);
    while (!to_load.empty())
    {
        KDTreeNode<Label, T> ** cur = to_load.front();
        bool exist;
        in.read((char *)&exist, sizeof(bool));
        if (!exist)
        {
            *cur = NULL; 
        }
        *cur = new KDTreeNode<Label, T>;
        in.read((char *)&(*cur)->_index, sizeof(T));
        in.read((char *)&(*cur)->_pivot, sizeof(double));
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

template <class Label, class T>
KDTree<Label, T>::~KDTree()
{
    delete _root;
}

template <class Label, class T>
void KDTree<Label, T>::save(ostream & out) const
{
    stack<KDTreeNode<Label, T> *> to_save;
    to_save.push(_root);
    while (!to_save.empty())
    {
        KDTreeNode<Label, T> * curr = to_save.top();
        to_save.pop();
        bool exists = curr != NULL;
        out.write((char *)&exists, sizeof(bool)); 
        if (exists)
        {
            out.write((char *)&curr->_index, sizeof(size_t)); 
            out.write((char *)&curr->_pivot, sizeof(T)); 
            size_t sz = curr->_domain.size();
            out.write((char *)&sz, sizeof(size_t)); 
            out.write((char *)&curr->_domain[0], sizeof(size_t) * curr->_domain.size());
            to_save.push(curr->_right);
            to_save.push(curr->_left);
        }
    }
}

template <class Label, class T>
vector<size_t> KDTree<Label, T>::subdomain(vector<T> * query)
{
    stack<KDTreeNode<Label, T> *> expl;
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

template <class Label, class T>
KDTreeNode<Label, T>::KDTreeNode() :
  _index (UNDEF),
  _pivot (UNDEF),
  _left (NULL),
  _right (NULL)
{ }

template <class Label, class T>
KDTreeNode<Label, T>::KDTreeNode(vector<size_t> domain) :
  _index (UNDEF),
  _pivot (UNDEF),
  _left (NULL),
  _right (NULL),
  _domain(domain)
{ }

template <class Label, class T>
KDTreeNode<Label, T>::KDTreeNode(size_t index, T pivot, vector<size_t> domain) :
  _index (index),
  _pivot (pivot),
  _left (NULL),
  _right (NULL),
  _domain(domain)
{ }

template <class Label, class T>
KDTreeNode<Label, T>::KDTreeNode(const KDTreeNode & copy) :
  _index (copy._index),    
  _pivot (copy._pivot),
  _domain (copy._domain),
  _left (copy._left),
  _right (copy._right)
{ }

template <class Label, class T>
KDTreeNode<Label, T>::~KDTreeNode()
{
    if (_left)
        delete _left;
    if (_right)
        delete _right;
}

template <class Label, class T>
size_t KDTreeNode<Label, T>::get_index() const
{
    return _index;
}

template <class Label, class T>
T KDTreeNode<Label, T>::get_pivot() const
{
    return _pivot;
}

template <class Label, class T>
vector<size_t> KDTreeNode<Label, T>::get_domain() const
{
    return _domain;
}

template <class Label, class T>
KDTreeNode<Label, T> * KDTree<Label, T>::operator 
        KDTreeNode<Label, T> * ()
{
    return _root;
}

template <class Label, class T>
KDTreeNode<Label, T> * KDTreeNode<Label, T>::get_left() const
{
    return _left;
}

template <class Label, class T>
KDTreeNode<Label, T> * KDTreeNode<Label, T>::get_right() const
{
    return _right;
}
