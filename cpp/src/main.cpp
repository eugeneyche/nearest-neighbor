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
    ifstream train_vtr_in ("data/mnist/test_vectors", ios::binary);
    DataSet<byte, byte> trainSet (test_vtr_in);
    for (int i = 0; i < trainSet.size(); i++)
    {
        print_glyph(trainSet[i]);
        cout << ">>>" << endl;
        print_glyph(nn(trainSet[i]));
        cout << "---" << endl;
    }
    test_vtr_in.close();
}
