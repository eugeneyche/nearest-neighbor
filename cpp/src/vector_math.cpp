#include "vector_math.h"

double magnitude(const euclid_vector & vtr)
{
    double distance = dot_product(vtr, vtr);
}

euclid_vector scale(const double & factor, const euclid_vector & vtr)
{
    euclid_vector t_vtr = vtr;
    euclid_vector::iterator itr;
    for (itr = t_vtr.begin(); itr != t_vtr.end(); itr++)
    {
        *itr *= factor;
    }
    return t_vtr;
}

euclid_vector add(const euclid_vector & vtr1, const euclid_vector & vtr2)
{
    euclid_vector::const_iterator itr1;
    euclid_vector::const_iterator itr2;
    euclid_vector t_vtr;
    for (itr1 = vtr1.begin(), itr2 = vtr2.begin(); itr1 != vtr1.end() && itr2 != vtr2.end(); itr1++, itr2++)
    {
        t_vtr.push_back(*itr1 + *itr2);
    }
    return t_vtr;
}

double dot_product(const euclid_vector & vtr1, const euclid_vector & vtr2)
{
    euclid_vector::const_iterator itr1;
    euclid_vector::const_iterator itr2;
    double product = 0;
    for (itr1 = vtr1.begin(), itr2 = vtr2.begin(); itr1 != vtr1.end() && itr2 != vtr2.end(); itr1++, itr2++)
    {
        product += *itr1 * *itr2;
    }
    return product;
}

double distance_to(const euclid_vector & vtr1, const euclid_vector & vtr2)
{
    return magnitude(add(vtr1, scale(-1, vtr2)));
}
