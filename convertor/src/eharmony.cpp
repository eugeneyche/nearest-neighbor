#include "eharmony.h"

typedef unsigned char byte;

static const char BASE_PATH [] = "data/eharmony";
static const char SLC1_DATA_PATH [] = "/in/EH-slice1-data.csv";
static const char SLC1_MIX_PATH []  = "/in/EH-slice1-labels.csv";
static const char SLC2_DATA_PATH [] = "/in/EH-slice2-data.csv";
static const char SLC2_MIX_PATH []  = "/in/EH-slice2-labels.csv";

static const char TRN_VTR_PATH [] = "/out/trn_vtr";
static const char TRN_LBL_PATH [] = "/out/trn_lbl";
static const char TST_VTR_PATH [] = "/out/tst_vtr";
static const char TST_LBL_PATH [] = "/out/tst_lbl";

static const size_t S1_MAX = 100000;
static const size_t S2_MAX = 10000;

static float databuf [300000][100];
static unsigned int mixbuf [600000][3];
static char strbuf [BUFSIZ], * tok;
static size_t pw, vtrw, mixh;
static int dataw, datah;
static byte res;

void eharmony_generate() {
    /* SLICE 1 */
    char filepath [BUFSIZ], * relpath;
    FILE * fin, * fout1, * fout2;
    strcpy(filepath, BASE_PATH);
    relpath = filepath + strlen(filepath);
    fprintf(stderr, "> converting eharmony data\n");
    fprintf(stderr, "  > populating data buffer for slice 1\n");
    strcpy(relpath, SLC1_DATA_PATH);
    fin = fopen(filepath, "rb");
    
    //height is the number of vectors
    datah = 0;
    while (fgets(strbuf, BUFSIZ, fin)) {
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
    fprintf(stderr, "  > dimensions w: %d, h: %d\n", dataw, datah);
    /* TODO: Normalizing */
    strcpy(relpath, SLC1_MIX_PATH);
    fin = fopen(filepath, "rb");
    strcpy(relpath, TRN_VTR_PATH);
    fout1 = fopen(filepath, "wb");
    strcpy(relpath, TRN_LBL_PATH);
    fout2 = fopen(filepath, "wb");
    fprintf(stderr, "  > generating pairwise data with labels\n");
    mixh = 0;
    while (fscanf(fin, "%d,%d,%d\n", &mixbuf[mixh][0], &mixbuf[mixh][1], 
            &mixbuf[mixh][2]) != EOF) {
        fprintf(stderr, "    > %ld:    %d,%d,%d\n", mixh, mixbuf[mixh][0],mixbuf[mixh][1], 
                mixbuf[mixh][2]);
        mixh++;
    }
    if (mixh > S1_MAX)
        mixh = S1_MAX;
    pw = dataw - 1;
    vtrw = 2 * pw;
    fprintf(stderr, "  > mix: %ld, %ld\n", vtrw, mixh);
    
    //height and width at the beginning of the vector data
    fwrite((const char *)&mixh, sizeof(size_t), 1, fout1);
    fwrite((const char *)&vtrw, sizeof(size_t), 1, fout1);
    
    //only height at the begining of the label data
    fwrite((const char *)&mixh, sizeof(size_t), 1, fout2);
    
    for (int i = 0; i < mixh; i++) {
        res = (byte)mixbuf[i][0];
        fwrite((const char *)&res, sizeof(byte), 1, fout2); //write in label data
        fwrite((const char *)&databuf[mixbuf[i][1]][1], sizeof(float), pw, fout1);
        fwrite((const char *)&databuf[mixbuf[i][2]][1], sizeof(float), pw, fout1);
    }
    fclose(fin);
    fclose(fout1);
    fclose(fout2);

    /* SLICE 2 */
    fprintf(stderr, "> converting eharmony data\n");
    fprintf(stderr, "  > populating data bu2fer for slice 2\n");
    strcpy(relpath, SLC2_DATA_PATH);
    fin = fopen(filepath, "rb");
    datah = 0;
    while (fgets(strbuf, BUFSIZ, fin)) {
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
    fprintf(stderr, "  > dimensions w: %d, h: %d\n", dataw, datah);
    /* TODO: Normalizing */
    strcpy(relpath, SLC2_MIX_PATH);
    fin = fopen(filepath, "rb");
    strcpy(relpath, TST_VTR_PATH);
    fout1 = fopen(filepath, "wb");
    strcpy(relpath, TST_LBL_PATH);
    fout2 = fopen(filepath, "wb");
    fprintf(stderr, "  > generating pairwise data with labels\n");
    mixh = 0;
    while (fscanf(fin, "%d,%d,%d\n", &mixbuf[mixh][0], &mixbuf[mixh][1], 
            &mixbuf[mixh][2]) != EOF) {
        fprintf(stderr, "    > %ld:    %d,%d,%d\n", mixh, mixbuf[mixh][0],mixbuf[mixh][1], 
                mixbuf[mixh][2]);
        mixh++;
    }
    if (mixh > S2_MAX)
        mixh = S2_MAX;
    pw = dataw - 1;
    vtrw = 2 * pw;
    fprintf(stderr, "  > mix: %ld, %ld\n", vtrw, mixh);
    fwrite((const char *)&mixh, sizeof(size_t), 1, fout1);
    fwrite((const char *)&mixh, sizeof(size_t), 1, fout2);
    fwrite((const char *)&vtrw, sizeof(size_t), 1, fout1);
    for (int i = 0; i < mixh; i++) {
        res = (byte)mixbuf[i][0];
        fwrite((const char *)&res, sizeof(byte), 1, fout2);
        fwrite((const char *)&databuf[mixbuf[i][1]][1], sizeof(float), pw, fout1);
        fwrite((const char *)&databuf[mixbuf[i][2]][1], sizeof(float), pw, fout1);
    }
    fclose(fin);
    fclose(fout1);
    fclose(fout2);
}
