#include "bundle.h"

Bundle::Bundle(int label, vector <double> vtr)
{
    this->label = label;
    this->vtr = vtr;
}

double Bundle::distance_to(const Bundle & other) const
{
    if (vtr.size() != other.get_vtr().size())
        return -1;
    double sum = 0;
    for (int i = 0; i < vtr.size(); i++)
    {
        double l_dist = vtr[i] - other.get_vtr()[i];
        sum += l_dist * l_dist;
    }
    return sum;
}

const int & Bundle::get_label() const
{
    return label;
}

const vector <double> & Bundle::get_vtr() const
{
    return vtr;
}
