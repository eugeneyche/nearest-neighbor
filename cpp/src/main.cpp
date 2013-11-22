#include "data_set.h"
#include "nn.h"
#include <fstream>
#include <algorithm>
using namespace std;

data_set train;
data_set test;

char gradient [] = {' ', '.',':',';','+','=','x','X','$','@'};

void printGlyph(euclid_vector * to_print)
{
    for (int i = 0; i < 28 * 28; i++)
    {
        printf("%c", gradient[(int)((*to_print)[i] / 255 * 10)]);
        if (i % 28 == 27) printf("\n");
    }
}

int main()
{
    FILE * train_vtrs = fopen("data/mnist/train_vectors", "r");
    FILE * train_labels = fopen("data/mnist/train_labels", "r");
    FILE * test_vtrs = fopen("data/mnist/test_vectors", "r");
    FILE * test_labels = fopen("data/mnist/test_labels", "r");
    load(train, train_vtrs);
    label(train, train_labels);
    printf("Loaded Train Set\n");
    load(test, test_vtrs);
    label(test, test_labels);
    printf("Loaded Test Set\n");
    int count [100] = {0};
    for (int i = 0; i < 10; i++)
    {
        data_set mn_nn = k_nn(test[i], train, 100);
        for (int j = 0; j < mn_nn.size(); j++)
        {
            if (mn_nn.get_label(j) == test.get_label(i))
            {
                count[j]++;
            }
        }
        printGlyph(test[i]);
    }
    for (int i = 0; i < 100; i++)
    {
        if (i > 0) printf(" ");
        printf("%d", count[i]);
    }
    printf("\n");
    fclose(train_vtrs);
    fclose(train_labels);
    fclose(test_vtrs);
    fclose(test_labels);
}
