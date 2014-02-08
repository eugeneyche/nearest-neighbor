#include "data_set.h"

void load_data_set(data_set & st, FILE * in)
{
    int n, m;
    fscanf(in, "%d %d\n", &n, &m);
    for (int i = 0; i < n; i++)
    {
        euclid_vector * vtr = new euclid_vector;
        double buffer [m];
        fread(buffer, sizeof(double), m, in);
        vtr->assign(buffer, buffer + m);
        st._domain.push_back((int)st._domain.size());
        st._vectors->push_back(vtr);
    }
}

void label_data_set(data_set & st, FILE * in)
{
    int n;
    fscanf(in, "%d\n", &n);
    int buffer [n];
    fread(buffer, sizeof(int), n, in);
    for (int i = 0; i < n; i++)
    {
        st.set_label(i, buffer[i]);
    }
}

int max_variance_index(int k, data_set & sub)
{
    vector <double> var;
    vector <double> vtr;
    int dimension = (int)sub[0]->size();
    int subsize = sub.size();
    for (int i = 0; i < dimension; i++)
    {
        vtr.clear();
        for (int j = 0; j < subsize; j++)
        {
            vtr.push_back((*sub[j])[i]);
        }
        double mean = selector(vtr, k);
        double variance = 0.0;
        for (int j = 0; j < subsize; j++)
        {
            double dif = (*sub[j])[i]-mean;
            variance += dif * dif;
        }
        variance = variance / subsize;
        var.push_back(variance);
    }
    int maxIndex = 0;
    for (int i = 1; i < dimension; i++)
    {
        if (var[i] > var[maxIndex])
            maxIndex = i;
    }
    return maxIndex;
}

/* Class Definition */

data_set::data_set(data_set & parent, vector <int> domain)
{
    _parent = &parent;
    _labels = parent._labels;
    _vectors = parent._vectors;
    for (vector <int>::iterator itr = domain.begin(); itr != domain.end(); itr++)
    {
        _domain.push_back(parent._domain[*itr]);
    }
}

data_set::data_set()
{
    _parent = NULL;
    _labels = new label_space;
    _vectors = new vector_space;
}


data_set::data_set(vector_space vectors)
{
    _parent = NULL;
    _labels = new label_space;
    _vectors = new vector_space;
    for (vector_space::iterator itr = vectors.begin(); itr != vectors.end(); itr++)
    {
        _domain.push_back((int)_domain.size());
        _vectors->push_back(*itr);
    }
}

data_set::~data_set()
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
        cerr << "[DEBUG: Deconstructing data_set]" << endl;
        #endif
    }
}

int data_set::size()
{
    return (int)_domain.size();
}

void data_set::set_label(int i, int label)
{
    set_label((*this)[i], label);
}

void data_set::set_label(euclid_vector * vtr, int label)
{
    (*_labels)[vtr] = label;
}

int data_set::get_label(int i)
{
    return get_label((*this)[i]);
}

int data_set::get_label(euclid_vector * vtr)
{
    return (*_labels)[vtr];
}

vector <int> data_set::get_domain()
{
    return _domain;
}

vector <int> data_set::get_abs_domain()
{
    vector <int> domain;
    for (int i = 0; i < _domain.size(); i++)
    {
        domain.push_back(i);
    }
    return domain;
}

euclid_vector * data_set::operator[](int i)
{
    return (*_vectors)[_domain[i]];
}

data_set data_set::subset(vector <int> domain)
{
    return data_set(*this, domain);
}
