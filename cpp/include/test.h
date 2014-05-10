#ifndef TEST_H_
#define TEST_H_

#include <map>
#include <thread>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "data_set.h"
#include "kd_tree.h"
#include "kd_spill_tree.h"
#include "kd_virtual_spill_tree.h"

#ifndef NN_DATA_TYPES_
#define NN_DATA_TYPES_
#define COL_W           (20)
#define ERROR_RATE      (0x0001)
#define TRUE_NN         (0x0002)
#define SUBDOMAIN       (0x0004)
#endif

static double l [] = {0.02, 0.05, 0.1};
static size_t l_len = 2;
static double a [] = {0, 0.05, 0.1, 0.15};
static size_t a_len = 3;

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
        stringstream dir; 
        dir << base_dir_ << "/kd_tree_" << setprecision(2) << ll;
        #ifdef DEBUG
        cerr << "[DEBUG: Generating kd-tree " << ll << "]" << endl;
        #endif
        KDTree<Label, T> tree ((size_t)(ll * (*trn_st_).size()), *trn_st_);
        ofstream tree_out (dir.str());
        tree.save(tree_out);
        tree_out.close();
    }

    void generate_kd_trees()
    {
        for (size_t i = 0; i < l_len; i++)
        {
            s_kd_tree(l[i]);
        }
    }

    void t_generate_kd_trees()
    {
        thread t [l_len];
        for (size_t i = 0; i < l_len; i++)
        {
            t[i] = thread(s_kd_tree, l[i]);
        }
    }

    void s_kd_spill_tree(double ll, double la)
    {
        stringstream dir; 
        dir << base_dir_ << "/kd_spill_tree_" << setprecision(2) << la << "_" << ll;
        #ifdef DEBUG
        cerr << "[DEBUG: Generating kd-spill-tree " << la << " " << ll << "]" << endl;
        #endif
        KDSpillTree<Label, T> tree ((size_t)(ll * (*trn_st_).size()), la, *trn_st_);
        ofstream tree_out (dir.str());
        tree.save(tree_out);
        tree_out.close();
    }

    void generate_kd_spill_trees()
    {
        for (size_t i = 0; i < l_len; i++)
        {
            for (size_t j = 0; j < a_len; j++)
            {
                s_kd_spill_tree(l[i], a[j]);
            }
        }
    }

    void t_generate_kd_spill_trees()
    {
        thread t [l_len][a_len];
        for (size_t i = 0; i < l_len; i++)
        {
            for (size_t j = 0; j < a_len; j++)
            {
                t[i][j] = thread(s_kd_spill_tree, l[i], a[j]);
            }
        }
        for (size_t i = 0; i < l_len; i++)
        {
            for (size_t j = 0; j < a_len; j++)
            {
                t[i][j].join();
            }
        }
    }

    void s_kd_v_spill_tree(double ll, double la)
    {
        stringstream dir; 
        dir << base_dir_ << "/kd_v_spill_tree_" << setprecision(2) << la << "_" << ll;
        #ifdef DEBUG
        cerr << "[DEBUG: Generating virtual-spill-tree " << la << " " << ll << "]" << endl;
        #endif
        KDVirtualSpillTree<Label, T> tree ((size_t)(ll * (*trn_st_).size()), la, *trn_st_);
        ofstream tree_out (dir.str());
        tree.save(tree_out);
        tree_out.close();
    }

    void generate_kd_v_spill_trees()
    {
        for (size_t i = 0; i < l_len; i++)
        {
            for (size_t j = 0; j < a_len; j++)
            {
                s_kd_v_spill_tree(l[i], a[j]);
            }
        }
    }

    void t_generate_kd_v_spill_trees()
    {
        thread t [l_len][a_len];
        for (size_t i = 0; i < l_len; i++)
        {
            for (size_t j = 0; j < a_len; j++)
            {
                t[i][j] = thread(s_kd_v_spill_tree, l[i], a[j]);
            }
        }
        for (size_t i = 0; i < l_len; i++)
        {
            for (size_t j = 0; j < a_len; j++)
            {
                t[i][j].join();
            }
        }
    }

    void s_kd_tree_data(double ll,
            ofstream * kd_error_out, 
            ofstream * kd_true_nn_out,
            ofstream * kd_subdomain_out)
    {
        stringstream dir; 
        dir << base_dir_ << "/kd_tree_" << setprecision(2) << ll;
        ifstream tree_in (dir.str());
        KDTree<Label, T> tree (tree_in, *trn_st_);
        size_t error_count = 0;
        size_t true_nn_count = 0;
        unsigned long long subdomain_count = 0;
        for (size_t i = 0; i < (*tst_st_).size(); i++)
        {
            DataSet<T, Label> subSet = (*trn_st_).subset(tree.subdomain((*tst_st_)[i]));
            vector<T> * nn_vtr = nearest_neighbor((*tst_st_)[i],
                                 subSet);
            Label nn_lbl = (*trn_st_).get_label(nn_vtr);
            if (kd_error_out && nn_lbl != (*tst_st_).get_label(i))
                error_count++;
            if (kd_true_nn_out && nn_vtr == (*trn_st_)[nn_mp_[(*tst_st_)[i]][0]])
                true_nn_count++;
            if (kd_subdomain_out)
                subdomain_count += subSet.size();
        }
        if (kd_error_out)
            (*kd_error_out) << setw(COL_W) << ll << setw(COL_W) << (error_count * 1. / (*tst_st_).size()) 
                            << endl;
        if (kd_true_nn_out)
            (*kd_true_nn_out) << setw(COL_W) << ll << setw(COL_W) 
                              << (true_nn_count * 1. / (*tst_st_).size()) << endl;
        if (kd_subdomain_out)
            (*kd_true_nn_out) << setw(COL_W) << ll << setw(COL_W) 
                              << (subdomain_count * 1. / (*tst_st_).size()) << endl;
        
    }

    void generate_kd_tree_data(int type, string out_dir = ".")
    {
        if (!type)
            return;
        ofstream * kd_error_out = NULL;
        ofstream * kd_true_nn_out = NULL;
        ofstream * kd_subdomain_out = NULL;
        if (type & ERROR_RATE)
            kd_error_out = new ofstream(out_dir + "/kd_tree_error.dat");
        if (type & TRUE_NN)
            kd_true_nn_out = new ofstream(out_dir + "/kd_tree_true_nn.dat");
        if (type & SUBDOMAIN)
            kd_true_nn_out = new ofstream(out_dir + "/kd_tree_subdomain.dat");
        for (size_t i = 0; i < l_len; i++)
        {
            s_kd_tree_data(l[i], kd_error_out, kd_true_nn_out, kd_subdomain_out);
        }
        if (kd_error_out)
        {
            kd_error_out->close();
            delete kd_error_out;
        }
        if (kd_true_nn_out)
        {
            kd_true_nn_out->close();
            delete kd_true_nn_out;
        }
        if (kd_subdomain_out)
        {
            kd_subdomain_out->close();
            delete kd_subdomain_out;
        }
    }

    void t_generate_kd_tree_data(int type, string out_dir = ".")
    {
        if (!type)
            return;
        ofstream * kd_error_out = NULL;
        ofstream * kd_true_nn_out = NULL;
        ofstream * kd_subdomain_out = NULL;
        if (type & ERROR_RATE)
            kd_error_out = new ofstream(out_dir + "/kd_tree_error.dat");
        if (type & TRUE_NN)
            kd_true_nn_out = new ofstream(out_dir + "/kd_tree_true_nn.dat");
        if (type & SUBDOMAIN)
            kd_true_nn_out = new ofstream(out_dir + "/kd_tree_subdomain.dat");
        thread t [l_len];
        for (size_t i = 0; i < l_len; i++)
        {
            t[i] = thread(s_kd_tree_data, l[i], kd_error_out, kd_true_nn_out, kd_subdomain_out);
        }
        for (size_t i = 0; i < l_len; i++)
        {
            t[i].join();
        }
        if (kd_error_out)
        {
            kd_error_out->close();
            delete kd_error_out;
        }
        if (kd_true_nn_out)
        {
            kd_true_nn_out->close();
            delete kd_true_nn_out;
        }
        if (kd_subdomain_out)
        {
            kd_subdomain_out->close();
            delete kd_subdomain_out;
        }
    }

    void s_kd_spill_tree_data(double ll, double la,
            ofstream * kd_error_out, 
            ofstream * kd_true_nn_out,
            ofstream * kd_subdomain_out)
    {
        stringstream dir; 
        dir << base_dir_ << "/kd_spill_tree_" << setprecision(2) << la << "_" << ll;
        ifstream tree_in (dir.str());
        KDSpillTree<Label, T> tree (tree_in, *trn_st_);
        size_t error_count = 0;
        size_t true_nn_count = 0;
        unsigned long long subdomain_count = 0;
        for (size_t i = 0; i < (*tst_st_).size(); i++)
        {
            DataSet<T, Label> subSet = (*trn_st_).subset(tree.subdomain((*tst_st_)[i]));
            vector<T> * nn_vtr = nearest_neighbor((*tst_st_)[i],
                                 subSet);
            Label nn_lbl = (*trn_st_).get_label(nn_vtr);
            if (kd_error_out && nn_lbl != (*tst_st_).get_label(i))
                error_count++;
            if (kd_true_nn_out && nn_vtr == (*trn_st_)[nn_mp_[(*tst_st_)[i]][0]])
                true_nn_count++;
            if (kd_subdomain_out)
                subdomain_count += subSet.size();
        }
        if (kd_error_out)
            (*kd_error_out) << setw(COL_W) << ll << setw(COL_W) << la << setw(COL_W) 
                            << (error_count * 1. / (*tst_st_).size()) << endl;
        if (kd_true_nn_out)
            (*kd_true_nn_out) << setw(COL_W) << ll << setw(COL_W) << la << setw(COL_W) 
                              << (true_nn_count * 1. / (*tst_st_).size()) << endl;
        if (kd_subdomain_out)
            (*kd_true_nn_out) << setw(COL_W) << ll << setw(COL_W) 
                              << (subdomain_count * 1. / (*tst_st_).size()) << endl;
    }

    void generate_kd_spill_tree_data(int type, string out_dir = ".")
    {
        if (!type)
            return;
        ofstream * kd_error_out = NULL;
        ofstream * kd_true_nn_out = NULL;
        ofstream * kd_subdomain_out = NULL;
        if (type & ERROR_RATE)
            kd_error_out = new ofstream(out_dir + "/kd_spill_tree_error.dat");
        if (type & TRUE_NN)
            kd_true_nn_out = new ofstream(out_dir + "/kd_spill_tree_true_nn.dat");
        if (type & SUBDOMAIN)
            kd_true_nn_out = new ofstream(out_dir + "/kd_spill_tree_subdomain.dat");
        for (size_t i = 0; i < l_len; i++)
        {
            for (size_t j = 0; j < a_len; j++)
            {
                s_kd_spill_tree_data(l[i], a[i], kd_error_out, kd_true_nn_out, kd_subdomain_out);
            }
        }
        if (kd_error_out)
        {
            kd_error_out->close();
            delete kd_error_out;
        }
        if (kd_true_nn_out)
        {
            kd_true_nn_out->close();
            delete kd_true_nn_out;
        }
        if (kd_subdomain_out)
        {
            kd_subdomain_out->close();
            delete kd_subdomain_out;
        }
    }

    void t_generate_kd_spill_tree_data(int type, string out_dir = ".")
    {
        if (!type)
            return;
        ofstream * kd_error_out = NULL;
        ofstream * kd_true_nn_out = NULL;
        ofstream * kd_subdomain_out = NULL;
        if (type & ERROR_RATE)
            kd_error_out = new ofstream(out_dir + "/kd_spill_tree_error.dat");
        if (type & TRUE_NN)
            kd_true_nn_out = new ofstream(out_dir + "/kd_spill_tree_true_nn.dat");
        if (type & SUBDOMAIN)
            kd_true_nn_out = new ofstream(out_dir + "/kd_spill_tree_subdomain.dat");
        thread t [l_len][a_len];
        for (size_t i = 0; i < l_len; i++)
        {
            for (size_t j = 0; j < a_len; j++)
            {
                t[i][j] = thread(s_kd_spill_tree_data, l[i], a[i], kd_error_out, kd_true_nn_out, kd_subdomain_out);
            }
        }
        for (size_t i = 0; i < l_len; i++)
        {
            for (size_t j = 0; j < a_len; j++)
            {
                t[i][j].join();
            }
        }
        if (kd_error_out)
        {
            kd_error_out->close();
            delete kd_error_out;
        }
        if (kd_true_nn_out)
        {
            kd_true_nn_out->close();
            delete kd_true_nn_out;
        }
        if (kd_subdomain_out)
        {
            kd_subdomain_out->close();
            delete kd_subdomain_out;
        }
    }

    void s_kd_v_spill_tree_data(double ll, double la,
            ofstream * kd_error_out, 
            ofstream * kd_true_nn_out,
            ofstream * kd_subdomain_out)
    {
        stringstream dir; 
        dir << base_dir_ << "/kd_v_spill_tree_" << setprecision(2) << la << "_" << ll;
        ifstream tree_in (dir.str());
        KDVirtualSpillTree<Label, T> tree (tree_in, *trn_st_);
        size_t error_count = 0;
        size_t true_nn_count = 0;
        unsigned long long subdomain_count = 0;
        for (size_t i = 0; i < (*tst_st_).size(); i++)
        {
            DataSet<T, Label> subSet = (*trn_st_).subset(tree.subdomain((*tst_st_)[i]));
            vector<T> * nn_vtr = nearest_neighbor((*tst_st_)[i],
                                 subSet);
            DataSet<T, Label> l_subdomain = tree.subdomain((*tst_st_)[i]);
            Label nn_lbl = (*trn_st_).get_label(nn_vtr);
            if (kd_error_out && nn_lbl != (*tst_st_).get_label(i))
                error_count++;
            if (kd_true_nn_out && nn_vtr == (*trn_st_)[nn_mp_[(*tst_st_)[i]][0]])
                true_nn_count++;
            if (kd_subdomain_out)
                subdomain_count += subSet.size();
        }
        if (kd_error_out)
            (*kd_error_out) << setw(COL_W) << ll << setw(COL_W) << la << setw(COL_W) 
                            << (error_count * 1. / (*tst_st_).size()) << endl;
        if (kd_true_nn_out)
            (*kd_true_nn_out) << setw(COL_W) << ll << setw(COL_W) << la << setw(COL_W) 
                              << (true_nn_count * 1. / (*tst_st_).size()) << endl;
        if (kd_subdomain_out)
            (*kd_true_nn_out) << setw(COL_W) << ll << setw(COL_W) 
                              << (subdomain_count * 1. / (*tst_st_).size()) << endl;
    }

    void generate_kd_v_spill_tree_data(int type, string out_dir = ".")
    {
        if (!type)
            return;
        ofstream * kd_error_out = NULL;
        ofstream * kd_true_nn_out = NULL;
        ofstream * kd_subdomain_out = NULL;
        if (type & ERROR_RATE)
            kd_error_out = new ofstream(out_dir + "/kd_v_spill_tree_error.dat");
        if (type & TRUE_NN)
            kd_true_nn_out = new ofstream(out_dir + "/kd_v_spill_tree_true_nn.dat");
        if (type & SUBDOMAIN)
            kd_subdomain_out = new ofstream(out_dir + "/kd_v_spill_tree_subdomain.dat");
        for (size_t i = 0; i < l_len; i++)
        {
            for (size_t j = 0; j < a_len; j++)
            {
                s_kd_v_spill_tree_data(l[i], a[j], kd_error_out, kd_true_nn_out, kd_subdomain_out);
            }
        }
        if (kd_error_out)
        {
            kd_error_out->close();
            delete kd_error_out;
        }
        if (kd_true_nn_out)
        {
            kd_true_nn_out->close();
            delete kd_true_nn_out;
        }
        if (kd_subdomain_out)
        {
            kd_subdomain_out->close();
            delete kd_subdomain_out;
        }
    }

    void t_generate_kd_v_spill_tree_data(int type, string out_dir = ".")
    {
        if (!type)
            return;
        ofstream * kd_error_out = NULL;
        ofstream * kd_true_nn_out = NULL;
        ofstream * kd_subdomain_out = NULL;
        if (type & ERROR_RATE)
            kd_error_out = new ofstream(out_dir + "/kd_v_spill_tree_error.dat");
        if (type & TRUE_NN)
            kd_true_nn_out = new ofstream(out_dir + "/kd_v_spill_tree_true_nn.dat");
        if (type & SUBDOMAIN)
            kd_subdomain_out = new ofstream(out_dir + "/kd_v_spill_tree_subdomain.dat");
        thread t [l_len][a_len];
        for (size_t i = 0; i < l_len; i++)
        {
            for (size_t j = 0; j < a_len; j++)
            {
                t[i][j] = thread(s_kd_v_spill_tree_data, l[i], a[j], kd_error_out, kd_true_nn_out, kd_subdomain_out);
            }
        }
        for (size_t i = 0; i < l_len; i++)
        {
            for (size_t j = 0; j < a_len; j++)
            {
                t[i][j].join();
            }
        }
        if (kd_error_out)
        {
            kd_error_out->close();
            delete kd_error_out;
        }
        if (kd_true_nn_out)
        {
            kd_true_nn_out->close();
            delete kd_true_nn_out;
        }
        if (kd_subdomain_out)
        {
            kd_subdomain_out->close();
            delete kd_subdomain_out;
        }
    }
};


template<class Label, class T>
Test<Label, T>::Test(string base_dir) :
  base_dir_ (base_dir)
{
    #ifdef DEBUG
    cerr << "[DEBUG: Loading datasets]" << endl;
    cerr << "[DEBUG: Loading vectors]" << endl;
    #endif
    ifstream trn_vtr_in (base_dir + "/trn_vtr");
    ifstream tst_vtr_in (base_dir + "/tst_vtr");
    trn_st_ = new DataSet<Label, T>(trn_vtr_in);
    tst_st_ = new DataSet<Label, T>(tst_vtr_in);
    trn_vtr_in.close();
    tst_vtr_in.close();
    #ifdef DEBUG
    cerr << "[DEBUG: Labeling dataset]" << endl;
    #endif
    ifstream trn_lbl_in (base_dir + "/trn_lbl");
    ifstream tst_lbl_in (base_dir + "/tst_lbl");
    trn_st_->label(trn_lbl_in);
    tst_st_->label(tst_lbl_in);
    trn_lbl_in.close();
    tst_lbl_in.close();
    #ifdef DEBUG
    cerr << "[DEBUG: SUCCESS]" << endl;
    cerr << "[DEBUG: Looking for k_true_nn]" << endl;
    #endif
    ifstream nn_dat_in (base_dir + "/k_true_nn", ios::binary);
    if (nn_dat_in.good())
    {
        size_t k;
        nn_dat_in.read((char *)&k, sizeof(size_t));
        #ifdef DEBUG
        cerr << "[DEBUG: k_true_nn found!!!]" << endl;
        cerr << "[DEBUG: Parsing k_true_nn with k = " << k << "]" << endl;
        #endif
        for (size_t i = 0; i < tst_st_->size(); i++)
        {
            #ifdef DEBUG
            cerr << "[DEBUG: " << i  << " |";
            #endif
            for (int j = 0; j < k; j++)
            {
                size_t nn; 
                nn_dat_in.read((char *)&nn, sizeof(size_t));
                nn_mp_[(*tst_st_)[i]].push_back(nn);
                #ifdef DEBUG
                cerr << " " << nn;
                #endif
            }
            #ifdef DEBUG
            cerr << "]" << endl;
            #endif
        }
        nn_dat_in.close();
        #ifdef DEBUG
        cerr << "[DEBUG: SUCCESS]" << endl;
        #endif
    }
    else
    {
        #ifdef DEBUG
        cerr << "[DEBUG: k_true_nn not found!!!]" << endl;
        #endif
        size_t k = 10;
        nn_dat_in.close();
        #ifdef DEBUG
        cerr << "[DEBUG: Generating k_true_nn with k = " << k << "]" << endl;
        #endif
        ofstream nn_dat_out (base_dir + "/k_true_nn", ios::binary);
        nn_dat_out.write((char *)&k, sizeof(size_t));
        for (size_t i = 0; i < tst_st_->size(); i++)
        {
            #ifdef DEBUG
            cerr << "[DEBUG: " << i  << " |";
            #endif
            DataSet<Label, T> l_st = k_nearest_neighbor(k, (*tst_st_)[i], *trn_st_);
            for (size_t j = 0; j < k; j++)
            {

                nn_dat_out.write((char *)&(l_st.get_domain()[j]), sizeof(size_t));
                nn_mp_[(*tst_st_)[i]].push_back(l_st.get_domain()[j]);
                #ifdef DEBUG
                cerr << " " << l_st.get_domain()[j];
                #endif
            }
            #ifdef DEBUG
            cerr << "]" << endl;
            #endif
        }
        nn_dat_out.close();
        #ifdef DEBUG
        cerr << "[DEBUG: SUCCESS]" << endl;
        #endif
    }
}

template<class Label, class T>
Test<Label, T>::~Test()
{
    delete trn_st_;
    delete tst_st_;
}

#endif
