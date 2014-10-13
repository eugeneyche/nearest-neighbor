#ifndef PART_H_
#define PART_H_

#include <utility>

template <class Label, class T>
class Part;

template <class Label, class T>
class Part
{
public:
    virtual pair<DataSet<Label, T>*, DataSet<Label, T>*> 
            operator<< (const DataSet<Label, T>*) = 0;
};
#endif /* PART_H_ */
