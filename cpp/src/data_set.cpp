#include "data_set.h"

data_set::data_set(data_set & parent, vector <int> domain)
{
    _parent = &parent;
    for (vector <int>::iterator itr = domain.begin(); itr != domain.end(); itr++)
    {
        _vectors.push_back(parent[*itr]);
    }
}

data_set::data_set(vector_space vectors)
{
    for (vector_space::iterator itr = vectors.begin(); itr != vectors.end(); itr++)
    {
        _vectors.push_back(*itr);
    }
}

data_set::~data_set()
{
    if (_parent == NULL)
    {
        while (_vectors.size() > 0)
        {
            delete _vectors.back();
            _vectors.pop_back();
        }
    }
}

int data_set::size()
{
    return _vectors.size();
}

label_space & data_set::labels()
{
    return _labels;
}

vector_space::iterator data_set::begin()
{
    return _vectors.begin();
}

vector_space::iterator data_set::end()
{
    return _vectors.end();
}

euclid_vector * data_set::operator[](int i)
{
    return _vectors[i];
}

vector <int> data_set::domain()
{
    vector <int> domain;
    for (int i = 0; i < _vectors.size(); i++)
    {
        domain.push_back(i);
    }
    return domain;
}

data_set data_set::subset(vector <int> domain)
{
    return data_set(*this, domain);
}
