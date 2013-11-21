#include "data_set.h"
#include "vector_math.h"
#include <vector>

using namespace std;

euclid_vector * nn(euclid_vector * test, data_set & train_set);

double selector(euclid_vector s, double k);
