#include "test.h"

data_set train_st;
data_set test_st;

data_set & get_train_set() 
{
    return train_st;
}

data_set & get_test_set() 
{
    return test_st;
}

void test_setup(string path)
{
    
    ifstream train_vtr_in (path + "train_vectors", ios::binary);
    ifstream test_vtr_in (path + "test_vectors", ios::binary);
    ifstream train_label_in (path + "train_labels", ios::binary);
    ifstream test_label_in (path + "test_labels", ios::binary);

    load_data_set(train_st, train_vtr_in);
    load_data_set(test_st, test_vtr_in);
    label_data_set(train_st, train_label_in);
    label_data_set(test_st, test_label_in);

    train_vtr_in.close();
    test_vtr_in.close();
    train_label_in.close();
    test_label_in.close();
}

/* TODO: integrate into workflow later on 

void build_save_root(int c, data_set & data, FILE * out)
{
    kd_tree_node * root = kd_tree(c, data);
    save_kd_tree(root, out);
}


vector < euclid_vector *> true_nn()
{
    vector <euclid_vector *> true_nn_set;
    for (int i = 0; i < test.size(); i++)
    {
        #ifdef DEBUG
        cerr << "[DEBUG: tc " << i << "]" << endl;
        #endif
        true_nn_set.push_back(nn(test[i], train));
    }
    return true_nn_set;
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
        cout <<c[k]<<"   "<<fraction_avg[k]<<endl;
    }
}


void tc_kd_tree_k_nn_error(const char * file_path, const char * output_path)
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
    ofstream file(output_path);
    for (int i = 0; i < 100; i++)
    {
        file << i + 1 << "  " << (test.size() - count[i]) / round(test.size()) << endl;
    }
    fclose(input);
    delete root;
}


void tc_kd_tree_true_nn(const char * file_path)
{
    FILE * input = fopen(file_path, "rb");
    kd_tree_node * root;
    root = load_kd_tree(input);
    int count = 0;
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


void tc_query_tree_k_nn_error(double a, const char * file_path, const char * output_path)
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
    ofstream file(output_path);
    for (int i = 0; i < 100; i++)
    {
        file << i + 1 << "  " << (test.size() - count[i]) / round(test.size()) << endl;
    }
    fclose(input);
    delete q_root;
    delete root;
}


double tc_query_tree_true_nn(double a, const char * file_path, vector <euclid_vector *> true_nn_set)
{
    FILE * input = fopen(file_path, "rb");
    kd_tree_node * root;
    root = load_kd_tree(input);
    query_tree_node * q_root = query_tree(a, root, train);
    int count = 0;
    for (int i = 0; i < test.size(); i++)
    {
        euclid_vector * q_nn = query_tree_nn(test[i], train, q_root);
        #ifdef DEBUG
        cerr << "[DEBUG: tc " << i << " | " <<  q_nn
        << " -> " << true_nn_set[i] << "]" << endl;
        #endif
        if (q_nn == true_nn_set[i])
            count++;
    }
    fclose(input);
    delete q_root;
    delete root;
    return count / round(test.size());
}

int tc_spill_tree_leaves(const char * file_path)
{
    FILE * input = fopen(file_path, "rb");
    kd_tree_node * root;
    root = load_kd_tree(input);
    stack <kd_tree_node *> to_process;
    to_process.push(root);
    int total_leaves = 0;
    while (!to_process.empty())
    {
        kd_tree_node * focus = to_process.top();
        to_process.pop();
        if (!focus->get_left() && !focus->get_right())
        {
            total_leaves += focus->get_domain().size();
        }
        else
        {
            if (focus->get_left())
                to_process.push(focus->get_left());
            if (focus->get_right())
                to_process.push(focus->get_right());
        }
    }
    return total_leaves;
}

void tc_virtual_spill_tree_leaves(double a, const char * file_path, const char * output_path)
{
    FILE * input = fopen(file_path, "rb");
    kd_tree_node * root;
    root = load_kd_tree(input);
    query_tree_node * q_root = query_tree(a, root, train);
    vector <long> leaves_size;
    for (int i = 0; i < test.size(); i++)
    {
        data_set subset = test.subset(query_subdomain(test[i], q_root));
        leaves_size.push_back(subset.get_domain().size());
    }
    ofstream file(output_path);
    for (int i = 0; i < test.size(); i++)
    {
        file << i << "  " << leaves_size[i] << endl;
    }
}

*/
