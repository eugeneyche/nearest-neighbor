#include "nn.h"
#include "test.h"
#include "prettyprint.h"
#include <fstream>
#include <cstdio>
#include <string>
#include <stack>
#include <algorithm>
#include <ctime>
#include <cmath>
using namespace std;

data_set train;
data_set test;

char janet_dir [] = "/Users/janetzhai/Desktop/nn-xcode/nn-xcode/";
char eugene_dir [] = "data/mnist/";

int main() {
    test_setup(eugene_dir);

    FILE * input = fopen("tree", "rb");
    kd_tree_node * root = load_kd_tree(input);
    fclose(input);
    virtual_spill_tree_node * qroot = virtual_spill_tree(0.01, root, get_train_set());

    print_tree(root);
    for (int i = 0; i < 10; i++)
    {
        euclid_vector * mnn = nn(get_test_set()[i], get_train_set());
        euclid_vector * kdnn = kd_tree_nn(get_test_set()[i], get_train_set(), root);
        euclid_vector * qnn = virtual_spill_tree_nn(get_test_set()[i], get_train_set(), qroot);
        printf("%ld %ld %ld\n", mnn, kdnn, qnn);
    }
    delete qroot;
    delete root;
}
