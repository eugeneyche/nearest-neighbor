/* 
 * File             : bsp_tree.h
 * Date             : 2014-5-29
 * Summary          : Infrastructure to hold a binary space partition tree.
 */
#ifndef BSP_TREE_H_
#define BSP_TREE_H_

#include <queue>
#include <map>
#include "vector_math.h"
#include "data_set.h"
using namespace std;

/* Class Prototypes */
template<class Label, class T>
class BSPTreeNode;

template<class Label, class T>
class BSPTree;

/* Class Definitions */


/* 
 * Name             : BSPTreeNode
 * Description      : Data structure to hold a node of a KDTree
 * Data Field(s)    : dir_      - Direction to project the distance onto
 *                    pivot_    - The value to pivot on
 *                    left_     - Pointer to left subtree node
 *                    right_    - Pointer to right subtree node
 *                    domain_   - vectors out of vector space in data set
 * Functions(s)     : BSPTreeNode(const vector<size_t>) 
 *                              - Create a BSPTreeNode of given domain (leaf)
 *                    BSPTreeNode(size_t, T, vector<size_t>)
 *                              - Create a BSPTreeNode of given domain (non-leaf)
 *                    BSPTreeNode(ifstream &)
 *                              - Creates a KDTreeNode through de-serialization
 *                    size_t get_index() const
 *                              - Gets index of max variance
 *                    BSPTreeNode * get_left() const
 *                              - Returns pointer to left subtree node
 *                    BSPTreeNode * get_right() const
 *                              - Returns pointer to right subtree node
 *                    vector<size_t> get_domain() const
 *                              - Returns the domain the node stores
 *                    void set_left(BSPTreeNode *)
 *                              - Sets the left subtree node
 *                    void set_right(BSPTreeNode *)
 *                              - Sets the right subtree node
 *                    void save(ofstream &)
 *                              - Serializes node 
 */
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

/*
 * Name             : BSPTree
 * Description      : Encapsulates the BSPTreeNodes into tree.
 * Data Field(s)    : root_ - Holds the root node of tree
 *                    st_   - Holds the data set associated with tree
 * Function(s)      : BSPTree(DataSet<Label, T>)
 *                          - Creates a tree of given data set
 *                    BSPTree(size_t, DataSet<Label, T>)
 *                          - Creates a tree of given min leaf size and
 *                            data set
 *                    BSPTree(ifstream &, DataSet<Label, T>)
 *                          - De-serialization 
 *                    ~BSPTree()
 *                          - Deconstructor
 *                    BSPTreeNode<Label, T> * get_root() const
 *                          - Returns the root
 *                    DataSet<Label, T> & get_st() const
 *                          - Returns the set associated with the tree
 *                    void save(ofstream &) const
 *                          - Serializes the tree
 *                    vector<size_t> subdomain(vector<T> *, size_t)
 *                          - Queries the tree for a subdomain
 */
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
    virtual vector<size_t> subdomain(vector<T> * query, size_t l_c = 0);
};

template<class Label, class T>
BSPTreeNode<Label, T> * BSPTree<Label, T>::build_tree(size_t c,
        DataSet<Label, T> & st, vector<size_t> domain)
{
    LOG_FINE("Building BSPTree of c = %ld and domain of size %ld\n", c, domain.size());
    if (domain.size() < c)
        return new BSPTreeNode<Label, T>(domain);
    DataSet<Label, T> subst = st.subset(domain);
    vector<double> mx_var_dir = max_eigen_vector(subst);
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
    LOG_FINE("Saving BSPTreeNode data with domain of size %ld\n", domain_.size());
    size_t dim = dir_.size();
    out.write((char *)&dim, sizeof(size_t)); 
    out.write((char *)&dir_[0], 
            sizeof(double) * dir_.size());
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
    LOG_FINE("De-serializing KDTree\n");
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
    LOG_FINE("Deconstructing BSPTree\n");
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
vector<size_t> BSPTree<Label, T>::subdomain(vector<T> * query, size_t l_c)
{
    LOG_FINE("Querying BSPTree down to min leaf of size %ld\n", l_c);
    queue<BSPTreeNode<Label, T> *> expl;
    expl.push(root_);
    while (!expl.empty())
    {
        BSPTreeNode<Label, T> * cur = expl.front();
        expl.pop();
        if (cur->left_ && cur->right_ &&
            cur->domain_.size() >= l_c)
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
