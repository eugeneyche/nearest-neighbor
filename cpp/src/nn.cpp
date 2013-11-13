#include "nn.h"

euclid_vector * nn(euclid_vector * test, data_set & train_set)
{
    euclid_vector * mn = NULL;
    double mn_dist = 0;
    for (vector_space::iterator itr = train_set.begin(); itr < train_set.end(); itr++)
    {
        double l_dist;
        if (nn == NULL || (l_dist = distance_to(*test, **itr)) < mn_dist)
        {
            mn_dist = l_dist;
            mn = *itr;
        }
    }
    return mn;
}
