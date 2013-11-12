#include "data_set.h"

void data_set::add_vector(euclid_vector vtr)
{
    _vectors.push_back(vtr);
}

void data_set::set_label(int label, euclid_vector * vtr)
{
    _labels[vtr] = label;
}

int data_set::get_label(euclid_vector * vtr)
{
    return _labels[vtr];
}

vector <euclid_vector *> data_set::domain()
{
    vector <euclid_vector *> domain;
    for (int i = 0; i < _vectors.size(); i++)
    {
        domain.push_back(&_vectors[i]);
    }
    return domain;
}


