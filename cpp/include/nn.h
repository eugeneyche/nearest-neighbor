#ifndef _NN_H
#define _NN_H

#include "data_set.h"
#include <map>
#include <vector>
using namespace std;

template<class Label, class T>
vector<T> * nearest_neighbor(const vector<T> * query, const DataSet<Label, T> & st)
{
    vector<T> * mn_vtr = NULL;
    double mn_dist = 0;
    double l_dist = 0;
    for (size_t i = 0; i < st.size(); i++)
    {
        l_dist = distance_to(query, st[i]);
        if (!mn_vtr || l_dist < mn_dist)
        {
            mn_dist = l_dist;
            mn_vtr = st[i];
        }
    }
    return mn_vtr;
}

template<class Label, class T>
DataSet<Label, T> k_nearest_neighbor(size_t k, vector<T> * query, DataSet<Label, T> & st)
{
    map<vector<T> *, double> dist_mp; 
    vector<double> dist_vtr;
    for (size_t i = 0; i < st.size(); i++)
    {
        double dist = distance_to(*query, *st[i]);
        dist_mp[st[i]] = dist;
        dist_vtr.push_back(dist);
    }
    double k_dist = selector(dist_vtr, k);
    vector<size_t> domain;
    for (size_t i = 0; domain.size() < k && i < st.size(); i++)
    {
        if (dist_mp[st[i]] <= k_dist)
        {
            vector<size_t>::iterator itr;
            for (itr = domain.begin(); itr != domain.end() && dist_mp[st[*itr]] < dist_mp[st[i]]; itr++);
            if (itr != domain.end())
                domain.insert(itr, i);
            else
                domain.push_back(i);
        }
    }
    return st.subset(domain);
}

template<class Label, class T>
DataSet<Label, T> c_approx_nn(double c, vector<T> * query, DataSet<Label, T> & st, 
        vector<T> * nn)
{
    map<vector<T> * , double> dist_mp;
    for (size_t i = 0; i < st.size(); i++)
    {
        double dist = distance_to(query, st[i]);
        dist_mp[st[i]] = dist;
    }
    double dist = distance_to(query, nn);
    double c_distance = c * dist;
    vector<size_t> domain;
    for (size_t i = 0; i < st.size(); i++)
    {
        if (dist_mp[st[i]] <= c_distance)
        {
            domain.push_back(i);
        }
    }
    DataSet<Label, T> & c_approx = st.subset(domain);
    return c_approx;
}

#endif
