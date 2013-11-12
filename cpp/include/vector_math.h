#ifndef _VECTOR_MATH_H
#define _VECTOR_MATH_H

#include <vector>
using namespace std;

typedef vector <double> euclid_vector;

double magnitude(const euclid_vector & vtr);
euclid_vector scale(const double & factor, const euclid_vector & vtr);
euclid_vector add(const euclid_vector & vtr1, const euclid_vector & vtr2);
double dot_product(const euclid_vector & vtr1, const euclid_vector & vtr2);
double distance_to(const euclid_vector & vtr1, const euclid_vector & vtr2);

#endif
