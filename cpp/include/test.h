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

static double min_leaf  = 0.005;
static double l []      = {0.025, 0.05, 0.075, 0.1};
const size_t l_len      = 4;
static double a []      = {0.05, 0.1};
const size_t a_len      = 2;

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

    void s_kd_tree(double ll)
    {
        LOG_INFO("Generating KDTree %lf\n", ll);
        stringstream dir; 
        dir << base_dir_ << "/kd_tree_" << setprecision(2) << ll;
        KDTree<Label, T> tree ((size_t)(ll * (*trn_st_).size()), *trn_st_);
        ofstream tree_out (dir.str());
        tree.save(tree_out);
        tree_out.close();
    }

    void generate_kd_trees()
    {
        s_kd_tree(min_leaf);
    }

    void s_kd_spill_tree(double ll, double la)
    {
        LOG_INFO("Generating KDSpillTree %lf %lf\n", la, ll);
        stringstream dir; 
        dir << base_dir_ << "/kd_spill_tree_" << setprecision(2) << la << "_" << ll;
        KDSpillTree<Label, T> tree ((size_t)(ll * (*trn_st_).size()), la, *trn_st_);
        ofstream tree_out (dir.str());
        tree.save(tree_out);
        tree_out.close();
    }

    void generate_kd_spill_trees()
    {
        thread t [a_len];
        for (size_t i = 0; i < a_len; i++)
        {
            t[i] = thread(&Test<T, Label>::s_kd_spill_tree, this, min_leaf, a[i]);
        }
        for (size_t i = 0; i < a_len; i++)
        {
            t[i].join();
        }
    }

    void s_kd_v_spill_tree(double ll, double la)
    {
        LOG_INFO("Generating KDVirtualSpillTree %lf %lf\n", la, ll);
        stringstream dir; 
        dir << base_dir_ << "/kd_v_spill_tree_" << setprecision(2) << la << "_" << ll;
        KDVirtualSpillTree<Label, T> tree ((size_t)(ll * (*trn_st_).size()), la, *trn_st_);
        ofstream tree_out (dir.str());
        tree.save(tree_out);
        tree_out.close();
    }

    void generate_kd_v_spill_trees()
    {
        thread t [a_len];
        for (size_t i = 0; i < a_len; i++)
        {
            t[i] = thread(&Test<T, Label>::s_kd_v_spill_tree, this, min_leaf, a[i]);
        }
        for (size_t i = 0; i < a_len; i++)
        {
            t[i].join();
        }
    }

    void s_bsp_tree(double ll)
    {
        LOG_INFO("Generating BSPTree %lf\n", ll);
        stringstream dir; 
        dir << base_dir_ << "/bsp_tree_" << setprecision(2) << ll;
        BSPTree<Label, T> tree ((size_t)(ll * (*trn_st_).size()), *trn_st_);
        ofstream tree_out (dir.str());
        tree.save(tree_out);
        tree_out.close();
    }

    void generate_bsp_trees()
    {
        s_bsp_tree(min_leaf);
    }

    void s_bsp_spill_tree(double ll, double la)
    {
        LOG_INFO("Generating BSPSpillTree %lf %lf\n", la,  ll);
        stringstream dir; 
        dir << base_dir_ << "/bsp_spill_tree_" << setprecision(2) << la << "_" << ll;
        BSPSpillTree<Label, T> tree ((size_t)(ll * (*trn_st_).size()), la, *trn_st_);
        ofstream tree_out (dir.str());
        tree.save(tree_out);
        tree_out.close();
    }

    void generate_bsp_spill_trees()
    {
        thread t [a_len];
        for (size_t i = 0; i < a_len; i++)
        {
            t[i] = thread(&Test<T, Label>::s_bsp_spill_tree, this, min_leaf, a[i]);
        }
        for (size_t i = 0; i < a_len; i++)
        {
            t[i].join();
        }
    }

    void s_kd_tree_data(double ll, string * result)
    {
        LOG_INFO("Generating KDTree %lf data\n", ll);
        stringstream dir; 
        dir << base_dir_ << "/kd_tree_" << setprecision(2) << min_leaf;
        ifstream tree_in (dir.str());
        KDTree<Label, T> tree (tree_in, *trn_st_);
        size_t error_count = 0;
        size_t true_nn_count = 0;
        unsigned long long subdomain_count = 0;
        for (size_t i = 0; i < (*tst_st_).size(); i++)
        {
            DataSet<T, Label> subSet = (*trn_st_).subset(tree.subdomain((*tst_st_)[i], (size_t)(ll * (*trn_st_).size())));
            vector<T> * nn_vtr = nearest_neighbor((*tst_st_)[i], subSet);
            Label nn_lbl = (*trn_st_).get_label(nn_vtr);
            if (nn_lbl != (*tst_st_).get_label(i))
                error_count++;
            if (nn_vtr == (*trn_st_)[nn_mp_[(*tst_st_)[i]][0]])
                true_nn_count++;
            subdomain_count += subSet.size();
        }
        stringstream data;
        data <<  setw(COL_W) <<  ll;
        data <<  setw(COL_W) << (error_count * 1. / (*tst_st_).size());
        data <<  setw(COL_W) << (true_nn_count * 1. / (*tst_st_).size());
        data <<  setw(COL_W) << (subdomain_count * 1. / (*tst_st_).size());
        data << endl;
        *result = data.str();
    }

    void generate_kd_tree_data(string out_dir = ".")
    {
        ofstream kd_out (out_dir + "/kd_tree.dat");
        kd_out <<  setw(COL_W) << "leaf";
        kd_out <<  setw(COL_W) << "error rate";
        kd_out <<  setw(COL_W) << "true nn";
        kd_out <<  setw(COL_W) << "subdomain";
        kd_out << endl;
        thread t [l_len];
        string r [l_len];
        for (size_t i = 0; i < l_len; i++)
        {
            t[i] = thread(&Test::s_kd_tree_data, this, l[i], &(r[i]));
        }
        for (size_t i = 0; i < l_len; i++)
        {
            t[i].join();
            kd_out << r[i];
        }
        kd_out.close();
    }

    void s_kd_spill_tree_data(double ll, double la, string * result)
    {
        LOG_INFO("Generating KDSpillTree %lf %lf data\n", la, ll);
        stringstream dir; 
        dir << base_dir_ << "/kd_spill_tree_" << setprecision(2) << la << "_" << min_leaf;
        ifstream tree_in (dir.str());
        KDSpillTree<Label, T> tree (tree_in, *trn_st_);
        size_t error_count = 0;
        size_t true_nn_count = 0;
        unsigned long long subdomain_count = 0;
        for (size_t i = 0; i < (*tst_st_).size(); i++)
        {
            DataSet<T, Label> subSet = (*trn_st_).subset(tree.subdomain((*tst_st_)[i], (size_t)(ll * (*trn_st_).size())));
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
        data <<  setw(COL_W) <<  ll;
        data <<  setw(COL_W) <<  la;
        data <<  setw(COL_W) << (error_count * 1. / (*tst_st_).size());
        data <<  setw(COL_W) << (true_nn_count * 1. / (*tst_st_).size());
        data <<  setw(COL_W) << (subdomain_count * 1. / (*tst_st_).size());
        data << endl;
        *result = data.str();
    }

    void generate_kd_spill_tree_data(string out_dir = ".")
    {
        ofstream kd_out (out_dir + "/kd_spill_tree.dat");
        kd_out <<  setw(COL_W) << "leaf";
        kd_out <<  setw(COL_W) << "alpha";
        kd_out <<  setw(COL_W) << "error rate";
        kd_out <<  setw(COL_W) << "true nn";
        kd_out <<  setw(COL_W) << "subdomain";
        kd_out << endl;
        thread t [l_len][a_len];
        string r [l_len][a_len];
        for (size_t i = 0; i < l_len; i++)
        {
            for (size_t j = 0; j < a_len; j++)
            {
                t[i][j] = thread(&Test<T, Label>::s_kd_spill_tree_data, this, l[i], a[j], &(r[i][j]));
            }
        }
        for (size_t i = 0; i < l_len; i++)
        {
            for (size_t j = 0; j < a_len; j++)
            {
                t[i][j].join();
                kd_out << r[i][j];
            }
        }
        kd_out.close();
    }

    void s_kd_v_spill_tree_data(double ll, double la, string * result)
    {
        LOG_INFO("Generating KDVirtualSpillTree %lf %lf data\n", la, ll);
        stringstream dir; 
        dir << base_dir_ << "/kd_v_spill_tree_" << setprecision(2) << la << "_" << min_leaf;
        ifstream tree_in (dir.str());
        KDVirtualSpillTree<Label, T> tree (tree_in, *trn_st_);
        size_t error_count = 0;
        size_t true_nn_count = 0;
        unsigned long long subdomain_count = 0;
        for (size_t i = 0; i < (*tst_st_).size(); i++)
        {
            DataSet<T, Label> subSet = (*trn_st_).subset(tree.subdomain((*tst_st_)[i], (size_t)(ll * (*trn_st_).size())));
            vector<T> * nn_vtr = nearest_neighbor((*tst_st_)[i], subSet);
            Label nn_lbl = (*trn_st_).get_label(nn_vtr);
            if (nn_lbl != (*tst_st_).get_label(i))
                error_count++;
            if (nn_vtr == (*trn_st_)[nn_mp_[(*tst_st_)[i]][0]])
                true_nn_count++;
            subdomain_count += subSet.size();
        }
        stringstream data;
        data <<  setw(COL_W) <<  ll;
        data <<  setw(COL_W) <<  la;
        data <<  setw(COL_W) << (error_count * 1. / (*tst_st_).size());
        data <<  setw(COL_W) << (true_nn_count * 1. / (*tst_st_).size());
        data <<  setw(COL_W) << (subdomain_count * 1. / (*tst_st_).size());
        data << endl;
        *result = data.str();
    }

    void generate_kd_v_spill_tree_data(string out_dir = ".")
    {
        ofstream kd_out (out_dir + "/kd_v_spill_tree.dat");
        kd_out <<  setw(COL_W) << "leaf";
        kd_out <<  setw(COL_W) << "alpha";
        kd_out <<  setw(COL_W) << "error rate";
        kd_out <<  setw(COL_W) << "true nn";
        kd_out <<  setw(COL_W) << "subdomain";
        kd_out << endl;
        thread t [l_len][a_len];
        string r [l_len][a_len];
        for (size_t i = 0; i < l_len; i++)
        {
            for (size_t j = 0; j < a_len; j++)
            {
                t[i][j] = thread(&Test<T, Label>::s_kd_v_spill_tree_data, this, l[i], a[j], &(r[i][j]));
            }
        }
        for (size_t i = 0; i < l_len; i++)
        {
            for (size_t j = 0; j < a_len; j++)
            {
                t[i][j].join();
                kd_out << r[i][j];
            }
        }
        kd_out.close();
    }

    void s_bsp_tree_data(double ll, string * result)
    {
        LOG_INFO("Generating BSPTree %lf data\n", ll);
        stringstream dir; 
        dir << base_dir_ << "/bsp_tree_" << setprecision(2) << min_leaf;
        ifstream tree_in (dir.str());
        BSPTree<Label, T> tree (tree_in, *trn_st_);
        size_t error_count = 0;
        size_t true_nn_count = 0;
        unsigned long long subdomain_count = 0;
        for (size_t i = 0; i < (*tst_st_).size(); i++)
        {
            DataSet<T, Label> subSet = (*trn_st_).subset(tree.subdomain((*tst_st_)[i], (size_t)(ll * (*trn_st_).size())));
            vector<T> * nn_vtr = nearest_neighbor((*tst_st_)[i], subSet);
            Label nn_lbl = (*trn_st_).get_label(nn_vtr);
            if (nn_lbl != (*tst_st_).get_label(i))
                error_count++;
            if (nn_vtr == (*trn_st_)[nn_mp_[(*tst_st_)[i]][0]])
                true_nn_count++;
            subdomain_count += subSet.size();
        }
        stringstream data;
        data <<  setw(COL_W) <<  ll;
        data <<  setw(COL_W) << (error_count * 1. / (*tst_st_).size());
        data <<  setw(COL_W) << (true_nn_count * 1. / (*tst_st_).size());
        data <<  setw(COL_W) << (subdomain_count * 1. / (*tst_st_).size());
        data << endl;
        *result = data.str();
    }

    void generate_bsp_tree_data(string out_dir = ".")
    {
        ofstream kd_out (out_dir + "/bsp_tree.dat");
        kd_out <<  setw(COL_W) << "leaf";
        kd_out <<  setw(COL_W) << "error rate";
        kd_out <<  setw(COL_W) << "true nn";
        kd_out <<  setw(COL_W) << "subdomain";
        kd_out << endl;
        thread t [l_len];
        string r [l_len];
        for (size_t i = 0; i < l_len; i++)
        {
            t[i] = thread(&Test::s_bsp_tree_data, this, l[i], &(r[i]));
        }
        for (size_t i = 0; i < l_len; i++)
        {
            t[i].join();
            kd_out << r[i];
        }
        kd_out.close();
    }

    void s_bsp_spill_tree_data(double ll, double la, string * result)
    {
        LOG_INFO("Generating BSPSpillTree %lf %lf data\n", la, ll);
        stringstream dir; 
        dir << base_dir_ << "/bsp_spill_tree_" << setprecision(2) << la << "_" << min_leaf;
        ifstream tree_in (dir.str());
        BSPSpillTree<Label, T> tree (tree_in, *trn_st_);
        size_t error_count = 0;
        size_t true_nn_count = 0;
        unsigned long long subdomain_count = 0;
        for (size_t i = 0; i < (*tst_st_).size(); i++)
        {
            DataSet<T, Label> subSet = (*trn_st_).subset(tree.subdomain((*tst_st_)[i], (size_t)(ll * (*trn_st_).size())));
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
        data <<  setw(COL_W) <<  ll;
        data <<  setw(COL_W) <<  la;
        data <<  setw(COL_W) << (error_count * 1. / (*tst_st_).size());
        data <<  setw(COL_W) << (true_nn_count * 1. / (*tst_st_).size());
        data <<  setw(COL_W) << (subdomain_count * 1. / (*tst_st_).size());
        data << endl;
        *result = data.str();
    }

    void generate_bsp_spill_tree_data(string out_dir = ".")
    {
        ofstream kd_out (out_dir + "/bsp_spill_tree.dat");
        kd_out <<  setw(COL_W) << "leaf";
        kd_out <<  setw(COL_W) << "alpha";
        kd_out <<  setw(COL_W) << "error rate";
        kd_out <<  setw(COL_W) << "true nn";
        kd_out <<  setw(COL_W) << "subdomain";
        kd_out << endl;
        thread t [l_len][a_len];
        string r [l_len][a_len];
        for (size_t i = 0; i < l_len; i++)
        {
            for (size_t j = 0; j < a_len; j++)
            {
                t[i][j] = thread(&Test<T, Label>::s_bsp_spill_tree_data, this, l[i], a[j], &(r[i][j]));
            }
        }
        for (size_t i = 0; i < l_len; i++)
        {
            for (size_t j = 0; j < a_len; j++)
            {
                t[i][j].join();
                kd_out << r[i][j];
            }
        }
        kd_out.close();
    }
};

template<class Label, class T>
Test<Label, T>::Test(string base_dir) :
  base_dir_ (base_dir)
{
    LOG_INFO("Loading data sets\n");
    ifstream trn_vtr_in (base_dir + "/trn_vtr");
    ifstream tst_vtr_in (base_dir + "/tst_vtr");
    trn_st_ = new DataSet<Label, T>(trn_vtr_in);
    tst_st_ = new DataSet<Label, T>(tst_vtr_in);
    trn_vtr_in.close();
    tst_vtr_in.close();
    LOG_INFO("Labeling data sets\n");
    ifstream trn_lbl_in (base_dir + "/trn_lbl");
    ifstream tst_lbl_in (base_dir + "/tst_lbl");
    trn_st_->label(trn_lbl_in);
    tst_st_->label(tst_lbl_in);
    trn_lbl_in.close();
    tst_lbl_in.close();
    LOG_INFO("Success!\n");
    ifstream nn_dat_in (base_dir + "/k_true_nn", ios::binary);
    if (nn_dat_in.good())
    {
        size_t k;
        nn_dat_in.read((char *)&k, sizeof(size_t));
        LOG_INFO("File \"k_true_nn\" found!!!\n");
        LOG_INFO("Parsing file with k = %ld\n", k);
        for (size_t i = 0; i < tst_st_->size(); i++)
        {
            for (int j = 0; j < k; j++)
            {
                size_t nn; 
                nn_dat_in.read((char *)&nn, sizeof(size_t));
                nn_mp_[(*tst_st_)[i]].push_back(nn);
            }
        }
        nn_dat_in.close();
        LOG_INFO("Success!\n");
    }
    else
    {
        LOG_WARNING("File \"k_true_nn\" not found!!!\n");
        size_t k = 10;
        nn_dat_in.close();
        LOG_WARNING("Generating \"k_true_nn\" with k = %ld\n", k);
        ofstream nn_dat_out (base_dir + "/k_true_nn", ios::binary);
        nn_dat_out.write((char *)&k, sizeof(size_t));
        for (size_t i = 0; i < tst_st_->size(); i++)
        {
            DataSet<Label, T> l_st = k_nearest_neighbor(k, (*tst_st_)[i], *trn_st_);
            for (size_t j = 0; j < k; j++)
            {

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

#endif
