#ifndef KD_SPILL_TREE_H_
#define KD_SPILL_TREE_H_

#include "kd_tree.h"

#define MIN(x, y) (x) < (y) ? (x) : (y)

template<class Label, class T>
class KDSpillTree;

template<class Label, class T>
class KDSpillTree : public KDTree<Label, T>
{
private:
    static KDTreeNode<Label, T> * build_tree(size_t c, double a,
            DataSet<Label, T> & st, vector<size_t> domain);
public:
    KDSpillTree(DataSet<Label, T> & st);
    KDSpillTree(size_t c, double a, DataSet<Label, T> & st);
    KDSpillTree(ifstream & in, DataSet<Label, T> & st);
};

template<class Label, class T>
KDTreeNode<Label, T> * KDSpillTree<Label, T>::build_tree(size_t c, double a,
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
    #ifdef DEBUG
        cerr << "[DEBUG: dl - " << d_l << "]" << endl;
        cerr << "[DEBUG: dr - " << d_r << "]" << endl;
        cerr << "[DEBUG: spill - " << spill << "]" << endl;
    #endif
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
    KDTreeNode<Label, T> * result = new KDTreeNode<Label, T>
            (mx_var_index, pivot, domain);
    result->set_left(build_tree(c, a, st, subdomain_l));
    result->set_right(build_tree(c, a, st, subdomain_r));
    return result;
}
template<class Label, class T>
KDSpillTree<Label, T>::KDSpillTree(DataSet<Label, T> & st) :
  KDTree<Label, T>(st)
{ }

template<class Label, class T>
KDSpillTree<Label, T>::KDSpillTree(size_t c, double a, DataSet<Label, T> & st) :
  KDTree<Label, T>(st)
{ this->set_root(build_tree(c, a, st, st.get_domain())); }

template<class Label, class T>
KDSpillTree<Label, T>::KDSpillTree(ifstream & in, DataSet<Label, T> & st) :
  KDTree<Label, T>(in, st)
{ }

#endif
