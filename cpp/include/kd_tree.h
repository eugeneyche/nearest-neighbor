#ifndef KD_TREE_H_
#define KD_TREE_H_

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
    size_t index_;
    T pivot_;
    KDTreeNode * left_, * right_;
    vector<size_t> domain_;
public:
    KDTreeNode(const vector<size_t> domain);
    KDTreeNode(size_t index, T pivot, vector<size_t> domain);
    KDTreeNode(ifstream & in);
    ~KDTreeNode();
    size_t get_index() const
    { return index_; }
    T get_pivot() const
    { return pivot_; }
    KDTreeNode * get_left() const
    { return left_; }
    KDTreeNode * get_right() const
    { return right_; }
    vector<size_t> get_domain() const
    { return domain_; }
    void set_left(KDTreeNode * left)
    { left_ = left; };
    void set_right(KDTreeNode * right)
    { right_ = right; };
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
    KDTreeNode<Label, T> * root_;
    DataSet<Label, T> & st_;
public:
    KDTree(DataSet<Label, T> & st);
    KDTree(size_t c, DataSet<Label, T> & st);
    KDTree(ifstream & in, DataSet<Label, T> & st);
    ~KDTree();
    KDTreeNode<Label, T> * get_root() const
    { return root_; }
    DataSet<Label, T> & get_st() const
    { return st_; }
    void set_root(KDTreeNode<Label, T> * root)
    { root_ = root; }
    virtual void save(ofstream & out) const;
    virtual vector<size_t> subdomain(vector<T> * query, size_t l_c = 0);
};

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
    size_t subdomain_l_lim = (size_t)(values.size() * 0.5);
    vector<size_t> subdomain_r;
    vector<size_t> pivot_pool;
    for (size_t i = 0; i < domain.size(); i++)
    {
        if (pivot == values[i])
        {
            pivot_pool.push_back(domain[i]);
        }
        else
        {
            if (values[i] <= pivot)
                subdomain_l.push_back(domain[i]);
            else
                subdomain_r.push_back(domain[i]);
        }
    }
    while (subdomain_l_lim > subdomain_l.size())
    {
        size_t curr = pivot_pool.back();
        pivot_pool.pop_back();
        subdomain_l.push_back(curr);
    }
    while (!pivot_pool.empty())
    {
        size_t curr = pivot_pool.back();
        pivot_pool.pop_back();
        subdomain_r.push_back(curr);
    }
    KDTreeNode<Label, T> * result = new KDTreeNode<Label, T>
            (mx_var_index, pivot, domain);
    result->left_ = build_tree(c, st, subdomain_l);
    result->right_ = build_tree(c, st, subdomain_r);
    return result;
}

template<class Label, class T>
KDTreeNode<Label, T>::KDTreeNode(const vector<size_t> domain) :
  index_ (0),
  pivot_ (0),
  left_ (NULL),
  right_ (NULL),
  domain_ (domain)
{ }

template<class Label, class T>
KDTreeNode<Label, T>::KDTreeNode(size_t index, 
        T pivot, vector<size_t> domain) :
  index_ (index),
  pivot_ (pivot),
  left_ (NULL), 
  right_ (NULL),
  domain_ (domain)
{ }

template<class Label, class T>
KDTreeNode<Label, T>::KDTreeNode(ifstream & in)
{
     in.read((char *)&index_, sizeof(size_t));
     in.read((char *)&pivot_, sizeof(T));
     size_t sz;
     in.read((char *)&sz, sizeof(size_t));
     #ifdef DEBUG
     cerr << "[DEBUG: Building node with index " << index_ <<
            " and size " << sz << "]" << endl;
     #endif
     while (sz--)
     {
         size_t v;
         in.read((char *)&v, sizeof(size_t));
         domain_.push_back(v);
     }
}

template<class Label, class T>
KDTreeNode<Label, T>::~KDTreeNode()
{
    if (left_) delete left_;
    if (right_) delete right_;
}

template<class Label, class T>
void KDTreeNode<Label, T>::save(ofstream & out) const
{
     #ifdef DEBUG
     cerr << "[DEBUG: Saving node with index " << index_ <<
            " and size " << domain_.size() << "]" << endl;
     #endif
    out.write((char *)&index_, sizeof(size_t)); 
    out.write((char *)&pivot_, sizeof(T)); 
    size_t sz = domain_.size();
    out.write((char *)&sz, sizeof(size_t)); 
    out.write((char *)&domain_[0], 
            sizeof(size_t) * domain_.size());
}

template<class Label, class T>
KDTree<Label, T>::KDTree(DataSet<Label, T> & st) :
  root_ (NULL),
  st_ (st)
{ }

template<class Label, class T>
KDTree<Label, T>::KDTree(size_t c, DataSet<Label, T> & st) :
  root_ (build_tree(c, st, st.get_domain())),
  st_ (st)
{ }

template<class Label, class T>
KDTree<Label, T>::KDTree(ifstream & in, DataSet<Label, T> & st) :
  st_ (st)
{
    queue<KDTreeNode<Label, T> **> to_load;
    to_load.push(&root_);
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
        to_load.push(&(*cur)->left_);
        to_load.push(&(*cur)->right_);
    }
}

template<class Label, class T>
KDTree<Label, T>::~KDTree()
{
    if (root_) delete root_;
}

template<class Label, class T>
void KDTree<Label, T>::save(ofstream & out) const
{
    queue<KDTreeNode<Label, T> *> to_save;
    to_save.push(root_);
    while (!to_save.empty())
    {
        KDTreeNode<Label, T> * cur = to_save.front();
        to_save.pop();
        bool exists = cur != NULL;
        out.write((char *)&exists, sizeof(bool)); 
        if (exists)
        {
            cur->save(out);
            to_save.push(cur->left_);
            to_save.push(cur->right_);
        }
    }
}

template<class Label, class T>
vector<size_t> KDTree<Label, T>::subdomain(vector<T> * query, size_t l_c)
{
    queue<KDTreeNode<Label, T> *> expl;
    expl.push(root_);
    while (!expl.empty())
    {
        KDTreeNode<Label, T> * cur = expl.front();
        expl.pop();
        if (cur->left_ && cur->right_ &&
            cur->domain_.size() >= l_c)
        {
            if ((*query)[cur->index_] <= cur->pivot_)
                expl.push(cur->left_);
            else
                expl.push(cur->right_);
        }
        else
            return cur->domain_;
    }
    return vector<size_t>();
}

#endif
