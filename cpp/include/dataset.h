/* 
 * File     : data_set.h
 * Date     : 2014-5-29
 * Summary  : Infrastructure to hold the data set.
 */
#ifndef DATASET_H_
#define DATASET_H_

#include <stdio.h>

#include <map>
#include <algorithm>
#include <Eigen/Core>
#include <Eigen/Eigenvalues>

#include "vector_math.h"
#include "logging.h"

/* 
 * Name             : DataSet
 * Description      : Data structure to hold the data set.
 * Data Field(s)    : parent_   - parent data set that this is spawned form
 *                    labels_   - map of labels for each of the vectors
 *                    vectors_  - all vectors in data set
 *                    domain_   - vectors out of vector space in data set
 * Functions(s)     : DataSet() - Default constructor
 *                    DataSet(ifstream & in)
 *                              - De-serialization constructor
 *                    ~DataSet() 
 *                              - Deconstructor 
 */
template<class Label, class T>
class DataSet
{
public:
    typedef std::vector<T>* Vector;
    typedef std::map<vector<T>*, Label> LabelSpace;
    typedef std::vector<Vector> VectorSpace;
    typedef std::vector<size_t> DomainSpace;

private:
    DataSet<Label, T>*  parent;
    LabelSpace*         labels;
    VectorSpace*        vectors;
    DomainSpace         domain;
    DataSet(const DataSet<Label, T>*, DomainSpace);

    void loadVectors(FILE* in);
    void labelVectors(FILE* in);

public:
    DataSet(void);
    DataSet(FILE*);
    DataSet(FILE*, FILE*);
    ~DataSet(void);
    const size_t        size(void)                      const;
    const Label         getLabel(const size_t)          const;
    const Label         getLabel(const Vector)          const;
    const DomainSpace   getDomain(void)                 const;
    const Vector        operator[](const size_t)        const;
    DataSet<Label, T>*  subset(const DomainSpace);
};

/*
 * Name             : max_variance_index
 * Prototype        : size_t max_variance_index(DataSet<Label, T> &)
 * Description      : Gets the index that produces the greatest variance
 *                    out of all the vectors
 * Parameter(s)     : subset - The data set to find the max varianced
 *                             index
 * Return Value     : The index that produces the max variance
 */
template<class Label, class T>
size_t max_variance_index(DataSet<Label, T> & subset)
{
    LOG_INFO("Enter max_variance_index\n");
    LOG_FINE("with subset.size = %ld\n", subset.size());
    vector<double> var;
    vector<T> vtr;
    size_t dimension = subset[0]->size();
    size_t subsize = subset.size();
    for (size_t i = 0; i < dimension; i++) {
        vtr.clear();
        for (size_t j = 0; j < subsize; j++) {
            vtr.push_back((*subset[j])[i]);
        }
        T median = selector(vtr, (size_t)(subset.size() * 0.5));
        double variance = 0.0;
        for (size_t j = 0; j < subsize; j++) {
            double dif = (double)(*subset[j])[i] - (double)median;
            variance += dif * dif;
        }
        variance = variance / subsize;
        var.push_back(variance);
    }
    size_t maxIndex = 0;
    for (size_t i = 1; i < dimension; i++) {
        if (var[i] > var[maxIndex]) {
            maxIndex = i;
        }
    }
    LOG_INFO("Exit max_variance_index\n");
    LOG_FINE("with result = %ld \n ", maxIndex);
    return maxIndex;
}

/*
 * Name             : max_variance_indices
 * Prototype        : vector<size_t> k_max_variance_index(DataSet<Label, T> &)
 * Description      : Gets the index that produces the greatest variance
 *                    out of all the vectors
 * Parameter(s)     : subset - The data set to find the max varianced
 *                             index
 * Return Value     : The index that produces the max variance
 */
template<class Label, class T>
vector<size_t> k_max_variance_indices(size_t k, DataSet<Label, T> & subset)
{
    vector<double> var;
    vector<T> vtr;
    size_t dimension = subset[0]->size();
    size_t subsize = subset.size();
    for (size_t i = 0; i < dimension; i++) {
        vtr.clear();
        for (size_t j = 0; j < subsize; j++) {
            vtr.push_back((*subset[j])[i]);
        }
        T median = selector(vtr, (size_t)(subset.size() * 0.5));
        double variance = 0.0;
        for (size_t j = 0; j < subsize; j++) {
            double dif = (double)(*subset[j])[i] - (double)median;
            variance += dif * dif;
        }
        variance = variance / subsize;
        var.push_back(variance);
    }
    size_t idx [dimension];
    for (size_t i = 0; i < dimension; i++) {
        idx[i] = i;
    }
    /* Selection Sort for Parallel Arrays */
    vector<size_t> result;
    for (size_t i = 0; i < k; i++) {
        size_t maxIndex = i;
        for (size_t j = i; j < dimension; j++) {
            if (var[j] > var[maxIndex]) {
                maxIndex = j;
            }
        }
        result.push_back(idx[maxIndex]);
        swap(var[i], var[maxIndex]);
        swap(idx[i], idx[maxIndex]);
    }
    return result;
}

/*
 * Name             : max_eigen_vector_oja
 * Prototype        : vector<double> max_eigen_vector_oja(DataSet<Label, T> &)
 * Description      : Gets the eigen vector of the data set using Oja's
 *                    algorithm..
 * Parameter(s)     : subset - The data set to find the eigen vector of
 * Return Value     : The (max) eigen vector of the data set
 * Notes            : Still needs quite a bit of work.
 */
template<class Label, class T>
vector<double> max_eigen_vector_oja(DataSet<Label, T> & subset)
{
    /* Calculate mean */
    vector<double> mean;
    vector<double> sum;

    for (size_t i = 0; i < subset[0]->size(); i++) {
        sum.push_back(0);
        for (size_t j = 0; j < subset.size();j++) {
            sum[i] += (*subset[j])[i];
        }
    }
    for (size_t i = 0; i < subset.size(); i++) {
        mean.push_back(sum[i] / subset.size());
    }
    
    /* initialize eigen here */
    vector<double> eigen;
    double eigen_size = 0.0;
    srand((int)time(NULL));

    /* WARNING: Potentially dangerous if subset has size 0 */
    for (size_t i = 0; i < (*subset[0]).size(); i++) {
        double x = (double)((rand() % 201) - 100) / 100;
        eigen.push_back(x);
        eigen_size += x * x;
    }
    eigen_size = sqrt(eigen_size);
    
    /* normalize eigen here */
    for (size_t i = 0; i < eigen.size(); i++) {
        eigen[i] = eigen[i] / eigen_size;
    }
    
    /* calculate dominant eigenvector*/
    for (size_t i = 0; i < subset.size(); i++) {
        double gamma = 1/(i + 1); // learning rate

        /* calculate X transpose dot V*/
        double X_transpo_dot_V = 0.0;
        vector<double> X;
        for (size_t j = 0; j < X.size(); j++) {
            X.push_back((double)(*subset[i])[j] - mean[j]); //centralize X
            X_transpo_dot_V += (X[j] * eigen[j]);
        }

        /* normalize eigenvector */
        double vector_size = 0.0;
        for (size_t j = 0; j < X.size(); j++) {
            eigen[j] += (gamma * X[j] * X_transpo_dot_V);
            vector_size += eigen[j] * eigen[j];
        }
        vector_size = sqrt(vector_size);
        for (size_t j = 0; j < X.size(); j++) {
            eigen[j] = eigen[j]/vector_size;
        }
    }
    LOG_INFO("Exit max_eigen_vector_oja\n");
    return eigen;
}

/*
 * Name             : max_eigen_vector
 * Prototype        : vector<double> max_eigen_vector(DataSet<Label, T> &)
 * Description      : Gets the eigen vector of the data set using Eigen's
 *                    algorithm.
 * Parameter(s)     : subset - The data set to find the eigen vector of
 * Return Value     : The (max) eigen vector of the data set
 */
template<class Label, class T>
vector<double> MaxEigenVector(DataSet<Label, T>* data)
{
    if (!data->size())
        return vector<double>;
    int dim = (*data)[0]->size();
    int num = data->size();
    Eigen::MatrixXd mtx = Eigen::MatrixXd::Zero(num, dim);
    for (size_t i = 0; i < num; i++) {
        for (size_t j = 0; j < dim; j++) {
            mtx(i, j) = (double)(*(*subset)[i])[j];
        }
    }
    Eigen::MatrixXd centered = mtx.rowwise() - mtx.colwise().mean();
    Eigen::MatrixXd covar    = (centered.adjoint() * centered) / (double)num;
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eig(covar);
    Eigen::VectorXd eigVtr   = eig.eigenvectors().rightCols(1);
    vector<double> maxEigVtr;
    double len;
    for (size_t i = 0; i < dim; i++) {
        len += eigVtr(i) * eigVtr(i);
    }
    for (size_t i = 0; i < dim; i++) {
        maxEigVtr.push_back(eigVtr(i) / len);
    }
    return maxEigVtr;
}

template<class Label, class T>
DataSet<Label, T>::DataSet(const DataSet<Label, T>* parent, DomainSpace domain) :
        parent (parent),
        labels (parent->labels),
        vectors (parent->vectors)
{
    DomainSpace::iterator itr;
    for (itr = domain.begin(); itr != domain.end(); itr++) {
        domain.push_back(parent->domain[*itr]);
    }
}

template <class Label, class T>
void 
DataSet<Label, T>::loadVectors(FILE* in)
{
    size_t n, m;
    fread(&n, sizeof(size_t), 1, in);
    fread(&m, sizeof(size_t), 1, in);
    for (size_t i = 0; i < n; i++)
    {
        Vector vtr = new Vector;
        T buffer [m];
        fread(buffer, sizeof(T), m, in);
        vtr->assign(buffer, buffer + m);
        domain.push_back(domain.size());
        vectors->push_back(vtr);
    }
}

template <class Label, class T>
void 
DataSet<Label, T>::labelVectors(FILE* in)
{
    size_t n;
    fread(&n, sizeof(size_t), 1, in);
    Label buffer [n];
    fread(buffer, sizeof(Label), n, in);
    for (size_t i = 0; i < n; i++)
    {
        (*labels)[(*this)[i]] = buffer[i];
    }
}

template<class Label, class T>
DataSet<Label, T>::DataSet(void) :
        parent (nullptr),
        labels (new LabelSpace),
        vectors (new VectorSpace)
{ }

template<class Label, class T>
DataSet<Label, T>::DataSet(FILE* vectorIn) :
        parent (nullptr),
        labels (new LabelSpace),
        vectors (new VectorSpace)
{
    loadVectors(vectorIn);
}

template<class Label, class T>
DataSet<Label, T>::DataSet(FILE* vectorIn, FILE* labelIn) :
        parent (nullptr),
        labels (new LabelSpace),
        vectors (new VectorSpace)
{
    loadVectors(vectorIn);
    labelVectors(labelIn);
}

template <class Label, class T>
DataSet<Label, T>::~DataSet(void)
{
    if (!parent)
    {
        while (!vectors->empty())
        {
            delete vectors->back();
            vectors->pop_back();
        }
        delete labels;
        delete vectors;
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
    return getLabel((*this)[index]);
}

template <class Label, class T>
const Label 
DataSet<Label, T>::getLabel(const Vector vtr) const
{
    return (*labels)[vtr];
}

template <class Label, class T>
const typename DataSet<Label,T>::DomainSpace 
DataSet<Label, T>::getDomain(void) const
{
    return domain;
}

template <class Label, class T>
const typename DataSet<Label, T>::Vector 
DataSet<Label, T>::operator[](const size_t index) const
{
    return (*vectors)[domain[index]];
}

template <class Label, class T>
DataSet<Label, T>* 
DataSet<Label, T>::subset(const DomainSpace domain)
{
    return new DataSet<Label, T>(this, domain);
}

#endif /* DATASET_H_ */
