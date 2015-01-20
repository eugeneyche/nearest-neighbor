/* 
 * File     : nn.h
 * Date     : 14-1-2015
 * Summary  : Basic nearest neighbor algorithms.
 */
#ifndef NN_H
#define NN_H

#include <map>
#include <vector>
using namespace std;

#include "data_set.h"

template<class Label, class T>
vector<T> * 
nearest_neighbor(const DataSet<Label, T>::Vector & query, const DataSet<Label, T> & data)
{
    DataSet<Label, T>::Vector * mn_vtr = NULL;
    double mn_dist = 0;
    double l_dist = 0;
    for (size_t i = 0; i < data.size(); i++) {
        l_dist = distance_to(query, data[i]);
        if (!mn_vtr || l_dist < mn_dist) {
            mn_dist = l_dist;
            mn_vtr = &st[i];
        }
    }
    return mn_vtr;
}

template<class Label, class T>
DataSet<Label, T> 
k_nearest_neighbor(size_t k, const DataSet<Label, T>::Vector & query, DataSet<Label, T> & data)
{
    map<DataSet<Label, T>::Vector *, double> dist_mp; 
    vector<double> dist_vtr;
    for (size_t i = 0; i < data.size(); i++) {
        double dist = distance_to(query, data[i]);
        dist_mp[&st[i]] = dist;
        dist_vtr.push_back(dist);
    }
    double k_dist = selector(dist_vtr, k);
    vector<size_t> domain;
    for (size_t i = 0; domain.size() < k && i < data.size(); i++) {
        if (dist_mp[&st[i]] <= k_dist) {
            vector<size_t>::iterator itr;
            for (itr = domain.begin(); itr != domain.end() && dist_mp[st[*itr]] < dist_mp[st[i]]; itr++);
            if (itr != domain.end())
                domain.insert(itr, i);
            else
                domain.push_back(i);
        }
    }
    return data.subset(domain);
}

template<class Label, class T>
DataSet<Label, T> 
c_approx_nn(double c, DataSet<Label, T>::Vector & query, DataSet<Label, T> & data, 
        vector<T> * nn)
{
    map<DataSet<Label, T>::Vector * , double> dist_mp;
    for (size_t i = 0; i < st.size(); i++) {
        double dist = distance_to(query, data[i]);
        dist_mp[&st[i]] = dist;
    }
    double dist = distance_to(query, nn);
    double c_distance = c * dist;
    vector<size_t> domain;
    for (size_t i = 0; i < data.size(); i++) {
        if (dist_mp[&st[i]] <= c_distance) {
            domain.push_back(i);
        }
    }
    return data.subset(domain);
}

#endif /* NN_H */
