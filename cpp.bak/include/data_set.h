#ifndef _DATA_SET_H
#define _DATA_SET_H

#include "vector_math.h"
#include <fstream>
#include <map>
using namespace std;


template <class Label, class T>
class DataSet;

/* loads the vectors from a file */
template <class Label, class T>
void load_data_set(DataSet<Label, T> & st, ifstream & in);
/* loads the labels from a file */
template <class Label, class T>
void label_data_set(DataSet<Label, T> & st, ifstream & in);
/* return the index of the vector with the max variance */
template <class Label, class T>
int max_variance_index(DataSet<Label, T> & sub); 

template <class Label, class T>
class DataSet
{
    typedef unsigned int size_type
    typedef map<vector<T> *, Label> label_space;
    typedef vector<vector<T> *> vector_space;
private:
    DataSet<Label, T> * _parent;
    label_space * _labels;
    vector_space * _vectors;
    vector<size_type> _domain;
    DataSet(DataSet<Label, T> & parent, vector<size_type> domain);
public:
    DataSet();
    DataSet(vector_space vectors);
    ~DataSet();
    unsigned int size();
    void set_label(size_type i, Label label);
    void set_label(vector<T> * vtr, Label label);
    int get_label(size_type i);
    int get_label(vector<T> * vtr);
    vector<size_type> get_domain();
    euclid_vector * operator[](size_type index);
    DataSet<Label, T> subset(vector<size_type> domain);
    friend void load_data_set(DataSet<Label, T> & st, ifstream & in);
    friend void label_data_set(DataSet<Label, T> & st, ifstream & in);
};

#endif
