#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>
using namespace std;

#define BYTE unsigned char
#define VECTOR_SIZE 54
#define TRAIN_SIZE 11340
#define VALIDATE_SIZE 3780
#define TEST_SIZE 565892

int main()
{
    FILE * input = fopen("data/covtype.data", "r");
    {
        FILE * vtr_out = fopen("out/train_vectors", "wb");
        FILE * label_out = fopen("out/train_labels", "wb");
        vector<int > labels;
        vector<vector<double > > vtrs;
        int label;
        int data;
        for (int i = 0; i < TRAIN_SIZE; i++)
        {
            fscanf(input, "%d,\n", &label);
            vector <double> vtr;
            for (int j = 0; j < VECTOR_SIZE; j++)
            {
                fscanf(input, "%d ,", &data);
                vtr.push_back(data);
            }
            labels.push_back(label);
            vtrs.push_back(vtr);
        }
        fprintf(label_out, "%d\n", labels.size());
        fwrite(&labels[0], sizeof(int), labels.size(), label_out); 
        fprintf(vtr_out, "%d %d\n", vtrs.size(), VECTOR_SIZE);
        for (int i = 0; i < vtrs.size(); i++)
        {
            fwrite(&vtrs[i][0], sizeof(double), VECTOR_SIZE, vtr_out); 
        }
        fclose(vtr_out);
        fclose(label_out);
    }
    {
        FILE * vtr_out = fopen("out/validate_vectors", "wb");
        FILE * label_out = fopen("out/validate_labels", "wb");
        vector<int > labels;
        vector<vector<double > > vtrs;
        int label;
        int data;
        for (int i = 0; i < VALIDATE_SIZE; i++)
        {
            fscanf(input, "%d,\n", &label);
            vector <double> vtr;
            for (int j = 0; j < VECTOR_SIZE; j++)
            {
                fscanf(input, "%d,\n", &data);
                vtr.push_back(data);
            }
            labels.push_back(label);
            vtrs.push_back(vtr);
        }
        fprintf(label_out, "%d\n", labels.size());
        fwrite(&labels[0], sizeof(int), labels.size(), label_out); 
        fprintf(vtr_out, "%d %d\n", vtrs.size(), VECTOR_SIZE);
        for (int i = 0; i < vtrs.size(); i++)
        {
            fwrite(&vtrs[i][0], sizeof(double), VECTOR_SIZE, vtr_out); 
        }
        fclose(vtr_out);
        fclose(label_out);
    }
    {
        FILE * vtr_out = fopen("out/test_vectors", "wb");
        FILE * label_out = fopen("out/test_labels", "wb");
        vector<int > labels;
        vector<vector<double > > vtrs;
        int label;
        int data;
        for (int i = 0; i < TEST_SIZE; i++)
        {
            fscanf(input, "%d,\n", &label);
            vector <double> vtr;
            for (int j = 0; j < VECTOR_SIZE; j++)
            {
                fscanf(input, "%d,\n", &data);
                vtr.push_back(data);
            }
            labels.push_back(label);
            vtrs.push_back(vtr);
        }
        fprintf(label_out, "%d\n", labels.size());
        fwrite(&labels[0], sizeof(int), labels.size(), label_out); 
        fprintf(vtr_out, "%d %d\n", vtrs.size(), VECTOR_SIZE);
        for (int i = 0; i < vtrs.size(); i++)
        {
            fwrite(&vtrs[i][0], sizeof(double), VECTOR_SIZE, vtr_out); 
        }
        fclose(vtr_out);
        fclose(label_out);
    }
    fclose(input);
}
