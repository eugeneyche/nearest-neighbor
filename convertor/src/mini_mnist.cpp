#include "mini_mnist.h"

typedef unsigned char byte;

static string base_path = "data/mini_mnist/";

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

void mini_mnist_generate()
{
    cerr << "> converting mini mnist data" << endl;
    {
        ifstream vtr_in (base_path + "in/train-images-idx3-ubyte", ios::binary);
        ifstream lbl_in (base_path + "in/train-labels-idx1-ubyte", ios::binary);
        ofstream vtr_out (base_path + "out/trn_vtr", ios::binary);
        ofstream lbl_out (base_path + "out/trn_lbl", ios::binary);
        vector<byte> lbls;
        vector<vector<byte> > vtrs;
        int mgk_vtr, mgk_lbl;
        mgk_vtr = read_bytes(vtr_in, 4);
        mgk_lbl = read_bytes(lbl_in, 4);
        cerr << "    > converting train data" << endl;
        cerr << "        > train_vtr magic number: " << mgk_vtr << endl;
        cerr << "        > train_lbl magic number: " << mgk_lbl << endl;
        size_t vtr_sz, lbl_sz;
        vtr_sz = read_bytes(vtr_in, 4);
        lbl_sz = read_bytes(lbl_in, 4);
        cerr << "        > train_vtr size: " << vtr_sz << endl;
        cerr << "        > train_lbl size: " << lbl_sz << endl;
        size_t vtr_r, vtr_c;
        vtr_r = read_bytes(vtr_in, 4);
        vtr_c = read_bytes(vtr_in, 4);
        cerr << "        > train_vtr row: " << vtr_r << endl;
        cerr << "        > train_vtr column: " << vtr_c << endl;
        size_t vtr_dim = vtr_r * vtr_c;
        cerr << "        > train_vtr dim: " << vtr_dim << endl;
        if (vtr_sz != lbl_sz)
        {
            cerr << "        > data corrupt!!!" << endl;
            vtr_in.close();
            lbl_in.close();
            vtr_out.close();
            lbl_out.close();
            return;
        }
        for (int i = 0; i < vtr_sz; i++)
        {
            byte l_lbl = read_bytes(lbl_in, 1);
            vector<byte> l_vtr;
            for (int j = 0; j < vtr_dim; j++)
            {
                byte l_pxl = read_bytes(vtr_in, 1);
                l_vtr.push_back(l_pxl);
            }
            lbls.push_back(l_lbl);
            vtrs.push_back(l_vtr);
        }
        vtr_sz = 600;
        lbl_sz = 600;
        cerr << "        > train_vtr size reduced to: " << vtr_sz << endl;
        cerr << "        > train_lbl size reduced to: " << lbl_sz << endl;
        lbl_out.write((char *)&lbl_sz, sizeof(size_t));
        lbl_out.write((char *)&lbls[0], sizeof(byte) * lbl_sz);
        vtr_out.write((char *)&vtr_sz, sizeof(size_t));
        vtr_out.write((char *)&vtr_dim, sizeof(size_t));
        for (int i = 0; i < vtr_sz; i++)
        {
            vtr_out.write((char *)&vtrs[i][0], sizeof(byte) * vtr_dim);
        }
        vtr_in.close();
        lbl_in.close();
        vtr_out.close();
        lbl_out.close();
        cerr << "    > finished train data" << endl;
    }
    {
        ifstream vtr_in (base_path + "in/t10k-images-idx3-ubyte", ios::binary);
        ifstream lbl_in (base_path + "in/t10k-labels-idx1-ubyte", ios::binary);
        ofstream vtr_out (base_path + "out/tst_vtr", ios::binary);
        ofstream lbl_out (base_path + "out/tst_lbl", ios::binary);
        vector<byte> lbls;
        vector<vector<byte> > vtrs;
        int mgk_vtr, mgk_lbl;
        mgk_vtr = read_bytes(vtr_in, 4);
        mgk_lbl = read_bytes(lbl_in, 4);
        cerr << "    > converting train data" << endl;
        cerr << "        > train_vtr magic number: " << mgk_vtr << endl;
        cerr << "        > train_lbl magic number: " << mgk_lbl << endl;
        size_t vtr_sz, lbl_sz;
        vtr_sz = read_bytes(vtr_in, 4);
        lbl_sz = read_bytes(lbl_in, 4);
        cerr << "        > train_vtr size: " << vtr_sz << endl;
        cerr << "        > train_lbl size: " << lbl_sz << endl;
        size_t vtr_r, vtr_c;
        vtr_r = read_bytes(vtr_in, 4);
        vtr_c = read_bytes(vtr_in, 4);
        cerr << "        > train_vtr row: " << vtr_r << endl;
        cerr << "        > train_vtr column: " << vtr_c << endl;
        size_t vtr_dim = vtr_r * vtr_c;
        cerr << "        > train_vtr dim: " << vtr_dim << endl;
        if (vtr_sz != lbl_sz)
        {
            cerr << "        > data corrupt!!!" << endl;
            vtr_in.close();
            lbl_in.close();
            vtr_out.close();
            lbl_out.close();
            return;
        }
        for (int i = 0; i < vtr_sz; i++)
        {
            byte l_lbl = read_bytes(lbl_in, 1);
            vector<byte> l_vtr;
            for (int j = 0; j < vtr_dim; j++)
            {
                byte l_pxl = read_bytes(vtr_in, 1);
                l_vtr.push_back(l_pxl);
            }
            lbls.push_back(l_lbl);
            vtrs.push_back(l_vtr);
        }
        vtr_sz = 100;
        lbl_sz = 100;
        cerr << "        > train_vtr size reduced to: " << vtr_sz << endl;
        cerr << "        > train_lbl size reduced to: " << lbl_sz << endl;
        lbl_out.write((char *)&lbl_sz, sizeof(size_t));
        lbl_out.write((char *)&lbls[0], sizeof(byte) * lbl_sz);
        vtr_out.write((char *)&vtr_sz, sizeof(size_t));
        vtr_out.write((char *)&vtr_dim, sizeof(size_t));
        for (int i = 0; i < vtr_sz; i++)
        {
            vtr_out.write((char *)&vtrs[i][0], sizeof(byte) * vtr_dim);
        }
        vtr_in.close();
        lbl_in.close();
        vtr_out.close();
        lbl_out.close();
        cerr << "    > finished train data" << endl;
    }
    cerr << "> finished mini mnist data" << endl;
}

