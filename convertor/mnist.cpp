#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>
using namespace std;

#define BYTE unsigned char

int read_bytes(FILE * in, int n_bytes)
{
    int res = 0; 
    BYTE bytes [n_bytes];
    fread(bytes, 1, n_bytes, in);
    for (int i = 0; i < n_bytes; i++)
    {
        res = (res << 8) + (int)(bytes[i]);
    }
    return res;
}



int main()
{
    {
        FILE * vtr_in = fopen("data/train_vectors", "rb");
        FILE * label_in = fopen("data/train_labels", "rb");
        FILE * vtr_out = fopen("out/train_vectors", "w+");
        FILE * label_out = fopen("out/train_labels", "w+");
        vector <int> labels;
        vector <vector <double> > vtrs;
        read_bytes(label_in, 4);
        read_bytes(vtr_in, 4);
        int size = read_bytes(label_in, 4);
        if (size != read_bytes(vtr_in, 4))
        {
            return -1;
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
        fprintf(label_out, "%d\n", labels.size());
        fwrite(&labels[0], sizeof(int), labels.size(), label_out); 
        fprintf(vtr_out, "%d %d\n", vtrs.size(), vtr_dim);
        for (int i = 0; i < vtrs.size(); i++)
        {
            fwrite(&vtrs[i][0], sizeof(double), vtr_dim, vtr_out); 
        }
        fclose(vtr_in);
        fclose(label_in);
        fclose(vtr_out);
        fclose(label_out);
    }
    {
        FILE * vtr_in = fopen("data/test_vectors", "r");
        FILE * label_in = fopen("data/test_labels", "r");
        FILE * vtr_out = fopen("out/test_vectors", "w+");
        FILE * label_out = fopen("out/test_labels", "w+");
        vector <int> labels;
        vector <vector <double> > vtrs;
        read_bytes(label_in, 4);
        read_bytes(vtr_in, 4);
        int size = read_bytes(label_in, 4);
        if (size != read_bytes(vtr_in, 4))
        {
            return -1;
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
        fprintf(label_out, "%d\n", labels.size());
        fwrite(&labels[0], sizeof(int), labels.size(), label_out); 
        fprintf(vtr_out, "%d %d\n", vtrs.size(), vtr_dim);
        for (int i = 0; i < vtrs.size(); i++)
        {
            fwrite(&vtrs[i][0], sizeof(double), vtr_dim, vtr_out); 
        }
        fclose(vtr_in);
        fclose(label_in);
        fclose(vtr_out);
        fclose(label_out);
    }
}
