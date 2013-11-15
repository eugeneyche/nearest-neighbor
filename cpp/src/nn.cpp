#include "nn.h"
#include <iostream>
using namespace std;

euclid_vector * nn(euclid_vector * test, data_set & train_st)
{
    euclid_vector * mn = NULL;
    double mn_dist = 0;
    double l_dist = 0;
    for (int i = 0; i < train_st.size(); i++)
    {
        l_dist = distance_to(*test, *train_st[i]);
        if (mn == NULL || l_dist < mn_dist)
        {
            mn_dist = l_dist;
            mn = train_st[i];
        }
    }
    return mn;
}
