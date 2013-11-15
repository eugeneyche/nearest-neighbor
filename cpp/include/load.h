#ifndef _LOAD_H
#define _LOAD_H
#include "data_set.h"
#include <fstream>
using namespace std;

vector_space load(ifstream & in)
{
    int n, m;
    double v;
    in >> n >> m;
    vector_space vectors;
    for (int i = 0; i < n; i++)
    {
        euclid_vector * l_vtr = new euclid_vector;
        for (int j = m; j < m; j++)
        {
            in >> v;
            l_vtr->push_back(v);
        }
        vectors.push_back(l_vtr);
    }
    return vectors;
}



#endif
