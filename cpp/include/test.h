#ifndef _TEST_H
#define _TEST_H

#include "data_set.h"
#include "kd_tree.h"
#include "spill_tree.h"
#include "virtual_spill_tree.h"
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>

static double a [] = {0.05, 0.075, 0.1, 0.15, 0.2, 0.25};
static size_t a_len = 6;

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
        cerr << "> generating trees" << endl;
        #endif
        {
            #ifdef DEBUG
            cerr << "    > generating kd-tree" << endl;
            #endif
            KDTree<Label, T> tree ((size_t)(0.05 * (*_trn_st).size()), *_trn_st);
            ofstream tree_out (_base_dir + "/kd_tree");
            tree.save(tree_out);
            tree_out.close();
        }
        for (size_t i = 0; i < a_len; i++)
        {
            #ifdef DEBUG
            cerr << "    > generating spill-tree " << a[i] << endl;
            #endif
            SpillTree<Label, T> tree ((size_t)(0.05 * (*_trn_st).size()), a[i], *_trn_st);
            ofstream tree_out (_base_dir + "/spill_tree_" + to_string(a[i]));
            tree.save(tree_out);
            tree_out.close();
        }
        for (size_t i = 0; i < a_len; i++)
        {
            #ifdef DEBUG
            cerr << "    > generating virual-spill-tree " << a[i] << endl;
            #endif
            VirtualSpillTree<Label, T> tree ((size_t)(0.05 * (*_trn_st).size()), a[i], *_trn_st);
            ofstream tree_out (_base_dir + "/v_spill_tree_" + to_string(a[i]));
            tree.save(tree_out);
            tree_out.close();
        }
    }

    void generate_error_data()
    {
        #ifdef DEBUG
        cerr << "> generating error data" << endl;
        #endif
        long count = 0;
        {
            #ifdef DEBUG
            cerr << "    > generating kd-tree data" << endl;
            #endif
            ifstream tree_in (_base_dir + "/kd_tree");
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
            cout << (count * 1. / (*_tst_st).size()) << endl;
        }

    }
};


template<class Label, class T>
Test<Label, T>::Test(string base_dir) :
  _base_dir (base_dir)
{
    #ifdef DEBUG
    cerr << "> loading datasets" << endl;
    cerr << "    > loading vectors" << endl;
    #endif
    ifstream trn_vtr_in (base_dir + "/trn_vtr");
    ifstream tst_vtr_in (base_dir + "/tst_vtr");
    _trn_st = new DataSet<Label, T>(trn_vtr_in);
    _tst_st = new DataSet<Label, T>(tst_vtr_in);
    trn_vtr_in.close();
    tst_vtr_in.close();
    #ifdef DEBUG
    cerr << "    > labeling dataset" << endl;
    #endif
    ifstream trn_lbl_in (base_dir + "/trn_lbl");
    ifstream tst_lbl_in (base_dir + "/tst_lbl");
    _trn_st->label(trn_lbl_in);
    _tst_st->label(tst_lbl_in);
    trn_lbl_in.close();
    tst_lbl_in.close();
    #ifdef DEBUG
    cerr << "    > SUCCESS" << endl;
    cerr << "> looking for k_true_nn" << endl;
    #endif
    ifstream nn_dat_in (base_dir + "/k_true_nn", ios::binary);
    if (nn_dat_in.good())
    {
        #ifdef DEBUG
        cerr << "    > k_true_nn found!!!" << endl;
        cerr << "    > parsing k_true_nn" << endl;
        #endif
        size_t k;
        #ifdef DEBUG
        cerr << "            > k: " << k <<  endl;
        #endif
        nn_dat_in.read((char *)&k, sizeof(size_t));
        for (size_t i = 0; i < _tst_st->size(); i++)
        {
            #ifdef DEBUG
            cerr << "        - [ " << i  << ":";
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
            cerr << endl;
            #endif
        }
        nn_dat_in.close();
        #ifdef DEBUG
        cerr << "    > SUCCESS" << endl;
        #endif
    }
    else
    {
        #ifdef DEBUG
        cerr << "    > k_true_nn not found!!!" << endl;
        #endif
        size_t k = 10;
        nn_dat_in.close();
        #ifdef DEBUG
        cerr << "    > generating k_true_nn" << endl;
        cerr << "    > k: " << k << endl;
        #endif
        ofstream nn_dat_out (base_dir + "/k_true_nn", ios::binary);
        nn_dat_out.write((char *)&k, sizeof(size_t));
        for (size_t i = 0; i < _tst_st->size(); i++)
        {
            #ifdef DEBUG
            cerr << "        - [ " << i  << ":";
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
            cerr << endl;
            #endif
        }
        nn_dat_out.close();
        #ifdef DEBUG
        cerr << "    > SUCCESS" << endl;
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
