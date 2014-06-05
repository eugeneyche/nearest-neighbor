/* 
 * File     : data_set.h
 * Date     : 2014-5-29
 * Summary  : Infrastructure to hold the data set.
 */
#ifndef DATA_SET_H_
#define DATA_SET_H_

#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <Eigen/Core>
#include <Eigen/Eigenvalues>
#include "vector_math.h"
#include "logging.h"
using namespace std;

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
    Label get_label(size_t index) const;
    Label get_label(vector<T> * vtr) const;
    vector<size_t> get_domain() const;
    vector<T> * operator[](size_t index) const;
    DataSet<Label, T> subset(vector<size_t> domain);
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
    size_t idx [dimension];
    for (size_t i = 0; i < dimension; i++)
    {
        idx[i] = i;
    }
    /* Selection Sort for Parallel Arrays */
    vector<size_t> result;
    for (size_t i = 0; i < k; i++)
    {
        size_t maxIndex = i;
        for (size_t j = i; j < dimension; j++)
        {
            if (var[j] > var[maxIndex])
            {
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
        mean.push_back(sum[i] / subset.size());
    }
    
    /* initialize eigen here */
    vector<double> eigen;
    double eigen_size = 0.0;
    srand((int)time(NULL));

    /* WARNING: Potentially dangerous if subset has size 0 */
    for (size_t i = 0; i < (*subset[0]).size(); i++)
    {
        double x = (double)((rand() % 201) - 100) / 100;
        eigen.push_back(x);
        eigen_size += x * x;
    }
    eigen_size = sqrt(eigen_size);
    
    /* normalize eigen here */
    for (size_t i = 0; i < eigen.size(); i++)
    {
        eigen[i] = eigen[i] / eigen_size;
    }
    
    /* calculate dominant eigenvector*/
    for (size_t i = 0; i < subset.size(); i++)
    {
        double gamma = 1/(i + 1); // learning rate

        /* calculate X transpose dot V*/
        double X_transpo_dot_V = 0.0;
        vector<double> X;
        for (size_t j = 0; j < X.size(); j++)
        {
            X.push_back((double)(*subset[i])[j] - mean[j]); //centralize X
            X_transpo_dot_V += (X[j] * eigen[j]);
        }

        /* normalize eigenvector */
        double vector_size = 0.0;
        for (size_t j = 0; j < X.size(); j++)
        {
            eigen[j] += (gamma * X[j] * X_transpo_dot_V);
            vector_size += eigen[j] * eigen[j];
        }
        vector_size = sqrt(vector_size);
        for (size_t j = 0; j < X.size(); j++)
        {
            eigen[j] = eigen[j]/vector_size;
        }
    }
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
vector<double> max_eigen_vector(DataSet<Label, T> & subset)
{
    LOG_INFO("Creating eigenvector of subset size %ld\n", subset.size());
    /*
    int rows = subset[0]->size();
    int cols = subset.size();
    */
    int dim = (*subset[0]).size();  /* Dimension of each vector */
    int num = subset.size();        /* Number of vectors */
    Eigen::MatrixXd mtx = Eigen::MatrixXd::Zero(num, dim);
    for (size_t i = 0; i < num; i++)
    {
        for (size_t j = 0; j < dim; j++)
        {
            mtx(i, j) = (double)(*subset[i])[j];
        }
    }
    LOG_FINE("Subset of dimensions %d, %d copied over\n", num, dim);
    Eigen::MatrixXd centered = mtx.rowwise() - mtx.colwise().mean();
    LOG_FINE("Done centering...\n");
    Eigen::MatrixXd covar    = (centered.adjoint() * centered) / (double)num;
    LOG_FINE("Done covariance...\n");
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eig(covar);
    Eigen::VectorXd eigVtr = eig.eigenvectors().rightCols(1);
    LOG_FINE("Done eigenvectors...\n");
    cout << eigVtr << endl;
    vector<double> maxEigVtr;
    double len;
    for (size_t i = 0; i < dim; i++)
    {
        len += eigVtr(i) * eigVtr(i);
    }
    LOG_FINE("Eigen Vector length %lf\n", len);
    for (size_t i = 0; i < dim; i++)
    {
        maxEigVtr.push_back(eigVtr(i) / len);
    }
    return maxEigVtr;
}

/* Private Functions */

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

/* Public Functions */

/*
 * Name             : DataSet
 * Prototype        : DataSet<Label, T>::DataSet()
 * Description      : The default constructor.
 * Parameter(s)     : None
 * Return Value     : Creates an empty data set
 */
template<class Label, class T>
DataSet<Label, T>::DataSet() :
  parent_ (NULL),
  labels_ (new label_space),
  vectors_ (new vector_space)
{ }

/*
 * Name             : DataSet
 * Prototype        : DataSet<Label, T>::DataSet(ifstream &)
 * Description      : De-serialization constructor.
 * Parameter(s)     : None
 * Return Value     : Creates a de-serialized data set
 */
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

/*
 * Name             : ~DataSet
 * Prototype        : DataSet<Label, T>::~DataSet()
 * Description      : The deconstructor.
 * Parameter(s)     : None
 * Return Value     : None
 */
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

/*
 * Name             : size
 * Prototype        : size_t DataSet<Label, T>::size() const
 * Description      : Returns the size of the data set.
 * Parameter(s)     : None
 * Return Value     : The size of the data set
 */
template<class Label, class T>
size_t DataSet<Label, T>::size() const
{
    return domain_.size();
}

/*
 * Name             : label
 * Prototype        : void DataSet<Label, T>::label(ifstream & in)
 * Description      : Labels data set through label file
 * Parameter(s)     : in    - The file to get the label data from
 * Return Value     : None
 */
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

/*
 * Name             : get_label
 * Prototype        : Label DataSet<Label, T>::get_label(size_t) const
 * Description      : Gets the label of a given vector.
 * Parameter(s)     : index - The index of vector to get label from
 * Return Value     : Label value of the given vector
 */
template<class Label, class T>
Label DataSet<Label, T>::get_label(size_t index) const
{
    return get_label((*this)[index]);
}

/*
 * Name             : get_label
 * Prototype        : Label DataSet<Label, T>::get_label(vector<T> *) const
 * Description      : Gets the label of a given vector.
 * Parameter(s)     : vtr - The vector to get label from
 * Return Value     : Label value of the given vector
 */
template<class Label, class T>
Label DataSet<Label, T>::get_label(vector<T> * vtr) const
{
    return (*labels_)[vtr];
}

/*
 * Name             : get_domain() const
 * Prototype        : vector<size_t> DataSet<Label, T>::get_domain() const
 * Description      : Gets the domain of the data set.
 * Parameter(s)     : None
 * Return Value     : A vector containing indices in vector space
 */
template<class Label, class T>
vector<size_t> DataSet<Label, T>::get_domain() const
{
    return domain_;
}

/*
 * Name             : operator []
 * Prototype        : vector<T> * operator[](size_t)
 * Description      : Gets the vector at given index
 * Parameter(s)     : index - The index of the vector
 * Return Value     : The vector pointer of the wanted vector
 */
template<class Label, class T>
vector<T> * DataSet<Label, T>::operator[](size_t index) const
{
    return (*vectors_)[domain_[index]];
}

/*
 * Name             : subset
 * Prototype        : DataSet<Label, T> DataSet<Label, T>::subset(vector<size_t>)
 * Description      : Creates a subset of the data set.
 * Parameter(s)     : domain - The subdomain of given data set
 * Return Value     : The sub set of the given subdomain
 */
template<class Label, class T>
DataSet<Label, T> DataSet<Label, T>::subset(vector<size_t> domain)
{
    return DataSet<Label, T>(*this, domain);
}

#endif
