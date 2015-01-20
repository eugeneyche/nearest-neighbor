/* 
 * File     : dataset.h
 * Date     : 14-1-2015
 * Summary  : Infrastructure to hold the data set.
 */
#ifndef DATASET_H
#define DATASET_H

#include <cstdio>
#include <map>
#include <algorithm>
#include <Eigen/Core>
#include <Eigen/Eigenvalues>

using namespace std;

#include "vmath.h"
#include "logging.h"

template<class Label, class T>
class DataSet
{
public:
    typedef vector<T> Vector;
    typedef typename Vector::const_iterator iterator;
    typedef map<Vector *, Label> LabelSpace;
    typedef vector<Vector> VectorSpace;
    typedef vector<size_t> DomainSpace;

private:
    DataSet<Label, T> * parent;
    LabelSpace        * labels;
    VectorSpace       * vectors;
    DomainSpace         domain;

    DataSet(const DataSet<Label, T> *, DomainSpace);

    void loadVectors(FILE *);
    void loadLabels(FILE *);

public:
    DataSet(void);
    DataSet(const DataSet<Label, T> &);
    DataSet(FILE *);
    DataSet(FILE *, FILE *);
    ~DataSet(void);
    const size_t        size(void)               const;
    const Label         getLabel(const size_t)   const;
    const Label         getLabel(const Vector &) const;
    const DomainSpace   getDomain(void)          const;
    iterator            begin(void)              const;
    iterator            end(void)                const;
    const Vector &      operator[](const size_t) const;
    DataSet<Label, T> subset(const DomainSpace);
};

template<class Label, class T>
DataSet<Label, T>::DataSet(const DataSet<Label, T> * parent, DomainSpace domain) 
    : parent  (parent)
    , labels  (parent->labels)
    , vectors (parent->vectors)
{
    DomainSpace::iterator itr;
    for (itr = domain.begin(); itr != domain.end(); itr++) {
        domain.push_back(parent->domain[*itr]);
    }
}

template<class Label, class T>
DataSet<Label, T>::DataSet(void) 
    : parent  (NULL)
    , labels  (NULL)
    , vectors (NULL)
{ }

template<class Label, class T>
DataSet<Label, T>::DataSet(const DataSet<Label, T> & copy)
    : parent  (copy.parent)
    , labels  (copy.labels)
    , vectors (copy.vectors)
    , domain  (copy.domain)
{ }

template<class Label, class T>
DataSet<Label, T>::DataSet(FILE* vectorIn) 
    : parent (NULL)
    , labels  (NULL)
{
    loadVectors(vectorIn);
}

template<class Label, class T>
DataSet<Label, T>::DataSet(FILE* vectorIn, FILE* labelIn) 
    : parent (NULL)
{
    loadVectors(vectorIn);
    loadLabels(labelIn);
}


template <class Label, class T>
void 
DataSet<Label, T>::loadVectors(FILE* in)
{
    parent = NULL;
    labels = NULL;
    vectors = new VectorSpace;
    size_t n, m;
    fread(&n, sizeof(size_t), 1, in);
    fread(&m, sizeof(size_t), 1, in);
    for (size_t i = 0; i < n; i++) {
        Vector vtr;
        T buffer [m];
        fread(buffer, sizeof(T), m, in);
        vtr.assign(buffer, buffer + m);
        domain.push_back(domain.size());
        vectors->push_back(vtr);
    }
}

template <class Label, class T>
void 
DataSet<Label, T>::loadLabels(FILE * in)
{
    if (!vectors)
        return;
    labels = new LabelSpace;
    size_t n;
    fread(&n, sizeof(size_t), 1, in);
    Label buffer [n];
    fread(buffer, sizeof(Label), n, in);
    for (size_t i = 0; i < n; i++) {
        (*labels)[&(*this)[i]] = buffer[i];
    }
}

template <class Label, class T>
DataSet<Label, T>::~DataSet(void)
{
    if (!parent) {
        if (labels) delete labels;
        if (vectors) delete vectors;
    }
}

template <class Label, class T>
const size_t 
DataSet<Label, T>::size(void) const
{
    return domain.size();
}

template <class Label, class T>
const Label 
DataSet<Label, T>::getLabel(const size_t index) const
{
    return getLabel(&(*this)[index]);
}

template <class Label, class T>
const Label 
DataSet<Label, T>::getLabel(const Vector & vtr) const
{
    return (*labels)[&vtr];
}

template <class Label, class T>
const typename DataSet<Label,T>::DomainSpace 
DataSet<Label, T>::getDomain(void) const
{
    return domain;
}

template <class Label, class T>
typename DataSet<Label, T>::iterator
DataSet<Label, T>::begin(void) const
{
    return vectors->cbegin();
}

template <class Label, class T>
typename DataSet<Label, T>::iterator
DataSet<Label, T>::end(void) const
{
    return vectors->cend();
}

template <class Label, class T>
const typename DataSet<Label, T>::Vector &
DataSet<Label, T>::operator[](const size_t index) const
{
    return (*vectors)[domain[index]];
}

template <class Label, class T>
DataSet<Label, T>
DataSet<Label, T>::subset(const DomainSpace domain)
{
    return DataSet<Label, T>(this, domain);
}

#endif /* DATASET_H */
