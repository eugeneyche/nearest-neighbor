#ifndef _TEST_H
#define _TEST_H

#include <map>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "data_set.h"
#include "kd_tree.h"
#include "spill_tree.h"
#include "virtual_spill_tree.h"

static double a [] = {0.05, 0.1, 0.15};
static size_t a_len = 3;
static double l [] = {0.02, 0.05, 0.1};
static size_t l_len = 3;

template<class Label, class T>
class Test
{
protected:
    const string _base_dir;
    DataSet<Label, T> * _trn_st;
    DataSet<Label, T> * _tst_st;
    map<vector<T> *, vector<size_t>> _nn_mp;
public:
    Test(string base_dir);
    ~Test();
    void generate_trees()
    {
        #ifdef DEBUG
        cerr << "[DEBUG: generating trees]" << endl;
        #endif
        for (size_t j = 0; j < l_len; j++)
        {
            stringstream dir; 
            dir << _base_dir << "/kd_tree_" << setprecision(2) << l[j];
            #ifdef DEBUG
            cerr << "[DEBUG: Generating kd-tree " << l[j] << "]" << endl;
            #endif
            KDTree<Label, T> tree ((size_t)(l[j] * (*_trn_st).size()), *_trn_st);
            ofstream tree_out (dir.str());
            tree.save(tree_out);
            tree_out.close();
        }
        for (size_t i = 0; i < a_len; i++)
        {
            for (size_t j = 0; j < l_len; j++)
            {
                stringstream dir; 
                dir << _base_dir << "/spill_tree_" << setprecision(2) << a[i] << "_" << l[j];
                #ifdef DEBUG
                cerr << "[DEBUG: Generating spill-tree " << a[i] << " " << l[j] << "]" << endl;
                #endif
                SpillTree<Label, T> tree ((size_t)(l[j] * (*_trn_st).size()), a[i], *_trn_st);
                ofstream tree_out (dir.str());
                tree.save(tree_out);
                tree_out.close();
            }
        }
        for (size_t i = 0; i < a_len; i++)
        {
            for (size_t j = 0; j < l_len; j++)
            {
                stringstream dir; 
                dir << _base_dir << "/v_spill_tree_" << setprecision(2) << a[i] << "_" << l[j];
                #ifdef DEBUG
                cerr << "[DEBUG: Generating virtual-spill-tree " << a[i] << " " << l[j] << "]" << endl;
                #endif
                VirtualSpillTree<Label, T> tree ((size_t)(l[j] * (*_trn_st).size()), a[i], *_trn_st);
                ofstream tree_out (dir.str());
                tree.save(tree_out);
                tree_out.close();
            }
        }
    }

    void generate_true_nn_data(string out_dir = ".")
    {
        #ifdef DEBUG
        cerr << "[DEBUG: Generate true nn data]" << endl;
        cerr << "[DEBUG: Writing to directory " << out_dir << "]" << endl;
        #endif
    }

    void generate_error_data(string out_dir = ".")
    {
        #ifdef DEBUG
        cerr << "[DEBUG: Generating error data]" << endl;
        cerr << "[DEBUG: Writing to directory " << out_dir << "]" << endl;
        #endif
        size_t count = 0;
        ofstream kd_out (out_dir + "/kd_tree_error.dat");
        {
            for (size_t i = 0; i < a_len; i++)
            {
                #ifdef DEBUG
                cerr << "[DEBUG: Generating kd-tree " << a[i] << " data]" << endl;
                #endif
                stringstream dir; 
                dir << _base_dir << "/kd_tree_" << setprecision(2) << l[i];
                ifstream tree_in (dir.str());
                KDTree<Label, T> tree (tree_in, *_trn_st);
                count = 0;
                for (size_t i = 0; i < (*_tst_st).size(); i++)
                {
                    Label nn_lbl = (*_trn_st).get_label(nearest_neighbor((*_tst_st)[i],
                                                                         (*_trn_st).subset(tree.subdomain((*_tst_st)[i]))));
                    if (nn_lbl != (*_tst_st).get_label(i))
                    {
                        count++;
                    }
                }
                kd_out << a[i] << "\t\t" << (count * 1. / (*_tst_st).size()) << endl;
            }
        }
        kd_out.close();
        ofstream spill_out (out_dir + "/spill_tree_error.dat");
        for (size_t i = 0; i < a_len; i++)
        {
            for (size_t j = 0; j < l_len; j++)
            {
                #ifdef DEBUG
                cerr << "[DEBUG: Generating spill-tree " << a[i] << " " << l[j] << " data]" << endl;
                #endif
                stringstream dir; 
                dir << _base_dir << "/spill_tree_" << setprecision(2) << a[i] << "_" << l[j];
                ifstream tree_in (dir.str());
                SpillTree<Label, T> tree (tree_in, *_trn_st);
                count = 0;
                for (size_t i = 0; i < (*_tst_st).size(); i++)
                {
                    Label nn_lbl = (*_trn_st).get_label(nearest_neighbor((*_tst_st)[i],
                                                                         (*_trn_st).subset(tree.subdomain((*_tst_st)[i]))));
                    if (nn_lbl != (*_tst_st).get_label(i))
                    {
                        count++;
                    }
                }
                spill_out << a[i] << "\t\t" << l[j] << "\t\t" << (count * 1. / (*_tst_st).size()) << endl;
            }
        }
        spill_out.close();
        ofstream v_spill_out (out_dir + "/v_spill_tree_error.dat");
        for (size_t i = 0; i < a_len; i++)
        {
            for (size_t j = 0; j < l_len; j++)
            {
                #ifdef DEBUG
                cerr << "[DEBUG: Generating v_spill-tree " << a[i] << " " << l[j] << " data]" << endl;
                #endif
                stringstream dir; 
                dir << _base_dir << "/v_spill_tree_" << setprecision(2) << a[i] << "_" << l[j];
                ifstream tree_in (dir.str());
                VirtualSpillTree<Label, T> tree (tree_in, *_trn_st);
                count = 0;
                for (size_t i = 0; i < (*_tst_st).size(); i++)
                {
                    Label nn_lbl = (*_trn_st).get_label(nearest_neighbor((*_tst_st)[i],
                                                                         (*_trn_st).subset(tree.subdomain((*_tst_st)[i]))));
                    if (nn_lbl != (*_tst_st).get_label(i))
                    {
                        count++;
                    }
                }
                v_spill_out << a[i] << "\t\t" << l[j] << "\t\t" << (count * 1. / (*_tst_st).size()) << endl;
            }
        }
        v_spill_out.close();
    }
};


template<class Label, class T>
Test<Label, T>::Test(string base_dir) :
  _base_dir (base_dir)
{
    #ifdef DEBUG
    cerr << "[DEBUG: Loading datasets]" << endl;
    cerr << "[DEBUG: Loading vectors]" << endl;
    #endif
    ifstream trn_vtr_in (base_dir + "/trn_vtr");
    ifstream tst_vtr_in (base_dir + "/tst_vtr");
    _trn_st = new DataSet<Label, T>(trn_vtr_in);
    _tst_st = new DataSet<Label, T>(tst_vtr_in);
    trn_vtr_in.close();
    tst_vtr_in.close();
    #ifdef DEBUG
    cerr << "[DEBUG: Labeling dataset]" << endl;
    #endif
    ifstream trn_lbl_in (base_dir + "/trn_lbl");
    ifstream tst_lbl_in (base_dir + "/tst_lbl");
    _trn_st->label(trn_lbl_in);
    _tst_st->label(tst_lbl_in);
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
        for (size_t i = 0; i < _tst_st->size(); i++)
        {
            #ifdef DEBUG
            cerr << "[DEBUG: " << i  << " |";
            #endif
            for (int j = 0; j < k; j++)
            {
                size_t nn; 
                nn_dat_in.read((char *)&nn, sizeof(size_t));
                _nn_mp[(*_tst_st)[i]].push_back(nn);
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
        for (size_t i = 0; i < _tst_st->size(); i++)
        {
            #ifdef DEBUG
            cerr << "[DEBUG: " << i  << " |";
            #endif
            DataSet<Label, T> l_st = k_nearest_neighbor(k, (*_tst_st)[i], *_trn_st);
            for (size_t j = 0; j < k; j++)
            {

                nn_dat_out.write((char *)&(l_st.get_domain()[j]), sizeof(size_t));
                _nn_mp[(*_tst_st)[i]].push_back(l_st.get_domain()[j]);
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
    delete _trn_st;
    delete _tst_st;
}

#endif
