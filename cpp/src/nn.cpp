#include "nn.h"
#include <iostream>
#include <time.h>

using namespace std;



euclid_vector * nn(euclid_vector * test, data_set & train_set)
{
    euclid_vector * mn = NULL;
    double mn_dist = 0;
    double l_dist = 0;
    for (int i = 0; i < train_set.size(); i++)
    {
        l_dist = distance_to(*test, *train_set[i]);
        if (mn == NULL || l_dist < mn_dist)
        {
            mn_dist = l_dist;
            mn = train_set[i];
        }
    }
    return mn;
}




/* return the nearest neighbor that is generated from kd-tree */
euclid_vector * kd_tree_nn(euclid_vector *test, data_set & train_set, int c)
{
    kd_tree_node root = kd_tree(c, train_set);
    data_set train = train_set.subset(sub_domain(*test, root));
    euclid_vector * mn = NULL;
    double mn_dist = 0;
    double l_dist = 0;
    for (int i = 0; i < train_set.size(); i++)
    {
        l_dist = distance_to(*test, *train_set[i]);
        if (mn == NULL || l_dist < mn_dist)
        {
            mn_dist = l_dist;
            mn = train_set[i];
        }
    }
    return mn;
}