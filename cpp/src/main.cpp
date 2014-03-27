#include "vector_math.h"
#include "nn.h"
#include "data_set.h"
#include <iostream>
using namespace std;

typedef unsigned char byte;

char gradient [] = {' ', '.',':',';','+','=','x','X','$','@'};

void print_glyph(vector<byte> * to_print)
{
    for (int i = 0; i < 28 * 28; i++) {
        printf("%c", gradient[(int)((double)(*to_print)[i] / 255 * 10)]);
        if (i % 28 == 27) printf("\n");
    }
}

int main() 
{
    ifstream test_vtr_in ("data/mnist/test_vectors", ios::binary);
    ifstream test_lbl_in ("data/mnist/test_labels", ios::binary);
    ifstream train_vtr_in ("data/mnist/train_vectors", ios::binary);
    ifstream train_lbl_in ("data/mnist/train_labels", ios::binary);
    DataSet<byte, byte> testSet (test_vtr_in);
    DataSet<byte, byte> trainSet (train_vtr_in);

    testSet.label(test_lbl_in);
    trainSet.label(train_lbl_in);
    print_glyph(testSet[0]);
    nearest_neighbor(trainSet[0], testSet);
    print_glyph(testSet[0]);
    test_vtr_in.close();
    test_lbl_in.close();
    train_vtr_in.close();
    train_lbl_in.close();
}
