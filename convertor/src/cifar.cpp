#include <cstdlib>
#include <cstring>
using std::atof;
using std::strtok;

#include "convertor.h"
#include "cifar.h"

typedef unsigned char byte;

#define BASE_PATH ("data/cifar")
#define TR_LABELS_PATH ("train_labels")
#define TR_VECTORS_PATH ("train_vectors")
#define TT_LABELS_PATH ("test_labels")
#define TT_VECTORS_PATH ("test_vectors")

static const size_t TRAIN_MAX = 50000;
static const size_t TEST_MAX = 10000;
static const size_t WIDTH = 1000;

static float databuf [TRAIN_MAX][WIDTH];
static unsigned int labelbuf [TRAIN_MAX];

static char strbuf [TRAIN_MAX], * tok;
static byte res;
static int datah, dataw;
static size_t labelh;

extern 
FILE * tr_labels_out,
     * tr_vectors_out,
     * tt_labels_out,
     * tt_vectors_out;

void 
cifar_generate(void) 
{
    /* TRAIN DATA */
    char file_path [BUFSIZ];
    FILE * fin;

    sprintf(file_path, "%s/%s", BASE_PATH, TR_VECTORS_PATH);
    fin = fopen(file_path, "rb");
    datah = 0;
    while (fgets(strbuf, TRAIN_MAX, fin)) {
        tok = strtok(strbuf, ",");
        dataw = 0;
        while (tok) {
            databuf[datah][dataw] = atof(tok);
            tok = strtok(NULL, ",");
            dataw++;
        }
        datah++;
    }
    fclose(fin);

    sprintf(file_path, "%s/%s", BASE_PATH, TR_LABELS_PATH);
    fin = fopen(file_path, "rb");
    labelh = 0;
    while (fscanf(fin, "%d\n", &labelbuf[labelh]) != EOF) {
        labelh++;
    }
    fclose(fin);
    
    fwrite(&TRAIN_MAX, sizeof(size_t), 1, tr_vectors_out);
    fwrite(&WIDTH, sizeof(size_t), 1, tr_vectors_out);
    
    fwrite(&TRAIN_MAX, sizeof(size_t), 1, tr_labels_out);
    
    for (int i = 0; i < TRAIN_MAX; i++) {
        res = (byte)labelbuf[i];
        fwrite(&databuf[i], sizeof(float), WIDTH, tr_vectors_out);
        fwrite(&res, sizeof(byte), 1, tr_labels_out); 
    }
    
    /* TEST DATA */

    sprintf(file_path, "%s/%s", BASE_PATH, TT_VECTORS_PATH);
    fin = fopen(file_path, "rb");
    datah = 0;
    while (fgets(strbuf, TEST_MAX, fin)) {
        tok = strtok(strbuf, ",");
        dataw = 0;
        while (tok) {
            databuf[datah][dataw] = atof(tok);
            tok = strtok(NULL, ",");
            dataw++;
        }
        datah++;
    }
    fclose(fin);

    sprintf(file_path, "%s/%s", BASE_PATH, TT_LABELS_PATH);
    fin = fopen(file_path, "rb");
    labelh = 0;
    while (fscanf(fin, "%d\n", &labelbuf[labelh]) != EOF) {
        labelh++;
    }
    fclose(fin);
    
    fwrite(&TEST_MAX, sizeof(size_t), 1, tt_vectors_out);
    fwrite(&WIDTH, sizeof(size_t), 1, tt_vectors_out);
    
    fwrite(&TEST_MAX, sizeof(size_t), 1, tt_labels_out);
    
    for (int i = 0; i < TEST_MAX; i++) {
        res = (byte)labelbuf[i];
        fwrite(&databuf[i], sizeof(float), WIDTH, tt_vectors_out);
        fwrite(&res, sizeof(byte), 1, tt_labels_out); 
    }
}
