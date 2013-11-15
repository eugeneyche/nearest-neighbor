#include "data_set.h"
#include "nn.h"
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

data_set train;
data_set test;

int main()
{
    ifstream train_vtrs_in ("data/mnist/train_vectors");
    ifstream train_labels_in ("data/mnist/train_labels");
    ifstream test_vtrs_in ("data/mnist/test_vectors");
    ifstream test_labels_in ("data/mnist/test_labels");
    cout << "Loading train set...";
    cout.flush();
    load(train, train_vtrs_in);
    label(train, train_labels_in);
    cout << "Done." << endl;
    cout << "Loading test set...";
    cout.flush();
    load(test, test_vtrs_in);
    label(test, test_labels_in);
    cout << "Done." << endl;

    for (int i = 0; i < test.size(); i++)
    {
        cout << test.get_label(test[i]) << " -> ";
        cout.flush();
        euclid_vector * l_mn = nn(test[i], train);
        cout << train.get_label(l_mn);
        if (test.get_label(test[i]) != train.get_label(l_mn))
            cout << " X" << endl;
        else
            cout << endl;
        
    }
    train_vtrs_in.close();
    train_labels_in.close();
    test_vtrs_in.close();
    test_labels_in.close();
}
