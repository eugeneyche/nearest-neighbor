#ifndef BSP_SPILL_TREE_H_
#define BSP_SPILL_TREE_H_

#include "bsp_tree.h"

#define MIN(x, y) (x) < (y) ? (x) : (y)

template<class Label, class T>
class BSPSpillTree;

template<class Label, class T>
class BSPSpillTree : public BSPTree<Label, T>
{
private:
    static BSPTreeNode<Label, T> * build_tree(size_t c, double a,
            DataSet<Label, T> & st, vector<size_t> domain);
public:
    BSPSpillTree(DataSet<Label, T> & st);
    BSPSpillTree(size_t c, double a, DataSet<Label, T> & st);
    BSPSpillTree(ifstream & in, DataSet<Label, T> & st);
};

template<class Label, class T>
BSPTreeNode<Label, T> * BSPSpillTree<Label, T>::build_tree(size_t c, double a,
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
    double pivot = selector(values,   (size_t)(values.size() *  0.5));
    double pivot_l = selector(values, (size_t)(values.size() * (0.5 - a)));
    double pivot_r = selector(values, (size_t)(values.size() * (0.5 + a)));
    vector<size_t> subdomain_l;
    size_t subdomain_lim = (size_t)(values.size() * (0.5 + a));
    vector<size_t> subdomain_r;
    vector<size_t> pivot_pool;
    vector<size_t> pivot_e_pool;
    for (size_t i = 0; i < domain.size(); i++)
    {
        if (pivot == values[i])
            pivot_pool.push_back(domain[i]);
        else if (pivot_l == dot(st[i], mx_var_dir) || pivot_r == dot(st[i], mx_var_dir))
            pivot_e_pool.push_back(domain[i]);
        else
        {
            if (pivot_l < dot(st[i], mx_var_dir) && dot(st[i], mx_var_dir) < pivot_r)
            {
                subdomain_l.push_back(domain[i]);
                subdomain_r.push_back(domain[i]);
            }
            else
            {
                if (dot(st[i], mx_var_dir) < pivot)
                    subdomain_l.push_back(domain[i]);
                else
                    subdomain_r.push_back(domain[i]);
            }
        }
    }
    size_t d_l = MIN(subdomain_lim - subdomain_l.size(), pivot_pool.size() + pivot_e_pool.size());
    size_t d_r = MIN(subdomain_lim - subdomain_r.size(), pivot_pool.size() + pivot_e_pool.size());
    size_t spill = d_l + d_r - (pivot_pool.size() + pivot_e_pool.size());
    for (size_t i = 0; i < spill; i++)
    {
        size_t curr;
        if (!pivot_pool.empty())
        {
             curr = pivot_pool.back();
             pivot_pool.pop_back();
        }
        else
        {
             curr = pivot_e_pool.back();
             pivot_e_pool.pop_back();
        }
        subdomain_l.push_back(curr);
        subdomain_r.push_back(curr);
    }
    for (size_t i = 0; i < d_l - spill; i++)
    {
        size_t curr;
        if (!pivot_pool.empty())
        {
             curr = pivot_pool.back();
             pivot_pool.pop_back();
        }
        else
        {
             curr = pivot_e_pool.back();
             pivot_e_pool.pop_back();
        }
        subdomain_l.push_back(curr);
    }
    for (size_t i = 0; i < d_r - spill; i++)
    {
        size_t curr;
        if (!pivot_pool.empty())
        {
             curr = pivot_pool.back();
             pivot_pool.pop_back();
        }
        else
        {
             curr = pivot_e_pool.back();
             pivot_e_pool.pop_back();
        }
        subdomain_r.push_back(curr);
    }
    BSPTreeNode<Label, T> * result = new BSPTreeNode<Label, T>
            (mx_var_dir, pivot, domain);
    result->set_left(build_tree(c, a, st, subdomain_l));
    result->set_right(build_tree(c, a, st, subdomain_r));
    return result;
}
template<class Label, class T>
BSPSpillTree<Label, T>::BSPSpillTree(DataSet<Label, T> & st) :
  BSPTree<Label, T>(st)
{ }

template<class Label, class T>
BSPSpillTree<Label, T>::BSPSpillTree(size_t c, double a, DataSet<Label, T> & st) :
  BSPTree<Label, T>(st)
{ this->set_root(build_tree(c, a, st, st.get_domain())); }

template<class Label, class T>
BSPSpillTree<Label, T>::BSPSpillTree(ifstream & in, DataSet<Label, T> & st) :
  BSPTree<Label, T>(in, st)
{ }

#endif
