#ifndef _DATA_SET_H
#define _DATA_SET_H
#include "vector_math.h"
#include <map>
using namespace std;

typedef map <euclid_vector *, int> label_space;
typedef vector <euclid_vector> vector_space;

class data_set
{
private:
    label_space _labels;
    vector_space _vectors;
public:
    void add_vector(euclid_vector vtr);
    void set_label(int label, euclid_vector * vtr);
    int get_label(euclid_vector * vtr);
    vector <euclid_vector *> domain();
};

#endif
