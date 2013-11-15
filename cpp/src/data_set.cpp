#include "data_set.h"

void load(data_set & st, ifstream & vtr_in)
{
    int n, m;
    vtr_in >> n >> m;
    double v;
    for (int i = 0; i < n; i++)
    {
        euclid_vector * vtr = new euclid_vector;
        for (int j = 0; j < m; j++)
        {
            vtr_in >> v;
            vtr->push_back(v);
        }
        st._domain.push_back(st._domain.size());
        st._vectors->push_back(vtr);
    }
}

void label(data_set & st, ifstream & label_in)
{
    int n;
    label_in >> n;
    int l;
    for (int i = 0; i < n; i++)
    {
        label_in >> l;
        st.set_label(st[i], l);
    }
}

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
        _domain.push_back(_domain.size());
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
    }
}

int data_set::size()
{
    return _domain.size();
}

void data_set::set_label(euclid_vector * vtr, int label)
{
    (*_labels)[vtr] = label;
}

int data_set::get_label(euclid_vector * vtr)
{
    return (*_labels)[vtr];
}

euclid_vector * data_set::operator[](int i)
{
    return (*_vectors)[_domain[i]];
}

data_set data_set::subset(vector <int> domain)
{
    return data_set(*this, domain);
}
