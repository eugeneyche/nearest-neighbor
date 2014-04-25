#ifndef _BSP_TREE_H
#define _BSP_TREE_H

#include <queue>
#include <map>
#include "vector_math.h"
#include "data_set.h"
using namespace std;

template<class Label, class T>
class BSPTreeNode;

template<class Label, class T>
class BSPTree;

template<class Label, class T>
class BSPTreeNode
{
protected:
    vector<double> dir_;
    double pivot_;
    BSPTreeNode * left_, * right_;
    vector<size_t> domain_;
public:
    BSPTreeNode(const vector<size_t> domain);
    BSPTreeNode(vector<double> dir, double pivot, vector<size_t> domain);
    BSPTreeNode(ifstream & in);
    ~BSPTreeNode();
    vector<double> get_direction() const
    { return dir_; }
    double get_pivot() const
    { return pivot_; }
    BSPTreeNode * get_left() const
    { return left_; }
    BSPTreeNode * get_right() const
    { return right_; }
    vector<size_t> get_domain() const
    { return domain_; }
    void set_left(BSPTreeNode * left)
    { left_ = left; };
    void set_right(BSPTreeNode * right)
    { right_ = right; };
    virtual void save(ofstream & out) const;
    friend class BSPTree<Label, T>;
};

template<class Label, class T>
class BSPTree
{
private:
    static BSPTreeNode<Label, T> * build_tree(size_t c,
            DataSet<Label, T> & st, vector<size_t> domain);
protected:
    BSPTreeNode<Label, T> * root_;
    DataSet<Label, T> & st_;
public:
    BSPTree(DataSet<Label, T> & st);
    BSPTree(size_t c, DataSet<Label, T> & st);
    BSPTree(ifstream & in, DataSet<Label, T> & st);
    ~BSPTree();
    BSPTreeNode<Label, T> * get_root() const
    { return root_; }
    DataSet<Label, T> & get_st() const
    { return st_; }
    void set_root(BSPTreeNode<Label, T> * root)
    { root_ = root; }
    virtual void save(ofstream & out) const;
    virtual vector<size_t> subdomain(vector<T> * query);
};

template<class Label, class T>
BSPTreeNode<Label, T> * BSPTree<Label, T>::build_tree(size_t c,
        DataSet<Label, T> & st, vector<size_t> domain)
{
    #ifdef DEBUG
        cerr << "[DEBUG: Building tree of size " << domain.size() << "]" << endl;
    #endif
    if (domain.size() < c)
        return new BSPTreeNode<Label, T>(domain);
    DataSet<Label, T> subst = st.subset(domain);
    vector<double> mx_var_dir; /* TODO: determine vtr */
    vector<double> values;
    for (size_t i = 0; i < subst.size(); i++)
        values.push_back(dot(*subst[i], mx_var_dir));
    double pivot = selector(values, (size_t)(values.size() * 0.5));
    vector<size_t> subdomain_l;
    size_t subdomain_l_lim = (size_t)(values.size() * 0.5);
    vector<size_t> subdomain_r;
    vector<size_t> pivot_pool;
    for (size_t i = 0; i < domain.size(); i++)
    {
        if (pivot == values[i])
            pivot_pool.push_back(domain[i]);
        else
            if (dot(st[i], mx_var_dir) <= pivot)
                subdomain_l.push_back(domain[i]);
            else
                subdomain_r.push_back(domain[i]);
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
    BSPTreeNode<Label, T> * result = new BSPTreeNode<Label, T>
            (mx_var_dir, pivot, domain);
    result->left_ = build_tree(c, st, subdomain_l);
    result->right_ = build_tree(c, st, subdomain_r);
    return result;
}

template<class Label, class T>
BSPTreeNode<Label, T>::BSPTreeNode(const vector<size_t> domain) :
  dir_ (),
  pivot_ (0),
  left_ (NULL),
  right_ (NULL),
  domain_ (domain)
{ }

template<class Label, class T>
BSPTreeNode<Label, T>::BSPTreeNode(vector<double> dir, 
        double pivot, vector<size_t> domain) :
  dir_ (dir),
  pivot_ (pivot),
  left_ (NULL), 
  right_ (NULL),
  domain_ (domain)
{ }

template<class Label, class T>
BSPTreeNode<Label, T>::BSPTreeNode(ifstream & in)
{
     size_t dim;
     in.read((char *)&dim, sizeof(size_t));
     while (dim--)
     {
         double v;
         in.read((char *)&v, sizeof(double));
         dir_.push_back(v);
     }
     in.read((char *)&pivot_, sizeof(double));
     size_t sz;
     in.read((char *)&sz, sizeof(size_t));
     while (sz--)
     {
         size_t v;
         in.read((char *)&v, sizeof(size_t));
         domain_.push_back(v);
     }
}

template<class Label, class T>
BSPTreeNode<Label, T>::~BSPTreeNode()
{
    if (left_) delete left_;
    if (right_) delete right_;
}

template<class Label, class T>
void BSPTreeNode<Label, T>::save(ofstream & out) const
{
    size_t dim = dir_.size();
    out.write((char *)&dim, sizeof(size_t)); 
    out.write((char *)&dir_[0], 
            sizeof(size_t) * dir_.size());
    out.write((char *)&pivot_, sizeof(double)); 
    size_t sz = domain_.size();
    out.write((char *)&sz, sizeof(size_t)); 
    out.write((char *)&domain_[0], 
            sizeof(size_t) * domain_.size());
}

template<class Label, class T>
BSPTree<Label, T>::BSPTree(DataSet<Label, T> & st) :
  root_ (NULL),
  st_ (st)
{ }

template<class Label, class T>
BSPTree<Label, T>::BSPTree(size_t c, DataSet<Label, T> & st) :
  root_ (build_tree(c, st, st.get_domain())),
  st_ (st)
{ }

template<class Label, class T>
BSPTree<Label, T>::BSPTree(ifstream & in, DataSet<Label, T> & st) :
  st_ (st)
{
    queue<BSPTreeNode<Label, T> **> to_load;
    to_load.push(&root_);
    while (!to_load.empty())
    {
        BSPTreeNode<Label, T> ** cur = to_load.front();
        to_load.pop();
        bool exist;
        in.read((char *)&exist, sizeof(bool));
        if (!exist)
        {
            *cur = NULL; 
            continue;
        }
        *cur = new BSPTreeNode<Label, T>(in);
        to_load.push(&(*cur)->left_);
        to_load.push(&(*cur)->right_);
    }
}

template<class Label, class T>
BSPTree<Label, T>::~BSPTree()
{
    if (root_) delete root_;
}

template<class Label, class T>
void BSPTree<Label, T>::save(ofstream & out) const
{
    queue<BSPTreeNode<Label, T> *> to_save;
    to_save.push(root_);
    while (!to_save.empty())
    {
        BSPTreeNode<Label, T> * cur = to_save.front();
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
vector<size_t> BSPTree<Label, T>::subdomain(vector<T> * query)
{
    queue<BSPTreeNode<Label, T> *> expl;
    expl.push(root_);
    while (!expl.empty())
    {
        BSPTreeNode<Label, T> * cur = expl.front();
        expl.pop();
        if (cur->left_ && cur->right_)
        {
            if (dot(*query, cur->dir_) <= cur->pivot_)
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
