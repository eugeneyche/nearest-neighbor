#include "data_set.h"
#include "nn.h"
#include "kd_tree_node.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int DATASIZE = 60000;
data_set train;
data_set test;

string janet_dir = "/Users/janetzhai/Desktop/KNN/KNN/";
string eugene_dir = "data/mnist/";

char gradient [] = {' ', '.',':',';','+','=','x','X','$','@'};

void printGlyph(euclid_vector * to_print)
{
    for (int i = 0; i < 28 * 28; i++) {
        printf("%c", gradient[(int)((*to_print)[i] / 255 * 10)]);
        if (i % 28 == 27) printf("\n");
    }
}

int kd_tree_count(int c, int count_correct, int size, data_set train, kd_tree_node * root)
{
    for (int i = 0; i < size; i++)
    {
        cout << test.get_label(test[i]) << " -> ";
        cout.flush();
        euclid_vector * l_mn = kd_tree_nn(test[i], train, c, root);
        cout << train.get_label(l_mn);
        if (test.get_label(test[i]) != train.get_label(l_mn))
            cout << " X" << endl;
        else
        {
            cout << endl;
            count_correct++;
        }
    }
    return count_correct;
}


int main() {
    string path = janet_dir;
    FILE * train_vtrs = fopen((path + "train_vectors").c_str(), "rb");
    FILE * train_labels = fopen((path + "train_labels").c_str(), "rb");
    FILE * test_vtrs = fopen((path + "test_vectors").c_str(), "rb");
    FILE * test_labels = fopen((path + "test_labels").c_str(), "rb");
    load(train, train_vtrs);
    label(train, train_labels);
    load(test, test_vtrs);
    label(test, test_labels);


    //kd-tree
    FILE * kd_tree_out = fopen("tree", "wb");

    //changed c and train_size just to test the load and store
    int c = 5;//0.05 * DATASIZE;
    int train_size = 30;//train.size();
    kd_tree_node * root = kd_tree(c, train, train_size);
    print_tree(root, 0);
    save_tree(root, kd_tree_out);
    fclose(kd_tree_out);
    
    FILE * input = fopen("tree", "rb");
    kd_tree_node * root2 = load_tree(input);//root2 should be the same as root
    cout << " DONE " << endl;
    print_tree(root2, 0);
    delete root;
    
    /*
    int count_correct = 0;
    int size = test.size();
    count_correct = kd_tree_count(c, count_correct, size, train, root);
    float rate = (float)count_correct / size;
    cout << " There are " << rate << "% correct labels" << endl;
    */

    /*
    //c-approximate NN
    int size = test.size();
    double c[6] = {1.0,1.2,1.4,1.6,1.8,2.0};
    double fraction_avg[6] = {0.0};
    for (int k = 0; k < 6; k++)
    {
        for (int i = 0; i < size; i++)
        {
            int fraction = 0;
            euclid_vector * i_nn = nn(test[i], train);
            data_set c_nn = c_approx_nn(test[i], train, i_nn, c[k]);
            for (int j = 0; j < c_nn.size(); j++)
            {
                if (c_nn.get_label(j) == test.get_label(i))
                {
                    fraction++;
                }
            }
            fraction = fraction / c_nn.size();
            fraction_avg[k] += fraction;
        }
        fraction_avg[k] = fraction_avg[k] / size;
        cout<<c[k]<<"   "<<fraction_avg[k]<<endl;
    }
     */


    fclose(input);
    fclose(train_vtrs);
    fclose(train_labels);
    fclose(test_vtrs);
    fclose(test_labels);
}
