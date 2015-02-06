/* 
 * File     : part.h
 * Date     : 20-1-2015
 * Summary  : Contains the data structure definition
 *            of a tree structure
 */
#ifndef PART_H
#define PART_H

#include <utility>
using namespace std;

#include "math.h"

#define MIN_SIZE (5)

enum Action {LEFT, RIGHT, SPILL};

template<class T>
size_t 
max_variance_index(const DataSet<T> &data)
{
    vector<double> var;
    vector<T> vtr;
    size_t dimension = data[0].size();
    size_t subsize = data.size();
    for (size_t i = 0; i < dimension; i++) {
        vtr.clear();
        for (size_t j = 0; j < subsize; j++) {
            vtr.push_back(data[j][i]);
        }
        T median = selector(vtr, (size_t)(data.size() * 0.5));
        double variance = 0.0;
        for (size_t j = 0; j < subsize; j++) {
            double diff = (double)data[j][i] - median;
            variance += diff * diff;
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

template<class T>
vector<size_t> 
k_max_variance_indices(const DataSet<T> &data, size_t k)
{
    vector<double> var;
    vector<T> vtr;
    size_t dimension = data[0]->size();
    size_t subsize = data.size();
    for (size_t i = 0; i < dimension; i++) {
        vtr.clear();
        for (size_t j = 0; j < subsize; j++) {
            vtr.push_back(data[j][i]);
        }
        T median = selector(vtr, (size_t)((*data).size() * 0.5));
        double variance = 0.0;
        for (size_t j = 0; j < subsize; j++) {
            double diff = (double)data[j][i] - (double)median;
            variance += diff * diff;
        }
        variance = variance / subsize;
        var.push_back(variance);
    }
    size_t idx [dimension];
    for (size_t i = 0; i < dimension; i++) {
        idx[i] = i;
    }

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

template<class T>
vector<double> 
max_eigen_vector_oja(const DataSet<T> &data)
{
    /* Calculate mean */
    vector<double> mean;
    vector<double> sum;

    for (size_t i = 0; i < data[0]->size(); i++) {
        sum.push_back(0);
        for (size_t j = 0; j < data.size();j++) {
            sum[i] += data[j][i];
        }
    }
    for (size_t i = 0; i < data.size(); i++) {
        mean.push_back(sum[i] / data.size());
    }
    
    /* initialize eigen here */
    vector<double> eigen;
    double eigen_size = 0.0;
    srand((int)time(NULL));

    /* WARNING: Potentially dangerous if subset has size 0 */
    for (size_t i = 0; i < data[0].size(); i++) {
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
            X.push_back((double)data[i][j] - mean[j]);
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
    return eigen;
}

template<class T>
vector<double> 
max_eigen_vector(const DataSet<T> &data)
{
    if (data->size() == 0)
        return vector<double>();
    int dim = (*data)[0]->size();
    int num = data->size();
    Eigen::MatrixXd mtx = Eigen::MatrixXd::Zero(num, dim);
    for (size_t i = 0; i < num; i++) {
        for (size_t j = 0; j < dim; j++) {
            mtx(i, j) = (double)data[i][j];
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

template<class T>
class KD
{
protected:
    typename DataSet<T>::DomainSpace 
            left, right;
    T pivot;
    size_t pivot_index;

public:
    KD(const DataSet<T> &data)
    {
        if (data.size() < MIN_SIZE)
            return;
        pivot_index = max_variance_index(data);
        vector<T> pivots;
        for (size_t it = 0; it < data.size(); it++) {
            pivots.push_back(data[it][pivot_index]);
        }
        size_t half = size_t(pivots.size() * 0.5);
        pivot = selector(pivots, half);
        typename DataSet<T>::DomainSpace middle;
        for (size_t it = 0; it < data.size(); it++) {
            if (data[it][pivot_index] < pivot)
                left.push_back(it);
            else if (data[it][pivot_index] > pivot)
                right.push_back(it);
            else if (data[it][pivot_index] == pivot)
                middle.push_back(it);
        }
        for (size_t it = 0; it < middle.size(); it++) {
            if (left.size() < half) {
                left.push_back(middle[it]);
            } else {
                right.push_back(middle[it]);
            }
        }
    }

    KD(const DataSet<T> &data, FILE *in)
    {
        size_t l_size, r_size, buffer_size;
        fread(&l_size, sizeof(size_t), 1, in);
        fread(&r_size, sizeof(size_t), 1, in);

        buffer_size = l_size > r_size ? l_size : r_size;
        size_t buffer [buffer_size];

        fread(buffer, sizeof(size_t), l_size, in);
        left.assign(buffer, buffer + l_size);

        fread(buffer, sizeof(size_t), r_size, in);
        right.assign(buffer, buffer + r_size);

        fread(&pivot, sizeof(T), 1, in);
        fread(&pivot_index, sizeof(size_t), 1, in);
    }

    virtual const 
    typename DataSet<T>::DomainSpace get_left(void)
    {
        return left;
    }

    virtual const 
    typename DataSet<T>::DomainSpace get_right(void)
    {
        return right;
    }

    virtual Action 
    query(const typename DataSet<T>::Vector & vtr)
    {
        return (vtr[pivot_index] < pivot)
            ? LEFT
            : RIGHT;
    }

    virtual 
    void save(FILE *out)
    {
        size_t l_size, r_size;
        l_size = left.size();
        r_size = right.size();

        fwrite(&l_size, sizeof(size_t), 1, out);
        fwrite(&r_size, sizeof(size_t), 1, out);

        fwrite(&left[0], sizeof(size_t), l_size, out);
        fwrite(&right[0], sizeof(size_t), r_size, out);

        fwrite(&pivot, sizeof(T), 1, out);
        fwrite(&pivot_index, sizeof(size_t), 1, out);
    }
};

double default_spill = 0;

template<class T, const double & spill = default_spill>
class Spill
{
protected:
    typename DataSet<T>::DomainSpace 
            left, right;
    T pivot;
    size_t pivot_index, pivot_l, pivot_r;

public:
    Spill(const DataSet<T> &data)
    { 
        if (data.size() < MIN_SIZE)
            return;
        pivot_index = max_variance_index(data);
        vector<T> pivots;
        for (size_t it = 0; it < data.size(); it++) {
            pivots.push_back(data[it][pivot_index]);
        }
        size_t half = size_t(pivots.size() * 0.5);
        size_t spill_l = size_t(pivots.size() * (0.5 - spill));
        size_t spill_r = size_t(pivots.size() * (0.5 + spill));
        pivot = selector(pivots, half);
        pivot_l = selector(pivots, spill_l);
        pivot_r = selector(pivots, spill_r);
        typename DataSet<T>::DomainSpace fence;
        typename DataSet<T>::DomainSpace fence_l;
        typename DataSet<T>::DomainSpace fence_r;
        for (size_t it = 0; it < data.size(); it++) {
            if (data[it][pivot_index] < pivot_l)
                left.push_back(it);
            else if (data[it][pivot_index] > pivot_r)
                right.push_back(it);
            else if (data[it][pivot_index] == pivot_l)
                fence_l.push_back(it);
            else if (data[it][pivot_index] == pivot_r)
                fence_r.push_back(it);
            else if (data[it][pivot_index] > pivot_l &&
                    data[it][pivot_index] < pivot_r) {
                fence.push_back(it);
            }
        }
        for (size_t it = 0; it < fence_l.size(); it++) {
            if (left.size() < spill_l) {
                left.push_back(fence_l[it]);
            } else {
                fence.push_back(fence_l[it]);
            }
        }
        for (size_t it = 0; it < fence_r.size(); it++) {
            if (right.size() < spill_r) {
                right.push_back(fence_r[it]);
            } else {
                fence.push_back(fence_r[it]);
            }
        }
        for (size_t it = 0; it < fence.size(); it++) {
            left.push_back(fence[it]);
            right.push_back(fence[it]);
        }
    }

    Spill(const DataSet<T> &data, FILE *in)
    { 
        size_t l_size, r_size, buffer_size;
        fread(&l_size, sizeof(size_t), 1, in);
        fread(&r_size, sizeof(size_t), 1, in);

        buffer_size = l_size > r_size ? l_size : r_size;
        size_t buffer [buffer_size];

        fread(buffer, sizeof(size_t), l_size, in);
        left.assign(buffer, buffer + l_size);

        fread(buffer, sizeof(size_t), r_size, in);
        right.assign(buffer, buffer + r_size);

        fread(&pivot, sizeof(T), 1, in);
        fread(&pivot_index, sizeof(size_t), 1, in);
        fread(&pivot_l, sizeof(size_t), 1, in);                       
        fread(&pivot_r, sizeof(size_t), 1, in);                       
    }

    virtual const 
    typename DataSet<T>::DomainSpace get_left(void)
    {
        return left;
    }

    virtual const 
    typename DataSet<T>::DomainSpace get_right(void)
    {
        return right;
    }

    virtual Action 
    query(const typename DataSet<T>::Vector & vtr)
    {
        return (vtr[pivot_index] < pivot)
            ? LEFT
            : RIGHT;
    }

    virtual 
    void save(FILE *out)
    {
        size_t l_size, r_size;
        l_size = left.size();
        r_size = right.size();

        fwrite(&l_size, sizeof(size_t), 1, out);
        fwrite(&r_size, sizeof(size_t), 1, out);

        fwrite(&left[0], sizeof(size_t), l_size, out);
        fwrite(&right[0], sizeof(size_t), r_size, out);

        fwrite(&pivot, sizeof(T), 1, out);
        fwrite(&pivot_index, sizeof(size_t), 1, out);
        fwrite(&pivot_l, sizeof(size_t), 1, out);
        fwrite(&pivot_r, sizeof(size_t), 1, out);
    }
};

template<class T, const double & spill = default_spill>
class VSpill : public KD<T>
{
protected:
    typename DataSet<T>::DomainSpace 
            left, right;
    T pivot;
    size_t pivot_index, pivot_l, pivot_r;

public:
    VSpill(const DataSet<T> &data)
    { 
        if (data.size() < MIN_SIZE)
            return;
        pivot_index = max_variance_index(data);
        vector<T> pivots;
        for (size_t it = 0; it < data.size(); it++) {
            pivots.push_back(data[it][pivot_index]);
        }
        size_t half = size_t(pivots.size() * 0.5);
        size_t spill_l = size_t(pivots.size() * (0.5 - spill));
        size_t spill_r = size_t(pivots.size() * (0.5 + spill));
        pivot = selector(pivots, half);
        pivot_l = selector(pivots, spill_l);
        pivot_r = selector(pivots, spill_r);
        typename DataSet<T>::DomainSpace middle;
        for (size_t it = 0; it < data.size(); it++) {
            if (data[it][pivot_index] < pivot)
                left.push_back(it);
            else if (data[it][pivot_index] > pivot)
                right.push_back(it);
            else if (data[it][pivot_index] == pivot)
                middle.push_back(it);
        }
        for (size_t it = 0; it < middle.size(); it++) {
            if (left.size() < half) {
                left.push_back(middle[it]);
            } else {
                right.push_back(middle[it]);
            }
        }
    }

    VSpill(const DataSet<T> &data, FILE *in)
    { 
        size_t l_size, r_size, buffer_size;
        fread(&l_size, sizeof(size_t), 1, in);
        fread(&r_size, sizeof(size_t), 1, in);

        buffer_size = l_size > r_size ? l_size : r_size;
        size_t buffer [buffer_size];

        fread(buffer, sizeof(size_t), l_size, in);
        left.assign(buffer, buffer + l_size);

        fread(buffer, sizeof(size_t), r_size, in);
        right.assign(buffer, buffer + r_size);

        fread(&pivot, sizeof(T), 1, in);
        fread(&pivot_index, sizeof(size_t), 1, in);
        fread(&pivot_l, sizeof(size_t), 1, in);                       
        fread(&pivot_r, sizeof(size_t), 1, in);                       
    }

    virtual const 
    typename DataSet<T>::DomainSpace get_left(void)
    {
        return left;
    }

    virtual const 
    typename DataSet<T>::DomainSpace get_right(void)
    {
        return right;
    }

    virtual Action 
    query(const typename DataSet<T>::Vector & vtr)
    {
        if (vtr[pivot_index] < pivot_l)
            return LEFT;
        if (vtr[pivot_index] > pivot_r)
            return RIGHT;
        return SPILL;
    }

    virtual 
    void save(FILE *out)
    {
        size_t l_size, r_size;
        l_size = left.size();
        r_size = right.size();

        fwrite(&l_size, sizeof(size_t), 1, out);
        fwrite(&r_size, sizeof(size_t), 1, out);

        fwrite(&left[0], sizeof(size_t), l_size, out);
        fwrite(&right[0], sizeof(size_t), r_size, out);

        fwrite(&pivot, sizeof(T), 1, out);
        fwrite(&pivot_index, sizeof(size_t), 1, out);
        fwrite(&pivot_l, sizeof(size_t), 1, out);
        fwrite(&pivot_r, sizeof(size_t), 1, out);
    }
};

#endif /* PART_H */
