#ifndef _NN_H
#define _NN_H

#include "data_set.h"
#include <map>
#include <vector>
using namespace std;

template<class Label, class T>
vector<T>  * nearest_neighbor(vector<T> * query, DataSet<Label, T> st)
{
    static map<vector<T> *, vector<T> *> mem; 
    if (mem[query])
        return mem[query];
    vector<T> * mn_vtr = NULL;
    double mn_dist = 0;
    double l_dist = 0;
    for (int i = 0; i < st.size(); i++)
    {
        l_dist = distance_to(*query, *st[i]);
        if (mn_vtr == NULL || l_dist < mn_dist)
        {
            mn_dist = l_dist;
            mn_vtr = st[i];
        }
    }
    mem[query] = mn_vtr;
    return mn_vtr;
}

template<class Label, class T>
DataSet<Label, T> k_nearest_neighbor(int k, vector<T> * query, DataSet<Label, T> st)
{
    map<vector<T> *, double> dist_mp; 
    vector<double> dist_vtr;
    for (int i = 0; i < st.size(); i++)
    {
        double dist = distance_to(*query, *st[i]);
        dist_mp[st[i]] = dist;
        dist_vtr.push_back(dist);
    }
    double k_dist = selector(dist_vtr, k);
    vector<size_t> domain;
    for (int i = 0; domain.size() < k && i < st.size(); i++)
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

#endif
