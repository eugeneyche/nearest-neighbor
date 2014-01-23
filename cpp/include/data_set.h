#ifndef _DATA_SET_H
#define _DATA_SET_H

#include "vector_math.h"
#include <cstdio>
#include <map>
#include <iostream>
using namespace std;

typedef map <euclid_vector *, int> label_space;
typedef vector <euclid_vector *> vector_space;

class data_set;

/* loads the vectors from a file */
void load(data_set & st, FILE * in);
/* loads the labels from a file */
void label(data_set & st, FILE * in);
/* return the index of the vector with the max variance */
int max_variance_index(int k, data_set & sub); 

class data_set
{
private:
    data_set * _parent;
    label_space * _labels;
    vector_space * _vectors;
    vector <int> _domain;
    data_set(data_set & parent, vector <int> domain);
public:
    data_set();
    data_set(vector_space vectors);
    ~data_set();
    int size();
    void set_label(int i, int label);
    void set_label(euclid_vector * vtr, int label);
    int get_label(int i);
    int get_label(euclid_vector * vtr);
    vector <int> get_domain();
    vector <int> get_abs_domain();
    euclid_vector * operator[](int i);
    data_set subset(vector <int> domain);
    friend void load(data_set & st, FILE * in);
    friend void label(data_set & st, FILE * in);
};

#endif
