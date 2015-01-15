#include <vector>
using std::vector;

#include "convertor.h"
#include "mnist.h"

#define BASE_PATH ("data/mnist")
#define TR_LABELS_PATH ("train-labels-idx1-ubyte")
#define TR_VECTORS_PATH ("train-images-idx3-ubyte")
#define TT_LABELS_PATH ("t10k-labels-idx1-ubyte")
#define TT_VECTORS_PATH ("t10k-images-idx3-ubyte")

extern 
FILE * tr_labels_out,
     * tr_vectors_out,
     * tt_labels_out,
     * tt_vectors_out;

void 
mnist_generate(void)
{
    fprintf(stderr, "Generating MNIST Data\n");

    FILE * vtr_in,
         * lbl_in;
    char file_path [BUFSIZ];
    int mgk_vtr, mgk_lbl;
    size_t vtr_sz, lbl_sz,
           vtr_r, vtr_c,
           vtr_dim;

    vector<byte> lbls;
    vector<vector<byte> > vtrs;

    /* TRAIN_DATA */
    fprintf(stderr, "Train Data Set\n");

    sprintf(file_path, "%s/%s", BASE_PATH, TR_VECTORS_PATH);
    vtr_in = fopen(file_path, "rb");
    sprintf(file_path, "%s/%s", BASE_PATH, TR_LABELS_PATH);
    lbl_in = fopen(file_path, "rb");

    lbls.clear();
    vtrs.clear();

    mgk_vtr = read_bytes(vtr_in, 4);
    mgk_lbl = read_bytes(lbl_in, 4);

    fprintf(stderr, "Magic Numbers: %d %d\n", mgk_vtr, mgk_lbl);

    vtr_sz = read_bytes(vtr_in, 4);
    lbl_sz = read_bytes(lbl_in, 4);

    fprintf(stderr, "Sizes: %d %d\n", vtr_sz, lbl_sz);

    vtr_r = read_bytes(vtr_in, 4);
    vtr_c = read_bytes(vtr_in, 4);

    fprintf(stderr, "Vector R, C: %d %d\n", vtr_r, vtr_c);

    vtr_dim = vtr_r * vtr_c;
    
    fprintf(stderr, "Reading in data...\n");
    for (int i = 0; i < vtr_sz; i++) {
        byte l_lbl = read_bytes(lbl_in, 1);
        vector<byte> l_vtr;
        for (int j = 0; j < vtr_dim; j++) {
            byte l_pxl = read_bytes(vtr_in, 1);
            l_vtr.push_back(l_pxl);
        }
        lbls.push_back(l_lbl);
        vtrs.push_back(l_vtr);
    }
    fprintf(stderr, "DONE!\n");

    fclose(vtr_in);
    fclose(lbl_in);

    fprintf(stderr, "Writing out data...\n");
    fwrite(&lbl_sz, sizeof(size_t), 1, tr_labels_out);
    fwrite(&lbls[0], sizeof(byte), lbls.size(), tr_labels_out);

    fwrite(&vtr_sz, sizeof(size_t), 1,  tr_vectors_out);
    fwrite(&vtr_dim, sizeof(size_t), 1,  tr_vectors_out);
    for (int i = 0; i < vtrs.size(); i++) {
        fwrite(&vtrs[i][0], sizeof(byte), vtr_dim, tr_vectors_out);
    }
    fprintf(stderr, "DONE!\n");

    /* TEST DATA */
    fprintf(stderr, "Test Data Set\n");

    sprintf(file_path, "%s/%s", BASE_PATH, TT_VECTORS_PATH);
    vtr_in = fopen(file_path, "rb");
    sprintf(file_path, "%s/%s", BASE_PATH, TT_LABELS_PATH);
    lbl_in = fopen(file_path, "rb");

    lbls.clear();
    vtrs.clear();

    mgk_vtr = read_bytes(vtr_in, 4);
    mgk_lbl = read_bytes(lbl_in, 4);

    fprintf(stderr, "Magic Numbers: %d %d\n", mgk_vtr, mgk_lbl);

    vtr_sz = read_bytes(vtr_in, 4);
    lbl_sz = read_bytes(lbl_in, 4);

    fprintf(stderr, "Sizes: %d %d\n", vtr_sz, lbl_sz);

    vtr_r = read_bytes(vtr_in, 4);
    vtr_c = read_bytes(vtr_in, 4);

    fprintf(stderr, "Vector R, C: %d %d\n", vtr_r, vtr_c);

    vtr_dim = vtr_r * vtr_c;

    fprintf(stderr, "Reading in data...\n");
    for (int i = 0; i < vtr_sz; i++) {
        byte l_lbl = read_bytes(lbl_in, 1);
        vector<byte> l_vtr;
        for (int j = 0; j < vtr_dim; j++) {
            byte l_pxl = read_bytes(vtr_in, 1);
            l_vtr.push_back(l_pxl);
        }
        lbls.push_back(l_lbl);
        vtrs.push_back(l_vtr);
    }
    fprintf(stderr, "DONE!\n");

    fclose(vtr_in);
    fclose(lbl_in);

    fprintf(stderr, "Writing out data...\n");
    fwrite(&lbl_sz, sizeof(size_t), 1, tt_labels_out);
    fwrite(&lbls[0], sizeof(byte), lbls.size(), tt_labels_out);

    fwrite(&vtr_sz, sizeof(size_t), 1,  tt_vectors_out);
    fwrite(&vtr_dim, sizeof(size_t), 1,  tt_vectors_out);
    for (int i = 0; i < vtrs.size(); i++) {
        fwrite(&vtrs[i][0], sizeof(byte), vtr_dim, tt_vectors_out);
    }
    fprintf(stderr, "DONE!\n");
}

