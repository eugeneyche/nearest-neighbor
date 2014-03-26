#ifndef _DATA_SET_H
#define _DATA_SET_H

#include "vector_math.h"
#include <fstream>
#include <map>
using namespace std;

template<class Label, class T>
class DataSet;

template<class Label, class T>
void label_DataSet(DataSet<Label, T> & st, istream & in);

template<class Label, class T>
size_t max_variance_index(DataSet<Label, T> & subset); 

template<class Label, class T>
class DataSet
{
    typedef map<vector<T> *, Label> label_space;
    typedef vector<vector<T> *> vector_space;
private:
    DataSet<Label, T> * _parent;
    label_space * _labels;
    vector_space * _vectors;
    vector<size_t> _domain;
    DataSet(DataSet<Label, T> & parent, vector<size_t> domain);
    DataSet(vector_space vectors);
public:
    DataSet();
    DataSet(istream & in);
    ~DataSet();
    size_t size();
    void label(istream & in);
    void set_label(size_t index, Label label);
    void set_label(vector<T> * vtr, Label label);
    int get_label(size_t index);
    int get_label(vector<T> * vtr);
    vector<size_t> get_domain();
    vector<T> * operator[](size_t index);
    DataSet<Label, T> subset(vector<size_t> domain);
};

#endif
