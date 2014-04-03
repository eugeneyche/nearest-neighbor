#ifndef _TEST_H
#define _TEST_H

#include "data_set.h"
#include <map>
#include <fstream>
#include <sstream>

template<class Label, class T>
class Test
{
protected:
    DataSet<Label, T> * _trn_st;
    DataSet<Label, T> * _tst_st;
    map<vector<T> *, vector<vector<T> *>> _nn_mp;
public:
    Test(string base_dir);
    ~Test();
};

template<class Label, class T>
Test<Label, T>::Test(string base_dir)
{
    cerr << "> loading datasets" << endl;
    cerr << "    > loading vectors" << endl;
    ifstream trn_vtr_in (base_dir + "/trn_vtr");
    ifstream tst_vtr_in (base_dir + "/tst_vtr");
    _trn_st = new DataSet<Label, T>(trn_vtr_in);
    _tst_st = new DataSet<Label, T>(tst_vtr_in);
    trn_vtr_in.close();
    tst_vtr_in.close();
    cerr << "    > labeling dataset" << endl;
    ifstream trn_lbl_in (base_dir + "/trn_lbl");
    ifstream tst_lbl_in (base_dir + "/tst_lbl");
    _trn_st->label(trn_lbl_in);
    _tst_st->label(tst_lbl_in);
    trn_lbl_in.close();
    tst_lbl_in.close();
    cerr << "    > SUCCESS" << endl;
    cerr << "> looking for k_true_nn" << endl;
    ifstream nn_dat_in (base_dir + "/k_true_nn", ios::binary);
    if (nn_dat_in.good())
    {
        cerr << "    > k_true_nn found!!!" << endl;
        cerr << "    > parsing k_true_nn" << endl;
        size_t k;
        cerr << "        > k: " << k <<  endl;
        nn_dat_in.read((char *)&k, sizeof(size_t));
        for (size_t i = 0; i < _tst_st->size(); i++)
        {
            cerr << "        - vtr  " << i  << ":";
            for (int j = 0; j < k; j++)
            {
                size_t nn; 
                nn_dat_in.read((char *)&nn, sizeof(size_t));
                _nn_mp[(*_tst_st)[i]].push_back((*_trn_st)[nn]);
                cerr << " " << nn;
            }
            cerr << endl;
        }
        nn_dat_in.close();
        cerr << "    > SUCCESS" << endl;
    }
    else
    {
        cerr << "    > k_true_nn not found!!!" << endl;
        size_t k = 10;
        nn_dat_in.close();
        cerr << "    > generating k_true_nn" << endl;
        ofstream nn_dat_out (base_dir + "/k_true_nn", ios::binary);
        nn_dat_out.write((char *)&k, sizeof(size_t));
        for (size_t i = 0; i < _tst_st->size(); i++)
        {
            cerr << "        - vtr  " << i  << ":";
            DataSet<Label, T> l_st = k_nearest_neighbor<T, T>(k, (*_tst_st)[i], *_trn_st);
            for (size_t j = 0; j < k; j++)
            {

                nn_dat_out.write((char *)&(l_st.get_domain()[j]), sizeof(size_t));
                _nn_mp[(*_tst_st)[i]].push_back(l_st[j]);
                cerr << " " << l_st.get_domain()[j];
            }
            cerr << endl;
        }
        nn_dat_out.close();
    }
    cerr << "    > SUCCESS" << endl;
}

template<class Label, class T>
Test<Label, T>::~Test()
{
    delete _trn_st;
    delete _tst_st;
}

#endif
