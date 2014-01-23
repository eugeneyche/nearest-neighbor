#include "nn.h"
#include "prettyprint.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
using namespace std;

data_set train;
data_set test;

string janet_dir = "/Users/janetzhai/Desktop/nn-xcode/nn-xcode/";
string eugene_dir = "data/mnist/";

void tc_knn_error();
void tc_knn_purity();
void tc_c_approx_nn();
void tc_save_kd_tree(const char * file_path);
void tc_load_kd_tree(const char * file_path);
void tc_kd_tree_k_nn_error(const char * file_path);

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

    tc_kd_tree_k_nn_error("tree");

    fclose(train_vtrs);
    fclose(train_labels);
    fclose(test_vtrs);
    fclose(test_labels);
}

void tc_knn_error()
{
    int count [100] = {0};
    for (int i = 0; i < test.size(); i++)
    {
        data_set mn_nn = k_nn(100, test[i], train);
        for (int j = 0; j < mn_nn.size(); j++)
        {
            if (mn_nn.get_label(j) == test.get_label(i))
            {
                count[j]++;
            }
        }
    }
    for (int i = 0; i < 100; i++)
    {
        if (i > 0) printf(" ");
        printf("%10d%10lf\n", i + 1, (test.size() - count[i]) / round(test.size()));
    }
}

void tc_knn_purity()
{
    int count [20] = {};
    for (int i = 1; i <= test.size(); i++)
    {
        data_set mn_nn = k_nn(20, test[i], train);
        for (int j = 0; j < mn_nn.size(); j++)
        {
            if (mn_nn.get_label(j) != test.get_label(i))
                break;
            count[j]++;
        }
    }
    for (int i = 0; i < 20; i++)
    {
        printf("%10d%10lf\n", i + 1, count[i] / round(test.size()));
    }
}

void tc_c_approx_nn()
{
    int size = test.size();
    int fraction[] = {0};
    double c[6] = {1.0,1.2,1.4,1.6,1.8,2.0};
    double fraction_avg[6] = {0.0};
    for (int k = 0; k < 6; k++)
    {
        for (int i = 0; i < size; i++)
        {
            euclid_vector * i_nn = nn(test[i], train);
            data_set c_nn = c_approx_nn(c[k], test[i], train, i_nn);
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
        /* TODO: make into .dat format? */
        cout<<c[k]<<"   "<<fraction_avg[k]<<endl;
    }
}

void tc_save_kd_tree(const char * file_path)
{
    FILE * output = fopen(file_path, "wb");
    kd_tree_node * root = kd_tree((int)(train.size() * 0.05) ,train);
    print_kd_tree(root);
    save_kd_tree(root, output);
    delete root;
    fclose(output);
}

void tc_load_kd_tree(const char * file_path)
{
    FILE * input = fopen(file_path, "rb");
    kd_tree_node * root = load_kd_tree(input);
    print_kd_tree(root);
    delete root;
    fclose(input);
}

void tc_kd_tree_k_nn_error(const char * file_path)
{
    FILE * input = fopen(file_path, "rb");
    kd_tree_node * root;
    root = load_kd_tree(input);
    int count [100] = {0};
    for (int i = 0; i < test.size(); i++)
    {
        data_set mn_nn = kd_tree_k_nn(100, test[i], train, root);
        for (int j = 0; j < mn_nn.size(); j++)
        {
            #ifdef DEBUG
            cerr << "[DEBUG: tc " << i << ":" << j << " | " <<  test.get_label(i) 
                 << " -> " << mn_nn.get_label(j) << "]" << endl;
            #endif
            if (mn_nn.get_label(j) == test.get_label(i))
            {
                count[j]++;
            }
        }
    }
    for (int i = 0; i < 100; i++)
    {
        if (i > 0) printf(" ");
        printf("%10d%10lf\n", i + 1, (test.size() - count[i]) / round(test.size()));
    }
    fclose(input);
}








/* TODO: implement into tc format
void generate_kd_tree(string input_file_name, data_set train)
{
    clock_t start, end;
    int c = 0.05 * DATASIZE;
    start = clock();
    kd_tree_node * root = kd_tree(c, train);
    end = clock();
    double time = double(end - start) / CLOCKS_PER_SEC;
    cout << time << endl;
    FILE * kd_tree_out = fopen((janet_dir+input_file_name).c_str(), "wb");
    save_tree(root, kd_tree_out);
    fclose(kd_tree_out);
    delete root;
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
    query_tree_node * spill_root = build_query_tree(* train, root, root->get_domain());
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
*/
