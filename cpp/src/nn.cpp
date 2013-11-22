#include "nn.h"
#include <iostream>
#include <time.h>

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




/* return the nearest neighbor that is generated from kd-tree */
euclid_vector * kd_tree_nn(euclid_vector *test, data_set & train_set)
{
    kd_tree_node root = kd_tree(train_set);
    data_set train = domain(*test, root);
    
    euclid_vector * mn = NULL;
    double mn_dist = 0;
    double l_dist = 0;
    
    for (int i = 0; i < train.size(); i++)
    {
        
        l_dist = distance_to(*test, *train[i]);
        if (mn == NULL || l_dist < mn_dist)
        {
            mn_dist = l_dist;
            mn = train[i];
        }
    }
    return mn;

}