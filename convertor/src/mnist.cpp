#include "mnist.h"

typedef unsigned char byte;

static string base_path = "data/mnist/";

static int read_bytes(ifstream & in, int n_bytes)
{
    int result = 0;
    byte bytes [n_bytes];
    in.read((char *)&bytes, sizeof(byte) * n_bytes);
    for (int i = 0; i < n_bytes; i++)
    {
        result = (result << 8) + (int)(bytes[i]);
    }
    return result;
}

void mnist_generate()
{
    cerr << "==> converting mnist data" << endl;
    {
        ifstream vtr_in (base_path + "in/train-images-idx3-ubyte", ios::binary);
        ifstream lbl_in (base_path + "in/train-labels-idx1-ubyte", ios::binary);
        ofstream vtr_out (base_path + "out/train_vectors", ios::binary);
        ofstream lbl_out (base_path + "out/train_labels", ios::binary);
        vector<int> lbls;
        vector<vector<double> > vtrs;
        int mgk_vtr, mgk_lbl;
        mgk_vtr = read_bytes(vtr_in, 4);
        mgk_lbl = read_bytes(lbl_in, 4);
        cerr << "    ==> converting train data" << endl;
        cerr << "        ==> train_vtr magic number: " << mgk_vtr << endl;
        cerr << "        ==> train_lbl magic number: " << mgk_lbl << endl;
        int vtr_sz, lbl_sz;
        vtr_sz = read_bytes(vtr_in, 4);
        lbl_sz = read_bytes(lbl_in, 4);
        cerr << "        ==> train_vtr size: " << vtr_sz << endl;
        cerr << "        ==> train_lbl size: " << lbl_sz << endl;
        int vtr_r, vtr_c;
        vtr_r = read_bytes(vtr_in, 4);
        vtr_c = read_bytes(vtr_in, 4);
        cerr << "        ==> train_vtr row: " << vtr_r << endl;
        cerr << "        ==> train_vtr column: " << vtr_c << endl;
        int vtr_dim = vtr_r * vtr_c;
        cerr << "        ==> train_vtr dim: " << vtr_dim << endl;
        if (vtr_sz != lbl_sz)
        {
            cerr << "        ==> data corrupt !!!" << endl;
            vtr_in.close();
            lbl_in.close();
            vtr_out.close();
            lbl_out.close();
            return;
        }
        for (int i = 0; i < vtr_sz; i++)
        {
            byte l_lbl = read_bytes(lbl_in, 1);
            vector<double> l_vtr;
            for (int j = 0; j < vtr_dim; j++)
            {
                byte l_pxl = read_bytes(vtr_in, 1);
                l_vtr.push_back(l_pxl);
            }
            lbls.push_back(l_lbl);
            vtrs.push_back(l_vtr);
        }
        lbl_out << lbls.size() << endl;
        lbl_out.write((char *)&lbls[0], sizeof(int) * lbls.size());
        vtr_out << vtrs.size() << " " << vtr_dim << endl;
        for (int i = 0; i < vtrs.size(); i++)
        {
            vtr_out.write((char *)&vtrs[i][0], sizeof(double) * vtr_dim);
        }
        vtr_in.close();
        lbl_in.close();
        vtr_out.close();
        lbl_out.close();
        cerr << "    ==> finished train data" << endl;
    }
    {
        ifstream vtr_in (base_path + "in/t10k-images-idx3-ubyte", ios::binary);
        ifstream lbl_in (base_path + "in/t10k-labels-idx1-ubyte", ios::binary);
        ofstream vtr_out (base_path + "out/test_vectors", ios::binary);
        ofstream lbl_out (base_path + "out/test_labels", ios::binary);
        vector<int> lbls;
        vector<vector<double> > vtrs;
        int mgk_vtr, mgk_lbl;
        mgk_vtr = read_bytes(vtr_in, 4);
        mgk_lbl = read_bytes(lbl_in, 4);
        cerr << "    ==> converting test data" << endl;
        cerr << "        ==> test_vtr magic number: " << mgk_vtr << endl;
        cerr << "        ==> test_lbl magic number: " << mgk_lbl << endl;
        int vtr_sz, lbl_sz;
        vtr_sz = read_bytes(vtr_in, 4);
        lbl_sz = read_bytes(lbl_in, 4);
        cerr << "        ==> test_vtr size: " << vtr_sz << endl;
        cerr << "        ==> test_lbl size: " << lbl_sz << endl;
        int vtr_r, vtr_c;
        vtr_r = read_bytes(vtr_in, 4);
       vtr_c = read_bytes(vtr_in, 4);
        cerr << "        ==> test_vtr row: " << vtr_r << endl;
        cerr << "        ==> test_vtr column: " << vtr_c << endl;
        int vtr_dim = vtr_r * vtr_c;
        cerr << "        ==> test_vtr dim: " << vtr_dim << endl;
        if (vtr_sz != lbl_sz)
        {
            cerr << "        ==> data corrupt !!!" << endl;
            vtr_in.close();
            lbl_in.close();
            vtr_out.close();
            lbl_out.close();
            return;
        }
        for (int i = 0; i < vtr_sz; i++)
        {
            byte l_lbl = read_bytes(lbl_in, 1);
            vector<double> l_vtr;
            for (int j = 0; j < vtr_dim; j++)
            {
                byte l_pxl = read_bytes(vtr_in, 1);
                l_vtr.push_back(l_pxl);
            }
            lbls.push_back(l_lbl);
            vtrs.push_back(l_vtr);
        }
        lbl_out << lbls.size() << endl;
        lbl_out.write((char *)&lbls[0], sizeof(int) * lbls.size());
        vtr_out << vtrs.size() << " " << vtr_dim << endl;
        for (int i = 0; i < vtrs.size(); i++)
        {
            vtr_out.write((char *)&vtrs[i][0], sizeof(double) * vtr_dim);
        }
        vtr_in.close();
        lbl_in.close();
        vtr_out.close();
        lbl_out.close();
        cerr << "    ==> finished test data" << endl;
    }
    cerr << "==> finished mnist data" << endl;
}
