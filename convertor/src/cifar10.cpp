#include "cifar10.h"

typedef unsigned char byte;

static string base_path = "data/cifar10/";
static const int vtr_dim = 3072;
static const int train_batch_size = 5;
static string train_batches [] = 
{
    "in/cifar-10-batches-bin/data_batch_1.bin",
    "in/cifar-10-batches-bin/data_batch_2.bin",
    "in/cifar-10-batches-bin/data_batch_3.bin",
    "in/cifar-10-batches-bin/data_batch_4.bin",
    "in/cifar-10-batches-bin/data_batch_5.bin"
};
static const int test_batch_size = 1;
static string test_batches [] = 
{
    "in/cifar-10-batches-bin/test_batch.bin",
};

void cifar10_generate()
{
    cerr << "==> converting cifar10 data" << endl;    
    {
        cerr << "    ==> converting train data" << endl;
        ofstream vtr_out (base_path + "out/train_vectors", ios::binary);
        ofstream lbl_out (base_path + "out/train_labels", ios::binary);
        vector<int> lbls;
        vector<vector<double> > vtrs;
        for (int i = 0; i < train_batch_size; i++)
        {
            cerr << "        ==> converting " << train_batches[i] << endl;
            ifstream in (base_path + train_batches[i], ios::binary);
            byte l_lbl;
            while (in.read((char *)&l_lbl, sizeof(byte)))
            {
                lbls.push_back(l_lbl);
                vector<double> l_vtr;
                for (int j = 0; j < vtr_dim; j++)
                {
                    byte l_pxl;
                    in.read((char *)&l_pxl, sizeof(byte));
                    l_vtr.push_back((int)l_pxl);
                }
                vtrs.push_back(l_vtr);
            }
            in.close();
            cerr << "            ==> sizeof " << vtrs.size() << endl;
            cerr << "        ==> finished " << train_batches[i] << endl;
        }
        cerr << "    ==> train_set size: " << vtrs.size() << endl;
        lbl_out << lbls.size() << endl;
        lbl_out.write((char *)&lbls[0], sizeof(int) * lbls.size());
        for (int i = 0; i < vtrs.size(); i++)
        {
            vtr_out.write((char *)&vtrs[i][0], sizeof(double) * vtrs[i].size());
        }
        lbl_out.close();
        vtr_out.close();
        cerr << "    ==> finished train set" << endl;
        
    }
    {
        cerr << "    ==> converting test data" << endl;
        ofstream vtr_out (base_path + "out/test_vectors", ios::binary);
        ofstream lbl_out (base_path + "out/test_labels", ios::binary);
        vector<int> lbls;
        vector<vector<double> > vtrs;
        for (int i = 0; i < test_batch_size; i++)
        {
            cerr << "        ==> converting " << test_batches[i] << endl;
            ifstream in (base_path + test_batches[i], ios::binary);
            byte l_lbl;
            while (in.read((char *)&l_lbl, sizeof(byte)))
            {
                lbls.push_back(l_lbl);
                vector<double> l_vtr;
                for (int j = 0; j < vtr_dim; j++)
                {
                    byte l_pxl;
                    in.read((char *)&l_pxl, sizeof(byte));
                    l_vtr.push_back((int)l_pxl);
                }
                vtrs.push_back(l_vtr);
            }
            in.close();
            cerr << "            ==> sizeof " << vtrs.size() << endl;
            cerr << "        ==> finished " << test_batches[i] << endl;
        }
        cerr << "    ==> test_set size: " << vtrs.size() << endl;
        lbl_out << lbls.size() << endl;
        lbl_out.write((char *)&lbls[0], sizeof(int) * lbls.size());
        for (int i = 0; i < vtrs.size(); i++)
        {
            vtr_out.write((char *)&vtrs[i][0], sizeof(double) * vtrs[i].size());
        }
        lbl_out.close();
        vtr_out.close();
        cerr << "    ==> finished test set" << endl;
    }
    cerr << "==> finished cifar10 data" << endl;    
}
