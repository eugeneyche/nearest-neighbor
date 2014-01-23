#include "nn.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <ctime>
using namespace std;

int DATASIZE = 60000;
data_set train;
data_set test;

string janet_dir = "/Users/janetzhai/Desktop/nn-xcode/nn-xcode/";
string eugene_dir = "data/mnist/";

char gradient [] = {' ', '.',':',';','+','=','x','X','$','@'};

void printGlyph(euclid_vector * to_print)
{
    for (int i = 0; i < 28 * 28; i++) {
        printf("%c", gradient[(int)((*to_print)[i] / 255 * 10)]);
        if (i % 28 == 27) printf("\n");
    }
}

void generate_kd_tree(string input_file_name, data_set train)
{
    clock_t start, end;
    int c = 0.05 * DATASIZE;
    int train_size = train.size();
    start = clock();
    kd_tree_node * root = kd_tree(c, train);
    end = clock();
    double time = double(end - start) / CLOCKS_PER_SEC;
    cout << time << endl;
    FILE * kd_tree_out = fopen((janet_dir+input_file_name).c_str(), "wb");
    save_tree(root, kd_tree_out);
    fclose(kd_tree_out);
}


int kd_tree_count(int c, int count_correct, int size, data_set train, data_set test, kd_tree_node * root)
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


int spill_query_correctness(data_set * train, data_set test, kd_tree_node * root, int a)
{
    query_tree_node * spill_root = build_query_tree(* train, root, root->get_domain(), a);
    int count_correct = 0;
    for (int i = 0; i < test.size(); i++)
    {
        euclid_vector * spill_nn = spill_query_nn(test[i], train, spill_root);
        if (test.get_label(test[i]) == train->get_label(spill_nn))
            count_correct++;
    }
    return count_correct;
}


void c_appr_nn(int size, int c_size, double c[], double fraction_avg[])
{
    for (int k = 0; k < c_size; k++)
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
}


int main() {
    string path = eugene_dir;
    FILE * train_vtrs = fopen((path + "train_vectors").c_str(), "rb");
    FILE * train_labels = fopen((path + "train_labels").c_str(), "rb");
    FILE * test_vtrs = fopen((path + "test_vectors").c_str(), "rb");
    FILE * test_labels = fopen((path + "test_labels").c_str(), "rb");

    load(train, train_vtrs);
    label(train, train_labels);
    load(test, test_vtrs);
    label(test, test_labels);

    FILE * input = fopen("tree", "rb");
    kd_tree_node * tree = load_tree(input);
    print_tree(tree, 0);
    delete tree;
    fclose(input);

    fclose(train_vtrs);
    fclose(train_labels);
    fclose(test_vtrs);
    fclose(test_labels);
}
