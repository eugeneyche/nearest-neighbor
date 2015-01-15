#ifndef MATH_H
#define MATH_H

#include <vector>
#include <random>

using namespace std;

template<class T>
double distance_to(const vector<T> * v1, const vector<T> * v2)
{
    if (v1->size() != v2->size())
        return -1;
    double distance = 0;
    for (size_t i = 0; i < v1->size(); i++)
    {
        double d = (double)(*v2)[i] - (double)(*v1)[i];
        distance += d * d;
    }
    return distance;
}

template<class A, class B>
double dot(vector<A> v, vector<B> vd)
{
    long double factor = 0;;
    for (int i = 0; i < v.size() && i < vd.size(); i++)
        factor += v[i] * vd[i];
    return factor;
}

template<class T>
T selector(const vector<T> & st, size_t k)
{
	srand(int(time(NULL)));
	size_t sz = st.size();
	double rand_idx= rand() % sz;
    
	vector<T> left;
	vector<T> right;
	vector<T> v;
    
    typename const vector<T>::iterator itr;
	for (itr = st.begin(); itr != st.end(); itr++)
	{
		if(*itr == st[rand_idx])
			v.push_back(*itr);
		else if(*itr < st[rand_idx])
			left.push_back(*itr);
		else
			right.push_back(*itr);
	}
    
	if (left.size() >= k)
		return selector(left, k);
	else if (left.size() + v.size() >= k)
		return st[rand_idx];
	else
		return selector(right, k - left.size() - v.size());
}


#endif
