#ifndef _TEST_H
#define _TEST_H

#include <map>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "data_set.h"
#include "kd_tree.h"
#include "kd_spill_tree.h"
#include "kd_virtual_spill_tree.h"

static double a [] = {0.05, 0.1, 0.15};
static size_t a_len = 3;
static double l [] = {0.02, 0.05, 0.1};
static size_t l_len = 3;

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

    void generate_kd_trees()
    {
        #ifdef DEBUG
        cerr << "[DEBUG: generating trees]" << endl;
        #endif
        for (size_t j = 0; j < l_len; j++)
        {
            stringstream dir; 
            dir << base_dir_ << "/kd_tree_" << setprecision(2) << l[j];
            #ifdef DEBUG
            cerr << "[DEBUG: Generating kd-tree " << l[j] << "]" << endl;
            #endif
            KDTree<Label, T> tree ((size_t)(l[j] * (*trn_st_).size()), *trn_st_);
            ofstream tree_out (dir.str());
            tree.save(tree_out);
            tree_out.close();
        }
    }

    void generate_kd_spill_trees()
    {
        for (size_t i = 0; i < a_len; i++)
        {
            for (size_t j = 0; j < l_len; j++)
            {
                stringstream dir; 
                dir << base_dir_ << "/kd_spill_tree_" << setprecision(2) << a[i] << "_" << l[j];
                #ifdef DEBUG
                cerr << "[DEBUG: Generating kd-spill-tree " << a[i] << " " << l[j] << "]" << endl;
                #endif
                KDSpillTree<Label, T> tree ((size_t)(l[j] * (*trn_st_).size()), a[i], *trn_st_);
                ofstream tree_out (dir.str());
                tree.save(tree_out);
                tree_out.close();
            }
        }
    }

    void generate_kd_v_spill_tree()
    {
        for (size_t i = 0; i < a_len; i++)
        {
            for (size_t j = 0; j < l_len; j++)
            {
                stringstream dir; 
                dir << base_dir_ << "/kd_v_spill_tree_" << setprecision(2) << a[i] << "_" << l[j];
                #ifdef DEBUG
                cerr << "[DEBUG: Generating virtual-spill-tree " << a[i] << " " << l[j] << "]" << endl;
                #endif
                KDVirtualSpillTree<Label, T> tree ((size_t)(l[j] * (*trn_st_).size()), a[i], *trn_st_);
                ofstream tree_out (dir.str());
                tree.save(tree_out);
                tree_out.close();
            }
        }
    }

    void generate_kd_tree_error_data(string out_dir = ".")
    {
        ofstream kd_out (out_dir + "/kd_tree_error.dat");
        {
            for (size_t i = 0; i < a_len; i++)
            {
                stringstream dir; 
                dir << base_dir_ << "/kd_tree_" << setprecision(2) << l[i];
                ifstream tree_in (dir.str());
                KDTree<Label, T> tree (tree_in, *trn_st_);
                size_t count = 0;
                for (size_t i = 0; i < (*tst_st_).size(); i++)
                {
                    Label nn_lbl = (*trn_st_).get_label(nearest_neighbor((*tst_st_)[i],
                                   (*trn_st_).subset(tree.subdomain((*tst_st_)[i]))));
                    if (nn_lbl != (*tst_st_).get_label(i))
                    {
                        count++;
                    }
                }
                kd_out << a[i] << "\t\t" << (count * 1. / (*tst_st_).size()) << endl;
            }
        }
        kd_out.close();
    }

    void generate_kd_spill_tree_error_data(string out_dir = ".")
    {
        ofstream spill_out (out_dir + "/kd_spill_tree_error.dat");
        for (size_t i = 0; i < a_len; i++)
        {
            for (size_t j = 0; j < l_len; j++)
            {
                stringstream dir; 
                dir << base_dir_ << "/kd_spill_tree_" << setprecision(2) << a[i] << "_" << l[j];
                ifstream tree_in (dir.str());
                KDSpillTree<Label, T> tree (tree_in, *trn_st_);
                size_t count = 0;
                for (size_t i = 0; i < (*tst_st_).size(); i++)
                {
                    Label nn_lbl = (*trn_st_).get_label(nearest_neighbor((*tst_st_)[i],
                                   (*trn_st_).subset(tree.subdomain((*tst_st_)[i]))));
                    if (nn_lbl != (*tst_st_).get_label(i))
                    {
                        count++;
                    }
                }
                spill_out << a[i] << "\t\t" << l[j] << "\t\t" << (count * 1. / (*tst_st_).size()) << endl;
            }
        }
        spill_out.close();
    }

    void generate_kd_v_spill_tree_error_data(string out_dir = ".")
    {
        ofstream v_spill_out (out_dir + "/kd_v_spill_tree_error.dat");
        for (size_t i = 0; i < a_len; i++)
        {
            for (size_t j = 0; j < l_len; j++)
            {
                stringstream dir; 
                dir << base_dir_ << "/kd_v_spill_tree_" << setprecision(2) << a[i] << "_" << l[j];
                ifstream tree_in (dir.str());
                KDVirtualSpillTree<Label, T> tree (tree_in, *trn_st_);
                size_t count = 0;
                for (size_t i = 0; i < (*tst_st_).size(); i++)
                {
                    Label nn_lbl = (*trn_st_).get_label(nearest_neighbor((*tst_st_)[i],
                                   (*trn_st_).subset(tree.subdomain((*tst_st_)[i]))));
                    if (nn_lbl != (*tst_st_).get_label(i))
                    {
                        count++;
                    }
                }
                v_spill_out << a[i] << "\t\t" << l[j] << "\t\t" << (count * 1. / (*tst_st_).size()) << endl;
            }
        }
        v_spill_out.close();
    }

    void generate_kd_tree_true_nn_data(string out_dir = ".")
    {
        ofstream kd_out (out_dir + "/kd_tree_true_nn.dat");
        {
            for (size_t i = 0; i < a_len; i++)
            {
                stringstream dir; 
                dir << base_dir_ << "/kd_tree" << setprecision(2) << l[i];
                ifstream tree_in (dir.str());
                KDTree<Label, T> tree (tree_in, *trn_st_);
                size_t count = 0;
                for (size_t i = 0; i < (*tst_st_).size(); i++)
                {
                    vector<T> * nn_vtr = nearest_neighbor((*tst_st_)[i],
                                        (*trn_st_).subset(tree.subdomain((*tst_st_)[i])));
                    if (nn_vtr != nn_mp_[(*tst_st_)[i]][0])
                    {
                        count++;
                    }
                }
                kd_out << a[i] << "\t\t" << (count * 1. / (*tst_st_).size()) << endl;
            }
        }
    }

    void generate_kd_spill_tree_true_nn_data(string out_dir = ".")
    {
        ofstream spill_out (out_dir + "/kd_spill_tree_true_nn.dat");
        for (size_t i = 0; i < a_len; i++)
        {
            for (size_t j = 0; j < l_len; j++)
            {
                stringstream dir; 
                dir << base_dir_ << "/kd_spill_tree_" << setprecision(2) << a[i] << "_" << l[j];
                ifstream tree_in (dir.str());
                KDSpillTree<Label, T> tree (tree_in, *trn_st_);
                size_t count = 0;
                for (size_t i = 0; i < (*tst_st_).size(); i++)
                {
                    vector<T> * nn_vtr = nearest_neighbor((*tst_st_)[i],
                                        (*trn_st_).subset(tree.subdomain((*tst_st_)[i])));
                    if (nn_vtr != nn_mp_[(*tst_st_)[i]][0])
                    {
                        count++;
                    }
                }
                spill_out << a[i] << "\t\t" << l[j] << "\t\t" << (count * 1. / (*tst_st_).size()) << endl;
            }
        }
        spill_out.close();
    }

    void generate_kd_v_spill_tree_true_nn_data(string out_dir = ".")
    {
        ofstream v_spill_out (out_dir + "/kd_v_spill_tree_true_nn.dat");
        for (size_t i = 0; i < a_len; i++)
        {
            for (size_t j = 0; j < l_len; j++)
            {
                stringstream dir; 
                dir << base_dir_ << "/kd_v_spill_tree_" << setprecision(2) << a[i] << "_" << l[j];
                ifstream tree_in (dir.str());
                KDVirtualSpillTree<Label, T> tree (tree_in, *trn_st_);
                size_t count = 0;
                for (size_t i = 0; i < (*tst_st_).size(); i++)
                {
                    vector<T> * nn_vtr = nearest_neighbor((*tst_st_)[i],
                                        (*trn_st_).subset(tree.subdomain((*tst_st_)[i])));
                    if (nn_vtr != nn_mp_[(*tst_st_)[i]][0])
                    {
                        count++;
                    }
                }
                v_spill_out << a[i] << "\t\t" << l[j] << "\t\t" << (count * 1. / (*tst_st_).size()) << endl;
            }
        }
        v_spill_out.close();
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
