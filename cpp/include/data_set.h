#ifndef _DATA_SET_H
#define _DATA_SET_H

#include "vector_math.h"
#include <iostream>
#include <fstream>
#include <map>
using namespace std;

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
    DataSet(ifstream & in);
    ~DataSet();
    size_t size() const;
    void label(ifstream & in);
    int get_label(size_t index) const;
    int get_label(vector<T> * vtr) const;
    vector<size_t> get_domain() const;
    vector<T> * operator[](size_t index) const;
    DataSet<Label, T> subset(vector<size_t> domain);
};

template<class Label, class T>
size_t max_variance_index(DataSet<Label, T> & subset)
{
    vector<double> var;
    vector<T> vtr;
    size_t dimension = subset[0]->size();
    size_t subsize = subset.size();
    for (size_t i = 0; i < dimension; i++)
    {
        vtr.clear();
        for (size_t j = 0; j < subsize; j++)
        {
            vtr.push_back((*subset[j])[i]);
        }
        T median = selector(vtr, (size_t)(subset.size() * 0.5));
        double variance = 0.0;
        for (size_t j = 0; j < subsize; j++)
        {
            double dif = (double)(*subset[j])[i] - (double)median;
            variance += dif * dif;
        }
        variance = variance / subsize;
        var.push_back(variance);
    }
    size_t maxIndex = 0;
    for (size_t i = 1; i < dimension; i++)
    {
        if (var[i] > var[maxIndex])
        {
            maxIndex = i;
        }
    }
    return maxIndex;
}

template<class Label, class T>
DataSet<Label, T>::DataSet(DataSet<Label, T> & parent, vector<size_t> domain) :
  _parent (&parent),
  _labels (parent._labels),
  _vectors (parent._vectors)
{
    vector<size_t>::iterator itr;
    for (itr = domain.begin(); itr != domain.end(); itr++)
    {
        _domain.push_back(parent._domain[*itr]);
    }
}

template<class Label, class T>
DataSet<Label, T>::DataSet() :
  _parent (NULL),
  _labels (new label_space),
  _vectors (new vector_space)
{ }

template<class Label, class T>
DataSet<Label, T>::DataSet(ifstream & in) :
  _parent (NULL),
  _labels (new label_space),
  _vectors (new vector_space)
{
    size_t n, m;
    in.read((char *)&n, sizeof(size_t));
    in.read((char *)&m, sizeof(size_t));
    for (size_t i = 0; i < n; i++)
    {
        vector<T> * vtr = new vector<T>;
        T buffer [m];
        in.read((char *)buffer, sizeof(T) * m);
        vtr->assign(buffer, buffer + m);
        _domain.push_back(_domain.size());
        _vectors->push_back(vtr);
    }
}

template<class Label, class T>
DataSet<Label, T>::DataSet(vector_space vectors) :
  _parent (NULL),
  _labels (new label_space),
  _vectors (new vector_space)
{
    typename vector_space::iterator itr;
    for (itr = vectors.begin(); itr != vectors.end(); itr++)
    {
        _domain.push_back((size_t)_domain.size());
        _vectors->push_back(*itr);
    }
}

template<class Label, class T>
DataSet<Label, T>::~DataSet()
{
    if (_parent == NULL)
    {
        while (_vectors->size() > 0)
        {
            delete _vectors->back();
            _vectors->pop_back();
        }
        delete _labels;
        delete _vectors;
    }
}

template<class Label, class T>
size_t DataSet<Label, T>::size() const
{
    return _domain.size();
}

template<class Label, class T>
void DataSet<Label, T>::label(ifstream & in)
{
    size_t n;
    in.read((char *)&n, sizeof(size_t));
    Label buffer [n];
    in.read((char *)buffer, sizeof(Label) * n);
    for (size_t i = 0; i < n; i++)
    {
        (*_labels)[(*this)[i]] = buffer[i];
    }
}

template<class Label, class T>
int DataSet<Label, T>::get_label(size_t index) const
{
    return get_label((*this)[index]);
}

template<class Label, class T>
int DataSet<Label, T>::get_label(vector<T> * vtr) const
{
    return (*_labels)[vtr];
}

template<class Label, class T>
vector<size_t> DataSet<Label, T>::get_domain() const
{
    return _domain;
}

template<class Label, class T>
vector<T> * DataSet<Label, T>::operator[](size_t index) const
{
    return (*_vectors)[_domain[index]];
}

template<class Label, class T>
DataSet<Label, T> DataSet<Label, T>::subset(vector<size_t> domain)
{
    return DataSet<Label, T>(*this, domain);
}
#endif
