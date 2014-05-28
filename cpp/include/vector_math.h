#ifndef VECTOR_MATH_H_
#define VECTOR_MATH_H_

#include <vector>
#include <random>

using namespace std;

template<class T>
double distance_to(const vector<T> * v1, const vector<T> * v2)
{
    if (v1->size() != v2->size())
        return -1;
    double distance = 0;
    for (size_t i = 0; i < v1->size(); i++)
    {
        double d = (double)(*v2)[i] - (double)(*v1)[i];
        distance += d * d;
    }
    return distance;
}

template<class T>
double dot(vector<T> v, vector<double> vd)
{
    long double factor = 0;;
    for (int i = 0; i < v.size() && i < vd.size(); i++)
        factor += (double)v[i] * vd[i];
    return factor;
}

template<class T>
T selector(vector<T> st, size_t k)
{
	srand(int(time(NULL)));
	size_t sz = st.size();
	double randomIndex = rand() % sz;
    
	vector<T> left;
	vector<T> right;
	vector<T> v;
    
    typename vector<T>::iterator itr;
	for (itr = st.begin(); itr != st.end(); itr++)
	{
		if(*itr == st[randomIndex])
			v.push_back(*itr);
		else if(*itr < st[randomIndex])
			left.push_back(*itr);
		else
			right.push_back(*itr);
	}
    
	if (left.size() >= k)
		return selector(left, k);
	else if (left.size() + v.size() >= k)
		return st[randomIndex];
	else
		return selector(right, (size_t)(k - left.size() - v.size()));
}

template<class T>
vector<double> eigen_vector(vector<T> & data_set)
{
    /*calculate mean*/
    vector<double> mean;
    vector<double> sum = data_set[0];
    for (size_t i = 0; i < data_set.size(); i++)
    {
        vector<double> v = data_set[i];
        for (size_t j = 0; j < v.size(); j++)
        {
            sum[j] += data_set[i][j];
        }
    }
    size_t size = data_set.size();
    for (size_t i = 0; i < data_set.size(); i++)
    {
        mean.push_back(sum[i]/size);
    }
    
    /* initialize eigen here */
    vector<double> eigen;
    double eigen_size = 0.0;
    srand(int(time(NULL)));
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
    for (size_t i = 0; i < data_set.size(); i++)
    {
        double gamma = 1/i;
        /*calculate X transpose dot V*/
        double X_transpo_dot_V = 0.0;
        vector<double> X = *data_set[i];
        for (size_t j = 0; j < X.size(); j++)
        {
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

#endif
