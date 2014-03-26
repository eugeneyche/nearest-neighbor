#ifndef _SPILL_TREE_H
#define _SPILL_TREE_H

#include "kd_tree.h"
using namespace std;

template<class Label, class T>
class SpillTree : public KDTree<Label, T>
{
    using KDTree<Label, T>::_root;
public:
    SpillTree(DataSet<Label, T> st);    
    SpillTree(size_t c, double a, DataSet<Label, T> st);    
    SpillTree(istream & in, DataSet<Label, T> & st);
    ~SpillTree();
};

#endif
