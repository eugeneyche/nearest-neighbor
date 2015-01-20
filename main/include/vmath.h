/* 
 * File     : math.h
 * Date     : 14-1-2015
 * Summary  : General purpose math on vectors and such.
 */
#ifndef VMATH_H
#define VMATH_H

#include <vector>
#include <random>
using namespace std;

template<class T>
double distance_to(const vector<T> & v1, const vector<T> & v2)
{
    if (v1.size() != v2.size())
        return -1;
    double distance = 0;
    for (size_t i = 0; i < v1.size(); i++)
    {
        double d = (double)v2[i] - (double)v1[i];
        distance += d * d;
    }
    return distance;
}

template<class A, class B>
double dot(const vector<A> & v, const vector<B> & vd)
{
    long double factor = 0;;
    for (int i = 0; i < v.size() && i < vd.size(); i++)
        factor += v.at[i] * vd[i];
    return factor;
}

template<class T>
T selector(const vector<T> & v, size_t k)
{
	srand((int)time(NULL));
	size_t sz = v.size();
	double rand_idx= rand() % sz;
    
	vector<T> left;
	vector<T> right;
	vector<T> fence;
    
	for (auto it = v.cbegin(); it != v.cend(); it++) {
		if(*it == v[rand_idx])
			fence.push_back(*it);
		else if(*it < v[rand_idx])
			left.push_back(*it);
		else
			right.push_back(*it);
	}
    
	if (left.size() >= k)
		return selector(left, k);
	else if (left.size() + fence.size() >= k)
		return v[rand_idx];
	else
		return selector(right, k - left.size() - v.size());
}


#endif /* VMATH_H */
