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


data_set k_nn(euclid_vector * test, data_set & train_st, double k)
{
    map <euclid_vector * , double> dist_mp;
    vector <double> dist_vtr; 
    for (int i = 0; i < train_st.size(); i++)
    {
        double dist = distance_to(*test, *(train_st[i]));
        dist_mp[train_st[i]] = dist;
        dist_vtr.push_back(dist);
    }
    double k_dist = selector(dist_vtr, k);
    vector <int> domain;
    for (int i = 0; domain.size() < k && i < train_st.size(); i++)
    {
        if (dist_mp[train_st[i]] <= k_dist)
        {
            vector <int>::iterator itr = domain.begin();
            while (itr != domain.end() && dist_mp[train_st[*itr]] < dist_mp[train_st[i]])
            {
                itr++;
            }
            if (itr != domain.end())
                domain.insert(itr, i);
            else
                domain.push_back(i);
        }
    }
    train_st.subset(domain);
    return data_set();
}

/* return the nearest neighbor that is generated from kd-tree */
euclid_vector * kd_tree_nn(euclid_vector * test, data_set & train_set, int c, kd_tree_node * root)
{
    data_set train = train_set.subset(sub_domain(test, root));
    return nn(test, train);
}


/* return the data_set of all vector within c*distance of the query */
data_set c_approx_nn(euclid_vector * test, data_set & train_st, euclid_vector * nn, double c)
{
    map <euclid_vector * , double> dist_mp;
    for (int i = 0; i < train_st.size(); i++)
    {
        double dist = distance_to(*test, *(train_st[i]));
        dist_mp[train_st[i]] = dist;
    }
    double dist = distance_to(*test, *nn);
    double c_distance = c * dist;
    vector <int> domain;
    for (int i = 0; i < train_st.size(); i++)
    {
        if (dist_mp[train_st[i]] <= c_distance)
        {
            domain.push_back(i);
        }
    }
    data_set c_approx = train_st.subset(domain);
    return c_approx;
}


/* return the nearest neighbor that is generated from kd-tree by spill the query */
euclid_vector * spill_query_nn(euclid_vector * test, data_set * train, spill_tree_node * spill_root)
{
    vector_space vector_set;
    vector_space queries = query_nn_set(vector_set, train, test, spill_root);
    data_set * set = new data_set(queries);
    return nn(test, * set);
}
