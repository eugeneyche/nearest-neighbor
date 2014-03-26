#include "virtual_spill_tree.h"
#include <queue>
using namespace std;

template<class Label, class T>
VirtualSpillTree<Label, T>::VirtualSpillTree
        (DataSet<Label, T> &st) :
  KDTree<Label, T>(st)
{ }

template<class Label, class T> 
VirtualSpillTree<Label, T>::VirtualSpillTree
        (size_t c, double a, DataSet<Label, T> & st) :
  KDTree<Label, T>(c, st)
{ 
    queue<KDTreeNode<Label, T> *> to_load;
    to_load.push(_root);
    while (!to_load.empty())
    {
        KDTreeNode<Label, T> * cur = to_load.front();
        DataSet<Label, T> subst = _st.subset(cur->get_domain());
        int mx_var_index = max_variance_index(subst);
        vector<T> values;
        for (int i = 0; i < subst.size(); i++)
        {
            values.push_back((*subst[i])[mx_var_index]);
        }
        T pivot_l, pivot_r;
        if (a > 0)
        {
            pivot_l = selector(values, (size_t)(values.size() * (0.5 - a)));
            pivot_r = selector(values, (size_t)(values.size() * (0.5 + a)));
        }
        _range_mp[cur] = range(pivot_l, pivot_r);
        to_load.push(cur->left);
        to_load.push(cur->right);
    }
}

template<class Label, class T>
VirtualSpillTree<Label, T>::VirtualSpillTree
        (istream & in, DataSet<Label, T> & st) :
    KDTree<Label, T>(in, st)
{
    queue<KDTreeNode<Label, T> *> to_load;
    to_load.push(_root);
    while (!to_load.empty())
    {
        KDTreeNode<Label, T> * cur = to_load.front();
        to_load.pop();
        bool exists = cur != NULL;
        if (exists)
        {
            DataSet<Label, T> subst = _st.subset(cur->get_domain());
            T pivot_l, pivot_r;
            in.read((char *)&pivot_l, sizeof(T));
            in.read((char *)&pivot_r, sizeof(T));
            _range_mp[cur] = range(pivot_l, pivot_r);
            to_load.push(cur->left);
            to_load.push(cur->right);
        }
    }
}

template<class Label, class T>
void VirtualSpillTree<Label, T>::save(ostream & out) const
{
    KDTree<Label, T>::save(out);
    queue<KDTreeNode<Label, T> *> to_save;
    to_save.push(_root);
    while (!to_save.empty())
    {
        KDTreeNode<Label, T> * cur = to_save.front();
        to_save.pop();
        bool exists = cur != NULL;
        if (exists)
        {
            range cur_range = _range_mp[cur];
            out.write((char *)&cur_range->first, sizeof(T));
            out.write((char *)&cur_range->second, sizeof(T));
            if (cur->right)
                to_save.push(cur->_right);
            if (cur->left)
                to_save.push(cur->_left);
        }
    }
}
