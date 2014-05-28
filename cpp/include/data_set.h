#ifndef DATA_SET_H_
#define DATA_SET_H_

#include <iostream>
#include <fstream>
#include <map>
#include "vector_math.h"
using namespace std;

template<class Label, class T>
class DataSet
{
    typedef map<vector<T> *, Label> label_space;
    typedef vector<vector<T> *> vector_space;
private:
    DataSet<Label, T> * parent_;
    label_space * labels_;
    vector_space * vectors_;
    vector<size_t> domain_;
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
vector<double> max_eigen_vector(DataSet<Label, T> & subset)
{
    /*calculate mean*/
    vector<double> mean;
    vector<double> sum;

    for (size_t i = 0; i < subset[0]->size(); i++)
    {
        sum.push_back(0);
        for (size_t j = 0; j < subset.size();j++)
        {
            sum[i] += (*subset[j])[i];
        }
    }
    for (size_t i = 0; i < subset.size(); i++)
    {
        mean.push_back(sum[i]/subset.size());
    }
    
    /* initialize eigen here */
    vector<double> eigen;
    double eigen_size = 0.0;
    srand((int)time(NULL));
    for (size_t i = 0; i < 10; i++)
    {
        double x = (double)((rand() % 201) - 100) / 100;
        eigen.push_back(x);
        eigen_size += x*x;
    }
    eigen_size = sqrt(eigen_size);
    
    /* normalize eigen here */
    for (size_t i = 0; i < eigen.size(); i++)
    {
        eigen[i] = eigen[i]/eigen_size;
    }
    
    /*calculate dominant eigenvector*/
    for (size_t i = 0; i < subset.size(); i++)
    {
        double gamma = 1/(i + 1);
        /*calculate X transpose dot V*/
        double X_transpo_dot_V = 0.0;
        vector<double> X;
        for (size_t j = 0; j < X.size(); j++)
        {
            X.push_back((double)(*subset[i])[j]);
            X[j] = X[j] - mean[j]; //centralize X
            X_transpo_dot_V += (X[j] * eigen[j]);
        }
        /*update dominant eigenvector*/
        double vector_size = 0.0;
        for (size_t k = 0; k < X.size(); k++)
        {
            eigen[k] += (gamma * X[k] * X_transpo_dot_V);
            vector_size += eigen[k]*eigen[k];
        }
        vector_size = sqrt(vector_size);
        /*normalize eigenvector*/
        for (size_t k = 0; k < X.size(); k++)
        {
            eigen[k] = eigen[k]/vector_size;
        }
    }
    return eigen;
}

template<class Label, class T>
DataSet<Label, T>::DataSet(DataSet<Label, T> & parent, vector<size_t> domain) :
  parent_ (&parent),
  labels_ (parent.labels_),
  vectors_ (parent.vectors_)
{
    vector<size_t>::iterator itr;
    for (itr = domain.begin(); itr != domain.end(); itr++)
    {
        domain_.push_back(parent.domain_[*itr]);
    }
}

template<class Label, class T>
DataSet<Label, T>::DataSet() :
  parent_ (NULL),
  labels_ (new label_space),
  vectors_ (new vector_space)
{ }

template<class Label, class T>
DataSet<Label, T>::DataSet(ifstream & in) :
  parent_ (NULL),
  labels_ (new label_space),
  vectors_ (new vector_space)
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
        domain_.push_back(domain_.size());
        vectors_->push_back(vtr);
    }
}

template<class Label, class T>
DataSet<Label, T>::DataSet(vector_space vectors) :
  parent_ (NULL),
  labels_ (new label_space),
  vectors_ (new vector_space)
{
    typename vector_space::iterator itr;
    for (itr = vectors.begin(); itr != vectors.end(); itr++)
    {
        domain_.push_back((size_t)domain_.size());
        vectors_->push_back(*itr);
    }
}

template<class Label, class T>
DataSet<Label, T>::~DataSet()
{
    if (parent_ == NULL)
    {
        while (vectors_->size() > 0)
        {
            delete vectors_->back();
            vectors_->pop_back();
        }
        delete labels_;
        delete vectors_;
    }
}

template<class Label, class T>
size_t DataSet<Label, T>::size() const
{
    return domain_.size();
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
        (*labels_)[(*this)[i]] = buffer[i];
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
    return (*labels_)[vtr];
}

template<class Label, class T>
vector<size_t> DataSet<Label, T>::get_domain() const
{
    return domain_;
}

template<class Label, class T>
vector<T> * DataSet<Label, T>::operator[](size_t index) const
{
    return (*vectors_)[domain_[index]];
}

template<class Label, class T>
DataSet<Label, T> DataSet<Label, T>::subset(vector<size_t> domain)
{
    return DataSet<Label, T>(*this, domain);
}
#endif
