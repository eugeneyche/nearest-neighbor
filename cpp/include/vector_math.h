#ifndef _VECTOR_MATH_H
#define _VECTOR_MATH_H

#include <vector>
using namespace std;

typedef vector <double> euclid_vector;

/* gets the squared distance between two vectors */
double distance_to(euclid_vector & v1, euclid_vector & v2);

#endif
