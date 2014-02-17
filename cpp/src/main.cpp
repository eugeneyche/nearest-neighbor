#include "nn.h"
#include "test.h"
#include "prettyprint.h"
#include <cstdio>
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
#include <ctime>
#include <cmath>
using namespace std;

char janet_dir [] = "/Users/janetzhai/Desktop/nn-xcode/nn-xcode/";
char eugene_dir [] = "data/mnist/";

int main() {
    test_setup(eugene_dir);

    double alpha_values [] = {0, 0.0025, 0.005, 0.01, 0.015, 0.02, 0.03, 0.05, 0.1, 0.2};

    vector <euclid_vector *> true_nn;
    for (int i = 0; i < get_test_set().size(); i++)
    {
        true_nn.push_back(nn(get_test_set()[i], get_train_set()));
    }

    char buffer [999];
    for (int i = 0; i < 10; i++)
    {
        buffer[0] = '\0';
        sprintf(buffer, "data/mnist/trees/mnist_spill_tree_%g", alpha_values[i]);
        FILE * input = fopen(buffer, "rb");
        kd_tree_node * root = load_kd_tree(input);
        print_tree(root);
        int count = 0;;
        for (int j = 0; j < get_test_set().size(); j++)
        {
            euclid_vector * kdnn = kd_tree_nn(get_test_set()[j], get_train_set(), root);
            if (true_nn[j] == kdnn)
            {
                count++;
            }
        }
        printf("%lg %lg\n", alpha_values[i], (double)count / get_test_set().size());
        delete root;
        fclose(input);
    }
}
