#include "nn.h"
#include <iostream>

int read_bytes(ifstream & in, int n_bytes)
{
    int res = 0;
    unsigned char byte;
    for (int i = 0; i < n_bytes; i++)
    {
        in.read((char *) &byte, 1);
        res = (res << 8) + (int)byte;
    }
    return res;
}

bool load_set(vector <Bundle> & out_vtr, ifstream & label_in, ifstream & vtr_in)
{
    read_bytes(label_in, 4);
    read_bytes(vtr_in, 4);
    int size = read_bytes(label_in, 4);
    if (size != read_bytes(vtr_in, 4))
        return false;
    int vtr_dim = read_bytes(vtr_in, 4) * read_bytes(vtr_in, 4);
    for (int i = 0; i < size; i++)
    {
        int label = read_bytes(label_in, 1);
        vector <double> vtr;
        for (int j = 0; j < vtr_dim; j++)
        {
            vtr.push_back(read_bytes(vtr_in, 1));
        }
        out_vtr.push_back(Bundle(label, vtr));
    }
    return true;
}

int nn(const Bundle & test, const vector <Bundle> & train_set, vector <int> domain)
{
    double mn_dist = -1;
    int mn = -1;
    for (int i = 0; i < domain.size(); i++)
    {
        double l_dist;
        l_dist = test.distance_to(train_set[domain[i]]);
        if (mn_dist == -1 || mn_dist > l_dist)
        {
            mn_dist = l_dist;
            mn = domain[i];
        }
    }
    return mn;
}
