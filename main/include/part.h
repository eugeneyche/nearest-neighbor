#ifndef PART_H
#define PART_H

#include <utility>
using namespace std;

#include "math.h"

template <class Label, class T>
class Part
{
public:
    virtual pair<DataSet<Label, T> *, DataSet<Label, T> *> 
            operator() (const DataSet<Label, T> *) = 0;
};

template<class Label, class T>
size_t 
max_variance_index(DataSet<Label, T> * data)
{
    vector<double> var;
    vector<T> vtr;
    size_t dimension = (*data)[0]->size();
    size_t subsize = (*data).size();
    for (size_t i = 0; i < dimension; i++) {
        vtr.clear();
        for (size_t j = 0; j < subsize; j++) {
            vtr.push_back((*(*data)[j])[i]);
        }
        T median = selector(vtr, (size_t)((*data).size() * 0.5));
        double variance = 0.0;
        for (size_t j = 0; j < subsize; j++) {
            double dif = (double)(*(*data)[j])[i] - (double)median;
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
    return maxIndex;
}

template<class Label, class T>
vector<size_t> 
k_max_variance_indices(size_t k, DataSet<Label, T> * data)
{
    vector<double> var;
    vector<T> vtr;
    size_t dimension = (*data)[0]->size();
    size_t subsize = (*data).size();
    for (size_t i = 0; i < dimension; i++) {
        vtr.clear();
        for (size_t j = 0; j < subsize; j++) {
            vtr.push_back((*(*data)[j])[i]);
        }
        T median = selector(vtr, (size_t)((*data).size() * 0.5));
        double variance = 0.0;
        for (size_t j = 0; j < subsize; j++) {
            double dif = (double)(*(*data)[j])[i] - (double)median;
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

template<class Label, class T>
vector<double> 
max_eigen_vector_oja(DataSet<Label, T> * data)
{
    /* Calculate mean */
    vector<double> mean;
    vector<double> sum;

    for (size_t i = 0; i < (*data)[0]->size(); i++) {
        sum.push_back(0);
        for (size_t j = 0; j < (*data).size();j++) {
            sum[i] += (*(*data)[j])[i];
        }
    }
    for (size_t i = 0; i < (*data).size(); i++) {
        mean.push_back(sum[i] / (*data).size());
    }
    
    /* initialize eigen here */
    vector<double> eigen;
    double eigen_size = 0.0;
    srand((int)time(NULL));

    /* WARNING: Potentially dangerous if subset has size 0 */
    for (size_t i = 0; i < (*(*data)[0]).size(); i++) {
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
    for (size_t i = 0; i < (*data).size(); i++) {
        double gamma = 1/(i + 1); // learning rate

        /* calculate X transpose dot V*/
        double X_transpo_dot_V = 0.0;
        vector<double> X;
        for (size_t j = 0; j < X.size(); j++) {
            X.push_back((double)(*(*data)[i])[j] - mean[j]); //centralize X
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

template<class Label, class T>
vector<double> 
max_eigen_vector(DataSet<Label, T> * data)
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


template <class Label, class T>
class KDPart : public Part<Label, T>
{
public:
    pair<DataSet<Label, T> *, DataSet<Label, T> *> 
            operator() (const DataSet<Label, T> * data)
    {
        size_t pivot_idx =  max_variance_index(data);
        vector<T> pivot_data;
        for (size_t it = 0; it < data.size(); it++) {
            data.push_back((*(*data)[it])[pivot_idx]);
        }
        T pivot = selector(pivot_data, pivot_data.size() / 2);
        DataSet<Label, T>::DomainSpace left_d;
        DataSet<Label, T>::DomainSpace right_d;
        DataSet<Label, T>::DomainSpace fence_d;
        for (size_t it = 0; it < data.size(); it++) {
            if ((*(*data)[it])[pivot_idx] < pivot) {
                left_d.push_back(it);
            }
            if ((*(*data)[it])[pivot_idx] > pivot) {
                right_d.push_back(it);
            }
            if ((*(*data)[it])[pivot_idx] == pivot) {
                fence_d.push_back(it);
            }
        }
        for (size_t it = 0; it < fence.size(); it++) {
            if (left_d.size() < pivot_data.size() / 2) {
                left_d.push_back(fence_d[it]);
            } else {
                right_d.push_back(fence_d[it]);
            }
        }
        return make_pair(data->subset(left_domain), data->subset(right_domain));
    }
};

template <class Label, class T>
class KDSpillPart : public Part<Label, T>
{
private:
    double alpha;
public:
    pair<DataSet<Label, T> *, DataSet<Label, T> *> 
            operator() (const DataSet<Label, T> * data)
    {
        size_t pivot_idx =  max_variance_index(data);
        vector<T> pivot_data;
        for (size_t it = 0; it < data.size(); it++) {
            data.push_back((*(*data)[it])[pivot_idx]);
        }
        T pivot = selector(pivot_data, pivot_data.size() / 2);
        DataSet<Label, T>::DomainSpace left_d;
        DataSet<Label, T>::DomainSpace right_d;
        DataSet<Label, T>::DomainSpace fence_d;
        for (size_t it = 0; it < data.size(); it++) {
            if ((*(*data)[it])[pivot_idx] < pivot) {
                left_d.push_back(it);
            }
            if ((*(*data)[it])[pivot_idx] > pivot) {
                right_d.push_back(it);
            }
            if ((*(*data)[it])[pivot_idx] == pivot) {
                fence_d.push_back(it);
            }
        }
        for (size_t it = 0; it < fence.size(); it++) {
            if (left_d.size() < pivot_data.size() / 2) {
                left_d.push_back(fence_d[it]);
            } else {
                right_d.push_back(fence_d[it]);
            }
        }
        return make_pair(data->subset(left_domain), data->subset(right_domain));
    }
};

#endif /* PART_H */
