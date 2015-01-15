#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

#define USAGE ( \
"Usage:\n"      \
"   %s [vector | label] [info | dump] file\n" \
)

#define UNDEF  (0)
#define VECTOR (1)
#define LABEL  (2)
#define INFO   (3)
#define DUMP   (4)

#define MAX_BYTES (16)

typedef unsigned char byte;

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
main(int argc, char * argv [])
{
    if (argc != 4) {
        printf(USAGE, argv[0]);
        exit(1);
    }
    int type, action;
    type = action = UNDEF;
    if (strcmp(argv[1], "vector") == 0)
        type = VECTOR;
    if (strcmp(argv[1], "label") == 0)
        type = LABEL;
    if (strcmp(argv[2], "info") == 0)
        action = INFO;
    if (strcmp(argv[2], "dump") == 0)
        action = DUMP;
    if (!type || !action) {
        printf(USAGE, argv[0]);
        exit(1);
    }
    if (type == VECTOR) {
        FILE * vtr_in = fopen(argv[3], "rb");
        size_t n_vtr, dim, f_pos, f_end, elem;
        fread(&n_vtr, sizeof(n_vtr), 1, vtr_in);
        fread(&dim, sizeof(dim), 1, vtr_in);
        f_pos = ftell(vtr_in);
        fseek(vtr_in, 0, SEEK_END);
        f_end = ftell(vtr_in);
        fseek(vtr_in, f_pos, SEEK_SET);
        elem = (f_end - f_pos) / (n_vtr * dim);
        if (action == INFO) {
            printf("Number of Vectors: %d\n", n_vtr);
            printf("Dimension of Vector: %d\n", dim);
            printf("Size of Vector Element: %d\n", elem);
        }
        if (action == DUMP) {
            for (int i = 0; i < n_vtr; i++) {
                for (int j = 0; j < dim; j++) {
                    if (j) printf(",");
                    printf("%d", read_bytes(vtr_in, elem));
                }
                printf("\n");
            }
        }
    }
    if (type == LABEL) {
        FILE * lbl_in = fopen(argv[3], "rb");
        size_t n_lbl, f_pos, f_end, elem;
        fread(&n_lbl, sizeof(n_lbl), 1, lbl_in);
        f_pos = ftell(lbl_in);
        fseek(lbl_in, 0, SEEK_END);
        f_end = ftell(lbl_in);
        fseek(lbl_in, f_pos, SEEK_SET);
        elem = (f_end - f_pos) / (n_lbl);
        if (action == INFO) {
            printf("Number of Labels: %d\n", n_lbl);
            printf("Size of Label Element: %d\n", elem);
        }
        if (action == DUMP) {
            for (int i = 0; i < n_lbl; i++) {
                printf("%d", read_bytes(lbl_in, elem));
                printf("\n");
            }
        }
    }
}
