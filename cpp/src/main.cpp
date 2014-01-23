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

/* % error for the kth nearest neighbor [1:100] */
void tc_k_nn_error();
/* % purity for the [1:k]th nearest neighbor [1:20] */
void tc_k_nn_purity();
/* % c[1:2] appromation nearest neighbor */
void tc_c_approx_nn();
/* % error for the kth nearest neighbor based on kd_tree [1:100]*/
void tc_kd_tree_k_nn_error(const char * file_path);
/* % nearest neighbor based on kd_tree is actual nearest neighbor */
void tc_kd_tree_true_nn(const char * file_path);
/* % error for the kth nearest neighbor based on query_tree [1:100]*/
void tc_query_tree_k_nn_error(double a, const char * file_path);

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

    tc_query_tree_k_nn_error(0.1, "data/mnist/trees/kd_tree");

    fclose(train_vtrs);
    fclose(train_labels);
    fclose(test_vtrs);
    fclose(test_labels);
}

void tc_k_nn_error()
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
        printf("%10d%10lf\n", i + 1, (test.size() - count[i]) / round(test.size()));
    }
}

void tc_k_nn_purity()
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
        printf("%10d%10lf\n", i + 1, (test.size() - count[i]) / round(test.size()));
    }
    fclose(input);
    delete root;
}

void tc_kd_tree_true_nn(const char * file_path)
{
    FILE * input = fopen(file_path, "rb");
    kd_tree_node * root;
    root = load_kd_tree(input);
    int count;
    for (int i = 0; i < test.size(); i++)
    {
        euclid_vector * kd_nn = kd_tree_nn(test[i], train, root);
        euclid_vector * true_nn = nn(test[i], train);
        #ifdef DEBUG
        cerr << "[DEBUG: tc " << i << " | " <<  kd_nn 
             << " -> " << true_nn << "]" << endl;
        #endif
        if (kd_nn == true_nn)
            count++;
    }
    printf("%10d%10lf\n", 0, count / round(test.size()));
    fclose(input);
    delete root;
}

void tc_query_tree_k_nn_error(double a, const char * file_path)
{
    FILE * input = fopen(file_path, "rb");
    kd_tree_node * root;
    root = load_kd_tree(input);
    query_tree_node * q_root = query_tree(a, root, train);
    int count [100] = {0};
    for (int i = 0; i < test.size(); i++)
    {
        data_set mn_nn = query_tree_k_nn(100, test[i], train, q_root);
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
        printf("%10d%10lf\n", i + 1, (test.size() - count[i]) / round(test.size()));
    }
    fclose(input);
    delete q_root;
    delete root;
}
