#include <cstring>
#include <cstdlib>
#include <vector>
using namespace std;

#include "convertor.h"
#include "mnist.h"
#include "cifar.h"

#define USAGE (\
  "Usage:\n"   \
  "   %s [mnist | cifar10 | cifar100] [dest]\n" \
)

#define TR_LABELS_PATH ("tr_labels")
#define TR_VECTORS_PATH ("tr_vectors")
#define TT_LABELS_PATH ("tt_labels")
#define TT_VECTORS_PATH ("tt_vectors")


FILE * tr_labels_out,
     * tr_vectors_out,
     * tt_labels_out,
     * tt_vectors_out;

int
read_bytes(FILE * in, int n_bytes)
{
    static byte bytes [MAX_BYTES];
    int result = 0;
    fread(bytes, sizeof(byte), n_bytes, in);
    for (int i = 0; i < n_bytes; i++) {
        result = (result << 8) + bytes[i];
    }
    return result;
}

int 
main(int argc, char ** argv)
{
    if (argc == 1) {
        printf(USAGE, argv[0]);
        exit(1);
    }
    if (argc > 2) {
        char file_path [BUFSIZ];
        sprintf(file_path, "%s/%s", argv[2], TR_LABELS_PATH);
        tr_labels_out = fopen(file_path, "wb");
        sprintf(file_path, "%s/%s", argv[2], TR_VECTORS_PATH);
        tr_vectors_out = fopen(file_path, "wb");
        sprintf(file_path, "%s/%s", argv[2], TT_LABELS_PATH);
        tt_labels_out = fopen(file_path, "wb");
        sprintf(file_path, "%s/%s", argv[2], TT_VECTORS_PATH);
        tt_vectors_out = fopen(file_path, "wb");
    } else {
        tr_labels_out = fopen(TR_LABELS_PATH, "wb");
        tr_vectors_out = fopen(TR_VECTORS_PATH, "wb");
        tt_labels_out = fopen(TT_LABELS_PATH, "wb");
        tt_vectors_out = fopen(TT_VECTORS_PATH, "wb");
    }
    if (strcmp(argv[1], "mnist") == 0) {
       mnist_generate();
    }
    if (strcmp(argv[1], "cifar") == 0) {
        cifar_generate();
    }
    fclose(tr_labels_out);
    fclose(tr_vectors_out);
    fclose(tt_labels_out);
    fclose(tt_vectors_out);
}
