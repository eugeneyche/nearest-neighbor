#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int read_bytes(ifstream & in, int n_bytes)
{
    int res = 0; unsigned char byte;
    for (int i = 0; i < n_bytes; i++)
    {
        in.read((char *)&byte, 1);
        res = (res << 8) + (int)byte;
    }
    return res;
}

int main(int argc, char * args [])
{
    if (argc != 5)
    {
        cout << "usage: " << args[0] << " " << "<labels> <vectors> <label_output> <vectors_output>" << endl;
        return 1;
    }
    ifstream label_in (args[1]);
    ifstream vtr_in (args[2]);
    ofstream label_out (args[3]);
    ofstream vtrs_out (args[4]);
    read_bytes(label_in, 4);
    read_bytes(vtr_in, 4);
    vector <int> labels;
    vector <vector <double> > vtrs;
    int size = read_bytes(label_in, 4);
    if (size != read_bytes(vtr_in, 4))
    {
        return 1;
    }
    int vtr_dim = read_bytes(vtr_in, 4) * read_bytes(vtr_in, 4);
    for (int i = 0; i < size; i++)
    {
        int label = read_bytes(label_in, 1);
        vector <double> vtr;
        for (int j = 0; j < vtr_dim; j++)
        {
            vtr.push_back(read_bytes(vtr_in, 1));
        }
        labels.push_back(label);
        vtrs.push_back(vtr);
    }
    label_out << labels.size() << endl;
    for (int i = 0; i < labels.size(); i++)
    {
        label_out << labels[i] << endl;
    }
    label_out.close();
    vtrs_out << vtrs.size() << " " << vtrs[0].size() << endl;
    for (int i = 0; i < vtrs.size(); i++)
    {
        for (int j = 0 ;j < vtrs[i].size(); j++)
        {
            if (j != 0) vtrs_out << " ";
            vtrs_out << vtrs[i][j];
        }
        vtrs_out << endl;
    }
    vtrs_out.close();
}
