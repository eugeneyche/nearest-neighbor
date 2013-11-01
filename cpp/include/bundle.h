#ifndef _BUNDLE_H
#define _BUNDLE_H

#include <vector>
using namespace std;

class Bundle
{
private:
    int label;
    vector <double> vtr;
public:
    Bundle(int label, vector <double> vtr);
    double distance_to(const Bundle & other) const;
    const int & get_label() const;
    const vector <double> & get_vtr() const;
};

#endif
