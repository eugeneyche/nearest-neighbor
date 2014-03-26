#include "data_set.h"

template<class Label, class T>
size_t max_variance_index(DataSet<Label, T> & subset)
{
    vector<T> var;
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
        size_t mean = selector(vtr, subset.size() / 2);
        double variance = 0.0;
        for (size_t j = 0; j < subsize; j++)
        {
            double dif = (*subset[j])[i] - mean;
            variance += dif * dif;
        }
        variance = variance / subsize;
        var.push_back(variance);
    }
    size_t maxIndex = 0;
    for (size_t i = 1; i < dimension; i++)
    {
        if (var[i] > var[maxIndex])
            maxIndex = i;
    }
    return maxIndex;
}

/* Class Definition */
template<class Label, class T>
DataSet<Label, T>::DataSet(DataSet<Label, T> & parent, vector<size_t> domain)
{
    _parent = &parent;
    _labels = parent._labels;
    _vectors = parent._vectors;
    vector<size_t>::iterator itr;
    for (itr = domain.begin(); itr != domain.end(); itr++)
    {
        _domain.push_back(parent._domain[*itr]);
    }
}

template<class Label, class T>
DataSet<Label, T>::DataSet()
{
    _parent = NULL;
    _labels = new label_space;
    _vectors = new vector_space;
}

template<class Label, class T>
DataSet<Label, T>::DataSet(istream & in)
{
    size_t n, m;
    in >> n >> m;
    in.ignore();
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
DataSet<Label, T>::DataSet(vector_space vectors)
{
    _parent = NULL;
    _labels = new label_space;
    _vectors = new vector_space;
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
        #ifdef DEBUG
        fprintf(stderr, "[DEBUG: Deconstructing DataSet]\n");
        #endif
    }
}

template<class Label, class T>
size_t DataSet<Label, T>::size()
{
    return _domain.size();
}

template<class Label, class T>
void DataSet<Label, T>::label(istream & in)
{
    size_t n;
    in >> n;
    in.ignore();
    Label buffer [n];
    in.read((char *)buffer, sizeof(Label) * n);
    for (size_t i = 0; i < n; i++)
    {
        set_label(i, buffer[i]);
    }
}

template<class Label, class T>
void DataSet<Label, T>::set_label(size_t index, Label label)
{
    set_label((*this)[index], label);
}

template<class Label, class T>
void DataSet<Label, T>::set_label(vector<T> * vtr, Label label)
{
    (*_labels)[vtr] = label;
}

template<class Label, class T>
int DataSet<Label, T>::get_label(size_t index)
{
    return get_label((*this)[index]);
}

template<class Label, class T>
int DataSet<Label, T>::get_label(vector<T> * vtr)
{
    return (*_labels)[vtr];
}

template<class Label, class T>
vector<size_t> DataSet<Label, T>::get_domain()
{
    return _domain;
}

template<class Label, class T>
vector<T> * DataSet<Label, T>::operator[](size_t index)
{
    return (*_vectors)[_domain[index]];
}

template<class Label, class T>
DataSet<Label, T> DataSet<Label, T>::subset(vector<size_t> domain)
{
    return DataSet<Label, T>(*this, domain);
}
