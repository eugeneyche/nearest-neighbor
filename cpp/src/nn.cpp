#include "nn.h"
#include <iostream>
#include <time.h>
using namespace std;

euclid_vector * nn(euclid_vector * query, data_set & data)
{
    euclid_vector * mn = NULL;
    double mn_dist = 0;
    double l_dist = 0;
    for (int i = 0; i < data.size(); i++)
    {
        l_dist = distance_to(*query, *data[i]);
        if (mn == NULL || l_dist < mn_dist)
        {
            mn_dist = l_dist;
            mn = data[i];
        }
    }
    return mn;
}

data_set k_nn(int k, euclid_vector * query, data_set & data)
{
    map <euclid_vector * , double> dist_mp;
    vector <double> dist_vtr; 
    for (int i = 0; i < data.size(); i++)
    {
        double dist = distance_to(*query, *(data[i]));
        dist_mp[data[i]] = dist;
        dist_vtr.push_back(dist);
    }
    double k_dist = selector(dist_vtr, k);
    vector <int> domain;
    for (int i = 0; domain.size() < k && i < data.size(); i++)
    {
        if (dist_mp[data[i]] <= k_dist)
        {
            vector <int>::iterator itr = domain.begin();
            while (itr != domain.end() && dist_mp[data[*itr]] < dist_mp[data[i]])
            {
                itr++;
            }
            if (itr != domain.end())
                domain.insert(itr, i);
            else
                domain.push_back(i);
        }
    }
    return data.subset(domain);
}

data_set c_approx_nn(double c, euclid_vector * query, data_set & data, euclid_vector * nn)
{
    map <euclid_vector * , double> dist_mp;
    for (int i = 0; i < data.size(); i++)
    {
        double dist = distance_to(*query, *(data[i]));
        dist_mp[data[i]] = dist;
    }
    double dist = distance_to(*query, *nn);
    double c_distance = c * dist;
    vector <int> domain;
    for (int i = 0; i < data.size(); i++)
    {
        if (dist_mp[data[i]] <= c_distance)
        {
            domain.push_back(i);
        }
    }
    data_set c_approx = data.subset(domain);
    return c_approx;
}

euclid_vector * kd_tree_nn(euclid_vector * query, data_set & data, kd_tree_node * root)
{
    data_set subset = data.subset(kd_subdomain(query, root));
    return nn(query, subset);
}

data_set kd_tree_k_nn(int k, euclid_vector * query, data_set & data, kd_tree_node * root)
{
    data_set subset = data.subset(kd_subdomain(query, root));
    return k_nn(k, query, subset);
}

euclid_vector * virtual_spill_tree_nn(euclid_vector * query, data_set & data, virtual_spill_tree_node * root)
{
    data_set subset = data.subset(virtual_spill_subdomain(query, root));
    return nn(query, subset);
}

data_set virtual_spill_tree_k_nn(int k, euclid_vector * query, data_set & data, virtual_spill_tree_node * root)
{
    data_set subset = data.subset(virtual_spill_subdomain(query, root));
    return k_nn(k, query, subset);
}
