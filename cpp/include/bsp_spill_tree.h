/* 
 * File             : bsp_spill_tree.h
 * Date             : 2014-5-29
 * Summary          : Infrastructure to hold a binary space partition spill 
 *                    tree.
 */
#ifndef BSP_SPILL_TREE_H_
#define BSP_SPILL_TREE_H_

#include "bsp_tree.h"

#define MIN(x, y) (x) < (y) ? (x) : (y)

/* Class Prototypes */

template<class Label, class T>
class BSPSpillTree;

/* Class Definitions */

/* 
 * Name             : BSPSpillTree
 * Description      : Encapsulates the BSPTreeNodes into a spill tree.
 *                    Effectively acts as identically to BSPTree with spillage in
 *                    terms of its nodes.
 * Data Field(s)    : None
 * Functions(s)     : BSPSpillTree(size_t, double, DataSet<Label, T> &)
 *                          - Creates a spill tree with given min leaf size
 *                            and the spill factor
 *                    BSPSpillTree(ifStream & in, DataSet<Label, T> & st)
 *                          - De-serializes a spill tree
 */
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
    LOG_FINE("Building BSPSpillTree of c = %ld and domain of size %ld\n", c, domain.size());
    if (domain.size() < c)
        return new BSPTreeNode<Label, T>(domain);
    DataSet<Label, T> subst = st.subset(domain);
    vector<double> mx_var_dir = max_eigen_vector_oja(subst);
    vector<double> values;
    for (size_t i = 0; i < subst.size(); i++)
        values.push_back(dot(*subst[i], mx_var_dir));
    T pivot = selector(values, (size_t)(values.size() * 0.5));
    T pivot_l = selector(values, (size_t)(values.size() * (0.5 - a)));
    T pivot_r = selector(values, (size_t)(values.size() * (0.5 + a)));
    vector<size_t> subdomain_l;
    size_t subdomain_l_lim = (size_t)(values.size() * (0.5 + a));
    size_t subdomain_r_lim = (size_t)(values.size() * (1 + 2*a) - subdomain_l_lim);
    vector<size_t> subdomain_r;
    vector<size_t> pivot_pool;
    vector<size_t> pivot_l_pool;
    vector<size_t> pivot_r_pool;
    for (size_t i = 0; i < domain.size(); i++)
    {
        if (pivot == values[i])
        {
            pivot_pool.push_back(domain[i]);
        }
        else if (pivot_l == values[i])
        {
            pivot_l_pool.push_back(domain[i]);
        }
        else if (pivot_r == values[i])
        {
            pivot_r_pool.push_back(domain[i]);
        }
        else
        {
            if (pivot_l < values[i] && values[i] < pivot_r)
            {
                subdomain_l.push_back(domain[i]);
                subdomain_r.push_back(domain[i]);
            }
            else
            {
                if (values[i] < pivot)
                {
                    subdomain_l.push_back(domain[i]);
                }
                else
                {
                    subdomain_r.push_back(domain[i]);
                }
            }
        }
    }
    size_t d_l = MIN(subdomain_l_lim - subdomain_l.size(), pivot_pool.size() + pivot_l_pool.size() + pivot_r_pool.size());
    size_t d_r = MIN(subdomain_r_lim - subdomain_r.size(), pivot_pool.size() + pivot_l_pool.size() + pivot_r_pool.size());
    size_t spill = d_l + d_r - (pivot_pool.size() + pivot_l_pool.size() + pivot_r_pool.size());
    LOG_FINE("dl = %ld, dr = %ld, spill = %ld\n", d_l, d_r, spill);
    for (size_t i = 0; i < d_l - spill; i++)
    {
        size_t curr;
        if (!pivot_l_pool.empty())
        {
             curr = pivot_l_pool.back();
             pivot_l_pool.pop_back();
        }
        else if (!pivot_pool.empty())
        {
             curr = pivot_pool.back();
             pivot_pool.pop_back();
        }
        else
        {
             curr = pivot_r_pool.back();
             pivot_r_pool.pop_back();
        }
        subdomain_l.push_back(curr);
    }
    for (size_t i = 0; i < d_r - spill; i++)
    {
        size_t curr;
        if (!pivot_r_pool.empty())
        {
             curr = pivot_r_pool.back();
             pivot_r_pool.pop_back();
        }
        else if (!pivot_pool.empty())
        {
             curr = pivot_pool.back();
             pivot_pool.pop_back();
        }
        else
        {
             curr = pivot_l_pool.back();
             pivot_l_pool.pop_back();
        }
        subdomain_r.push_back(curr);
    }
    for (size_t i = 0; i < spill; i++)
    {
        size_t curr;
        if (!pivot_pool.empty())
        {
             curr = pivot_pool.back();
             pivot_pool.pop_back();
        }
        else if (!pivot_l_pool.empty())
        {
             curr = pivot_l_pool.back();
             pivot_l_pool.pop_back();
        }
        else
        {
             curr = pivot_r_pool.back();
             pivot_r_pool.pop_back();
        }
        subdomain_l.push_back(curr);
        subdomain_r.push_back(curr);
    }
    BSPTreeNode<Label, T> * result = new BSPTreeNode<Label, T>
            (mx_var_dir, pivot, domain);
    result->set_left(build_tree(c, a, st, subdomain_l));
    result->set_right(build_tree(c, a, st, subdomain_r));
    return result;
}

/* Private Functions */

template<class Label, class T>
BSPSpillTree<Label, T>::BSPSpillTree(DataSet<Label, T> & st) :
  BSPTree<Label, T>(st)
{ }

/* Public Functions */

template<class Label, class T>
BSPSpillTree<Label, T>::BSPSpillTree(size_t c, double a, DataSet<Label, T> & st) :
  BSPTree<Label, T>(st)
{ this->set_root(build_tree(c, a, st, st.get_domain())); }

template<class Label, class T>
BSPSpillTree<Label, T>::BSPSpillTree(ifstream & in, DataSet<Label, T> & st) :
  BSPTree<Label, T>(in, st)
{ }

#endif
