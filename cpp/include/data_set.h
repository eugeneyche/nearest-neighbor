#ifndef _DATA_SET_H
#define _DATA_SET_H

#include "vector_math.h"
#include <map>
using namespace std;

typedef map <euclid_vector *, int> label_space;
typedef vector <euclid_vector *> vector_space;

class data_set
{
private:
    data_set * _parent;
    label_space _labels;
    vector_space _vectors;
    data_set(data_set & parent, vector <int> domain);
public:
    data_set(vector_space vectors);
    ~data_set();
    int size();
    label_space & labels();
    vector_space::iterator begin();
    vector_space::iterator end();
    euclid_vector * operator[](int i);
    vector <int> domain();
    data_set subset(vector <int> domain);
};

#endif
