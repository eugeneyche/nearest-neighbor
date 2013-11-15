#include "vector_math.h"

double distance_to( euclid_vector & v1, euclid_vector & v2)
{
    if (v1.size() != v2.size())
        return -1;
    double distance = 0;
    for (int i = 0; i < v1.size(); i++)
    {
        double d = v2[i] - v1[i];
        distance += d * d;
    }
    return distance;
}
