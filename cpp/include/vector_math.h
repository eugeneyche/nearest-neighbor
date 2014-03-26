#ifndef _VECTOR_MATH_H
#define _VECTOR_MATH_H

#include <vector>
#include <random>
using namespace std;

/* gets the squared distance between two vectors */
template<class T>
double distance_to(vector<T> & v1, vector<T> & v2)
{
    if (v1.size() != v2.size())
        return -1;
    long double distance = 0;
    for (int i = 0; i < v1.size(); i++)
    {
        long double d = (long double)v2[i] - (long double)v1[i];
        distance += d * d;
    }
    return distance;
}

/* finds the kth smallest */
template<class T>
T selector(vector<T> st, size_t k)
{
	srand(int(time(NULL))); /* random seed */
	size_t sz = st.size();
	double randomIndex = rand() % sz; /* random number between 0 and size-1 */
    
	vector<T> left; /* data that are smaller than v */
	vector<T> right; /* data that are larger than v */
	vector<T> v; /* data that are equal to v */
    
    /* iterate through to push the smaller to left, larger to right, the rest to v */
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
    
    /* three conditions */
	if (left.size() >= k)
    { /* the kth smallest on the left */
		return selector(left, k);
	}
	else if(left.size() + v.size() >= k)
    { /* the kth smallest found */
		return st[randomIndex];
	}
	else
    { /* the kth smallest on the right */
		return selector(right, (size_t)(k-left.size() - v.size()));
	}
}

#endif
