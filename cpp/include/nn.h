#ifndef _NN_H
#define _NN_H

#include "data_set.h"
using namespace std;

template<class Label, class T>
vector<T>  * nearest_neighbor(vector<T> * query, DataSet<Label, T> st);

template<class Label, class T>
DataSet<Label, T> k_nearest_neighbor(int k, vector<T> * query, DataSet<Label, T> st);

template<class Label, class T>
DataSet<Label, T> c_approximate_nearest_neighbor(double c, vector<T> * nn);

#endif
