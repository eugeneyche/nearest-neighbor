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

template<class T>
class DataSet
{
public:
    typedef vector<T> Vector;
    typedef vector<Vector> VectorSpace;
    typedef vector<size_t> DomainSpace;

private:
    const DataSet<T>  *parent;
    const VectorSpace *vectors;
    const DomainSpace *domain;

    DataSet(const DataSet<T> *, const DomainSpace &, bool);

    void loadVectors(FILE *);

public:

    class iterator 
    {
        friend class DataSet<T>;
    private: 
        typename DataSet<T>::DomainSpace::const_iterator index;
        const VectorSpace *vectors;

        iterator(typename DataSet<T>::DomainSpace::const_iterator index,
                const typename DataSet<T>::VectorSpace *vectors)
            : index   (index)
            , vectors (vectors)
        { }

    public:
        iterator(const iterator &copy)
            : index   (copy.index)
            , vectors (copy.vectors)
        { }

        const typename DataSet<T>::Vector & 
        operator*(void) 
        {
            return vectors->at(*index);
        }

        const typename DataSet<T>::Vector * 
        operator->(void) 
        {
            return &(operator*()); 
        }
        
        iterator & 
        operator++(void) 
        {
            ++index;
            return *this;
        }

        iterator
        operator++(int) 
        {
            return iterator(index++, vectors);
        }

        bool 
        operator!=(const iterator &other) 
        {
            return vectors != other.vectors || index != other.index;
        }

        bool 
        operator==(const iterator & other) 
        {
            return !operator!=(other);
        }
        
        size_t 
        get_index(void)
        {
            return *index;
        }
    };

    DataSet(void);
    DataSet(const DataSet<T> &);
    DataSet(FILE *);
    ~DataSet(void);
    const size_t       size(void)                  const;
    const VectorSpace &get_vectors(void)           const;
    const DomainSpace &get_domain(void)            const;
    iterator           begin(void)                 const;
    iterator           end(void)                   const;
    const Vector &     operator[](const size_t)    const;
    DataSet<T>         subset(const DomainSpace &) const;
    DataSet<T>         merge(const DataSet<T> &)   const;
};

template<class T>
DataSet<T>::DataSet(
        const DataSet<T> *parent, 
        const DomainSpace &domain, 
        bool relative)
    : parent  (parent)
    , vectors (parent->vectors)
{
    DomainSpace *build_domain = new DomainSpace;
    if (relative) {
        for (auto it = domain.begin(); it != domain.end(); it++) {
            build_domain->push_back(parent->domain->at(*it));
        }
    } else {
        build_domain->insert(build_domain->begin(), domain.begin(), domain.end());
    }
    this->domain = build_domain;
}

template<class T>
DataSet<T>::DataSet(void) 
    : parent  (nullptr)
    , vectors (nullptr)
    , domain  (nullptr)
{ }

template<class T>
DataSet<T>::DataSet(const DataSet<T> &copy)
    : parent  (copy.parent)
    , vectors (copy.vectors)
    , domain  (new DomainSpace(*copy.domain))
{ }

template<class T>
DataSet<T>::DataSet(FILE *in) 
    : parent (nullptr)
{
    parent = nullptr;
    VectorSpace *build_vectors = new VectorSpace;
    DomainSpace *build_domain = new DomainSpace;
    size_t n, m;
    fread(&n, sizeof(size_t), 1, in);
    fread(&m, sizeof(size_t), 1, in);
    for (size_t i = 0; i < n; i++) {
        Vector vtr;
        T buffer [m];
        fread(buffer, sizeof(T), m, in);
        vtr.assign(buffer, buffer + m);
        build_domain->push_back(build_domain->size());
        build_vectors->push_back(vtr);
    }
    vectors = build_vectors;
    domain = build_domain;
}

template <class T>
DataSet<T>::~DataSet(void)
{
    if (!parent && vectors) {
        delete vectors;
    }
    if (domain) delete domain;
}

template <class T>
const size_t 
DataSet<T>::size(void) const
{
    return domain->size();
}

template <class T>
const typename DataSet<T>::VectorSpace &
DataSet<T>::get_vectors(void) const
{
    return *vectors;
}

template <class T>
const typename DataSet<T>::DomainSpace &
DataSet<T>::get_domain(void) const
{
    return *domain;
}

template <class T>
typename DataSet<T>::iterator
DataSet<T>::begin(void) const
{
    return DataSet<T>::iterator(domain->begin(), vectors);
}

template <class T>
typename DataSet<T>::iterator
DataSet<T>::end(void) const
{
    return DataSet<T>::iterator(domain->end(), vectors);
}

template <class T>
const typename DataSet<T>::Vector &
DataSet<T>::operator[](const size_t index) const
{
    return (*vectors)[domain->at(index)];
}

template <class T>
DataSet<T>
DataSet<T>::subset(const DomainSpace &domain) const
{
    return DataSet<T>(this, domain, true);
}


template <class T>
DataSet<T>
DataSet<T>::merge(const DataSet<T> &other) const
{
    DomainSpace merged_domain;
    size_t it_this, it_other;
    it_this = it_other = 0;
    while (it_this < size() && it_other < other.size()) {
        if ((*this)[it_this] < other[it_other]) {
            merged_domain.push_back((*this)[it_this]);
            it_this++;
        }
        if ((*this)[it_this] == other[it_other]) {
            merged_domain.push_back((*this)[it_this]);
            it_this++;
            it_other++;
        }
        if ((*this)[it_this] > other[it_other]) {
            merged_domain.push_back(other[it_this]);
            it_other++;
        }
    }
    while (it_this < size()) {
        merged_domain.push_back((*this)[it_this]);
        it_this++;
    }
    while (it_other < other.size()) {
        merged_domain.push_back(other[it_other]);
        it_other++;
    }
    return DataSet<T>(this, merged_domain, false);
}

#endif /* DATASET_H */
