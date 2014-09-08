/* 
 * File             : test.h
 * Date             : 2014-5-29
 * Summary          : Provides the testing interface for the data structures
 */
#ifndef TEST_H_
#define TEST_H_

#include <map>
#include <thread>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "logging.h"
#include "data_set.h"
#include "kd_tree.h"
#include "kd_spill_tree.h"
#include "kd_virtual_spill_tree.h"
#include "bsp_tree.h"
#include "bsp_spill_tree.h"
using namespace std;

#ifndef NN_DATA_TYPES_
#define NN_DATA_TYPES_
#define COL_W           (20)
#define ERROR_RATE      (0x0001)
#define TRUE_NN         (0x0002)
#define SUBDOMAIN       (0x0004)
#endif

#define MAX_THREADS     4

struct Result
{
    double error;
    double true_nn;
    double subdomain;
};

template<class Label, class T>
class Test
{
protected:
    const string base_dir_;
    DataSet<Label, T> * trn_st_;
    DataSet<Label, T> * tst_st_;
    map<vector<T> *, vector<size_t>> nn_mp_;
public:
    Test(string base_dir);
    ~Test();

    void kd_tree(double ll, string path) {
        stringstream dir; 
        dir << base_dir_ << path;
        KDTree<Label, T> tree ((size_t)(ll * (*trn_st_).size()), *trn_st_);
        ofstream tree_out (dir.str());
        tree.save(tree_out);
        tree_out.close();
    }

    void kd_spill_tree(double ll, double la, string path) {
        stringstream dir; 
        dir << base_dir_ << path;
        KDSpillTree<Label, T> tree ((size_t)(ll * (*trn_st_).size()), la, *trn_st_);
        ofstream tree_out (dir.str());
        tree.save(tree_out);
        tree_out.close();
    }

    void kd_v_spill_tree(double ll, double la, string path)
    {
        stringstream dir; 
        dir << base_dir_ << path;
        KDVirtualSpillTree<Label, T> tree ((size_t)(ll * (*trn_st_).size()), la, *trn_st_);
        ofstream tree_out (dir.str());
        tree.save(tree_out);
        tree_out.close();
    }

    void bsp_tree(double ll, string path)
    {
        stringstream dir; 
        dir << base_dir_ << path;
        BSPTree<Label, T> tree ((size_t)(ll * (*trn_st_).size()), *trn_st_);
        ofstream tree_out (dir.str());
        tree.save(tree_out);
        tree_out.close();
    }

    void bsp_spill_tree(double ll, double la, string path)
    {
        stringstream dir; 
        dir << base_dir_ << path;
        BSPSpillTree<Label, T> tree ((size_t)(ll * (*trn_st_).size()), la, *trn_st_);
        ofstream tree_out (dir.str());
        tree.save(tree_out);
        tree_out.close();
    }
    void kd_tree_data(double ll, string path, Result * result)
    {
        stringstream dir; 
        dir << base_dir_ << path;
        ifstream tree_in (dir.str());
        KDTree<Label, T> tree (tree_in, *trn_st_);
        size_t error_count = 0;
        size_t true_nn_count = 0;
        unsigned long long subdomain_count = 0;
        for (size_t i = 0; i < (*tst_st_).size(); i++) {
            DataSet<Label, T> subSet = (*trn_st_).subset(tree.subdomain((*tst_st_)[i], (size_t)(ll * (*trn_st_).size())));
            vector<T> * nn_vtr = nearest_neighbor((*tst_st_)[i], subSet);
            Label nn_lbl = (*trn_st_).get_label(nn_vtr);
            if (nn_lbl != (*tst_st_).get_label(i))
                error_count++;
            if (nn_vtr == (*trn_st_)[nn_mp_[(*tst_st_)[i]][0]])
                true_nn_count++;
            subdomain_count += subSet.size();
        }
        stringstream data;
        result->error = (error_count * 1. / (*tst_st_).size());
        result->true_nn = (true_nn_count * 1. / (*tst_st_).size());
        result->subdomain = (subdomain_count * 1. / (*tst_st_).size());
    }

    void kd_spill_tree_data(double ll, string path, Result * result)
    {
        stringstream dir; 
        dir << base_dir_ << path;
        ifstream tree_in (dir.str());
        KDSpillTree<Label, T> tree (tree_in, *trn_st_);
        size_t error_count = 0;
        size_t true_nn_count = 0;
        unsigned long long subdomain_count = 0;
        for (size_t i = 0; i < (*tst_st_).size(); i++) {
            DataSet<Label, T> subSet = (*trn_st_).subset(tree.subdomain((*tst_st_)[i], (size_t)(ll * (*trn_st_).size())));
            vector<T> * nn_vtr = nearest_neighbor((*tst_st_)[i],
                                 subSet);
            Label nn_lbl = (*trn_st_).get_label(nn_vtr);
            if (nn_lbl != (*tst_st_).get_label(i))
                error_count++;
            if (nn_vtr == (*trn_st_)[nn_mp_[(*tst_st_)[i]][0]])
                true_nn_count++;
            subdomain_count += subSet.size();
        }
        stringstream data;
        result->error = (error_count * 1. / (*tst_st_).size());
        result->true_nn = (true_nn_count * 1. / (*tst_st_).size());
        result->subdomain = (subdomain_count * 1. / (*tst_st_).size());
    }

    void kd_v_spill_tree_data(double ll, string path, Result * result)
    {
        stringstream dir; 
        dir << base_dir_ << path;
        ifstream tree_in (dir.str());
        KDVirtualSpillTree<Label, T> tree (tree_in, *trn_st_);
        size_t error_count = 0;
        size_t true_nn_count = 0;
        unsigned long long subdomain_count = 0;
        for (size_t i = 0; i < (*tst_st_).size(); i++) {
            DataSet<Label, T> subSet = (*trn_st_).subset(tree.subdomain((*tst_st_)[i], (size_t)(ll * (*trn_st_).size())));
            vector<T> * nn_vtr = nearest_neighbor((*tst_st_)[i], subSet);
            Label nn_lbl = (*trn_st_).get_label(nn_vtr);
            if (nn_lbl != (*tst_st_).get_label(i))
                error_count++;
            if (nn_vtr == (*trn_st_)[nn_mp_[(*tst_st_)[i]][0]])
                true_nn_count++;
            subdomain_count += subSet.size();
        }
        stringstream data;
        result->error = (error_count * 1. / (*tst_st_).size());
        result->true_nn = (true_nn_count * 1. / (*tst_st_).size());
        result->subdomain = (subdomain_count * 1. / (*tst_st_).size());
    }

    void bsp_tree_data(double ll, string path, Result * result)
    {
        stringstream dir; 
        dir << base_dir_ << path;
        ifstream tree_in (dir.str());
        BSPTree<Label, T> tree (tree_in, *trn_st_);
        size_t error_count = 0;
        size_t true_nn_count = 0;
        unsigned long long subdomain_count = 0;
        for (size_t i = 0; i < (*tst_st_).size(); i++) {
            DataSet<Label, T> subSet = (*trn_st_).subset(tree.subdomain((*tst_st_)[i], (size_t)(ll * (*trn_st_).size())));
            vector<T> * nn_vtr = nearest_neighbor((*tst_st_)[i], subSet);
            Label nn_lbl = (*trn_st_).get_label(nn_vtr);
            if (nn_lbl != (*tst_st_).get_label(i))
                error_count++;
            if (nn_vtr == (*trn_st_)[nn_mp_[(*tst_st_)[i]][0]])
                true_nn_count++;
            subdomain_count += subSet.size();
        }
        stringstream data;
        result->error = (error_count * 1. / (*tst_st_).size());
        result->true_nn = (true_nn_count * 1. / (*tst_st_).size());
        result->error = (subdomain_count * 1. / (*tst_st_).size());
    }

    void bsp_spill_tree_data(double ll, string path, Result * result)
    {
        stringstream dir; 
        dir << base_dir_ << path;
        ifstream tree_in (dir.str());
        BSPSpillTree<Label, T> tree (tree_in, *trn_st_);
        size_t error_count = 0;
        size_t true_nn_count = 0;
        unsigned long long subdomain_count = 0;
        for (size_t i = 0; i < (*tst_st_).size(); i++) {
            DataSet<Label, T> subSet = (*trn_st_).subset(tree.subdomain((*tst_st_)[i], (size_t)(ll * (*trn_st_).size())));
            vector<T> * nn_vtr = nearest_neighbor((*tst_st_)[i],
                                 subSet);
            Label nn_lbl = (*trn_st_).get_label(nn_vtr);
            if (nn_lbl != (*tst_st_).get_label(i))
                error_count++;
            if (nn_vtr == (*trn_st_)[nn_mp_[(*tst_st_)[i]][0]])
                true_nn_count++;
            subdomain_count += subSet.size();
        }
        stringstream data;
        result->error = (error_count * 1. / (*tst_st_).size());
        result->true_nn = (true_nn_count * 1. / (*tst_st_).size());
        result->subdomain = (subdomain_count * 1. / (*tst_st_).size());
    }

};

template<class Label, class T>
Test<Label, T>::Test(string base_dir) :
  base_dir_ (base_dir)
{
    LOG_INFO("Loading Data Sets\n");
    ifstream trn_vtr_in (base_dir + "/trn_vtr", ios::binary);
    ifstream tst_vtr_in (base_dir + "/tst_vtr", ios::binary);
    trn_st_ = new DataSet<Label, T>(trn_vtr_in);
    tst_st_ = new DataSet<Label, T>(tst_vtr_in);
    trn_vtr_in.close();
    tst_vtr_in.close();
    LOG_INFO("Labeling Data Sets\n");
    ifstream trn_lbl_in (base_dir + "/trn_lbl", ios::binary);
    ifstream tst_lbl_in (base_dir + "/tst_lbl", ios::binary);
    trn_st_->label(trn_lbl_in);
    tst_st_->label(tst_lbl_in);
    trn_lbl_in.close();
    tst_lbl_in.close();
    LOG_INFO("Success!\n");
    ifstream nn_dat_in (base_dir + "/k_true_nn", ios::binary);
    if (nn_dat_in.good()) {
        size_t k;
        nn_dat_in.read((char *)&k, sizeof(size_t));
        LOG_INFO("File \"k_true_nn\" found!!!\n");
        LOG_INFO("Parsing file with k = %ld\n", k);
        for (size_t i = 0; i < tst_st_->size(); i++) {
            for (int j = 0; j < k; j++) {
                size_t nn; 
                nn_dat_in.read((char *)&nn, sizeof(size_t));
                nn_mp_[(*tst_st_)[i]].push_back(nn);
            }
        }
        nn_dat_in.close();
        LOG_INFO("Success!\n");
    } else {
        LOG_WARNING("File \"k_true_nn\" not found!!!\n");
        size_t k = 10;
        nn_dat_in.close();
        LOG_WARNING("Generating \"k_true_nn\" with k = %ld\n", k);
        ofstream nn_dat_out (base_dir + "/k_true_nn", ios::binary);
        nn_dat_out.write((char *)&k, sizeof(size_t));
        for (size_t i = 0; i < tst_st_->size(); i++) {
            DataSet<Label, T> l_st = k_nearest_neighbor(k, (*tst_st_)[i], *trn_st_);
            for (size_t j = 0; j < k; j++) {

                nn_dat_out.write((char *)&(l_st.get_domain()[j]), sizeof(size_t));
                nn_mp_[(*tst_st_)[i]].push_back(l_st.get_domain()[j]);
            }
        }
        nn_dat_out.close();
        LOG_INFO("Success!\n");
    }
}

template<class Label, class T>
Test<Label, T>::~Test()
{
    delete trn_st_;
    delete tst_st_;
}

#endif /* TEST_H_ */
