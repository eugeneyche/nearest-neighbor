#include <cstdio>
using namespace std;

#include "tree.h"
#include "dataset.h"

typedef unsigned char byte;

int 
main(void) 
{
    DataSet<int, int> meh;
    Part<int, int> * partp;
    Tree<int, int> * treep = new Tree<int, int>(meh, *partp);
}
