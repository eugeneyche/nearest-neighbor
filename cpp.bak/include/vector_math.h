#ifndef _VECTOR_MATH_H
#define _VECTOR_MATH_H

#include <vector>
using namespace std;


/* gets the squared distance between two vectors */
template <class T>
double distance_to(vector<T> & v1, vector<T> & v2);
/* finds the kth smallest */
template <class T>
double selector(vector<T> s, unsigned int k);

#endif
