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

#include "vmath.h"
#include "dataset.h"

template<class T>
const vector<T> &
nearest_neighbor(const typename DataSet<T>::Vector &query, const DataSet<T> &data)
{
    const typename DataSet<T>::Vector *mn_vtr = nullptr;
    double mn_dist = 0;
    double l_dist = 0;
    for (size_t it = 0; it < data.size(); it++) {
        l_dist = distance_to(query, data[it]);
        if (!mn_vtr || l_dist < mn_dist) {
            mn_dist = l_dist;
            mn_vtr = &data[it];
        }
    }
    return *mn_vtr;
}

template<class T>
DataSet<T> 
k_nearest_neighbor(const typename DataSet<T>::Vector &query, DataSet<T> &data, size_t k)
{
    map<const typename DataSet<T>::Vector *, double> dist_mp; 
    vector<double> dist_vtr;
    for (size_t it = 0; it < data.size(); it++) {
        double dist = distance_to(query, data[it]);
        dist_mp[&data[it]] = dist;
        dist_vtr.push_back(dist);
    }
    double k_dist = selector(dist_vtr, k);
    fprintf(stderr, "dist %lf\n", k_dist);
    typename DataSet<T>::DomainSpace domain;
    for (size_t it = 0; domain.size() < k && it < data.size(); it++) {
        if (dist_mp[&data[it]] <= k_dist) {
            typename DataSet<T>::DomainSpace::iterator jt;
            for (jt = domain.begin(); jt != domain.end() && 
                    dist_mp[&data[*jt]] < dist_mp[&data[it]]; jt++) {
            }
            domain.insert(jt, it);
        }
    }
    fprintf(stderr, "size %lu\n", domain.size());
    return data.subset(domain);
}

template<class T>
DataSet<T> 
c_approx_nn(typename DataSet<T>::Vector &query, DataSet<T> & data, 
        const typename DataSet<T>::Vector nn, double c)
{
    map<typename DataSet<T>::Vector *, double> dist_mp;
    for (size_t it = 0; it < data.size(); it++) {
        double dist = distance_to(query, data[it]);
        dist_mp[&data[it]] = dist;
    }
    double dist = distance_to(query, nn);
    double c_distance = c * dist;
    vector<size_t> domain;
    for (size_t it = 0; it < data.size(); it++) {
        if (dist_mp[&data[it]] <= c_distance) {
            domain.push_back(it);
        }
    }
    return data.subset(domain);
}

#endif /* NN_H */
