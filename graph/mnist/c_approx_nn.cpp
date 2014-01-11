#include "data_set.h"
#include "nn.h"
#include "kd_tree_node.h"
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;

int DATASIZE = 60000;
data_set train;
data_set test;

char gradient [] = {' ', '.',':',';','+','=','x','X','$','@'};

void printGlyph(euclid_vector * to_print)
{
    for (int i = 0; i < 28 * 28; i++) {
        printf("%c", gradient[(int)((*to_print)[i] / 255 * 10)]);
        if (i % 28 == 27) printf("\n");
    }
}


int main() {
    FILE * train_vtrs = fopen("/Users/janetzhai/Desktop/KNN/KNN/train_vectors", "rb");
    FILE * train_labels = fopen("/Users/janetzhai/Desktop/KNN/KNN/train_labels", "rb");
    FILE * test_vtrs = fopen("/Users/janetzhai/Desktop/KNN/KNN/test_vectors", "rb");
    FILE * test_labels = fopen("/Users/janetzhai/Desktop/KNN/KNN/test_labels", "rb");
    load(train, train_vtrs);
    label(train, train_labels);
    printf("Loaded Train Set\n");
    load(test, test_vtrs);
    label(test, test_labels);
    printf("Loaded Test Set\n");
    
    //c-approximate NN
    int size = test.size();
    int fraction[] = {0};
    double c[6] = {1.0,1.2,1.4,1.6,1.8,2.0};
    double fraction_avg[6] = {0.0};
    for (int k = 0; k < 6; k++)
    {
        for (int i = 0; i < size; i++)
        {
            euclid_vector * i_nn = nn(test[i], train);
            data_set c_nn = c_approx_nn(test[i], train, i_nn, c[k]);
            for (int j = 0; j < c_nn.size(); j++)
            {
                if (c_nn.get_label(j) == test.get_label(i))
                {
                    fraction[i]++;
                }
            }
            fraction[i] = fraction[i] / c_nn.size();
            fraction_avg[k] += fraction[i];
        }
        fraction_avg[k] = fraction_avg[k] / size;
        cout<<c[k]<<"   "<<fraction_avg[k]<<endl;
    }
    fclose(train_vtrs);
    fclose(train_labels);
    fclose(test_vtrs);
    fclose(test_labels);
    fclose(train_vtrs);
    fclose(train_labels);
    fclose(test_vtrs);
    fclose(test_labels);
}
