#ifndef _VECTOR_MATH_H
#define _VECTOR_MATH_H

#include <vector>
#include <random>
using namespace std;

template<class T>
double distance_to(const vector<T> * v1, const vector<T> * v2)
{
    if (v1->size() != v2->size())
        return -1;
    long double distance = 0;
    for (size_t i = 0; i < v1->size(); i++)
    {
        long double d = (long double)(*v2)[i] - (long double)(*v1)[i];
        distance += d * d;
    }
    return distance;
}

template<class T>
T selector(vector<T> st, size_t k)
{
	srand(int(time(NULL)));
	size_t sz = st.size();
	double randomIndex = rand() % sz;
    
	vector<T> left;
	vector<T> right;
	vector<T> v;
    
    typename vector<T>::iterator itr;
	for (itr = st.begin(); itr != st.end(); itr++)
	{
		if(*itr == st[randomIndex]){
			v.push_back(*itr);
		}
		else if(*itr < st[randomIndex]){
			left.push_back(*itr);
		}
		else{
			right.push_back(*itr);
		}
	}
    
	if (left.size() >= k)
    {
		return selector(left, k);
	}
	else if(left.size() + v.size() >= k)
    {
		return st[randomIndex];
	}
	else
    {
		return selector(right, (size_t)(k-left.size() - v.size()));
	}
}

#endif
