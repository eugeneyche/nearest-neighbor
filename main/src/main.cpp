#include <cstdio>
using namespace std;

#include "dataset.h"
#include "tree.h"
#include "nn.h"

typedef unsigned char byte;

void
draw(const DataSet<byte>::Vector &v) 
{
    for (int y = 0; y < 28; y++) {
        for (int x = 0; x < 28; x++) {
            printf("%c", v[28 * y + x] > 100 ? '#' : ' ');
        }
        printf("\n");
    }
}


double my_spill = 0.2;

int 
main(void) 
{
    FILE *in = fopen("tmp/tt_vectors", "rb");
    DataSet<byte> data (in);
    fclose(in);

    const DataSet<byte>::Vector &v = data[10];

    /* Module 1
    printf("Show All\n");
    for (auto it = data.begin(); it != data.end(); it++) {
        draw(*it);
    }
    */

    /* Modules 2
    printf("\nNearest Neighbor Test\n");
    const DataSet<byte>::Vector &nn_v = nearest_neighbor(v, data);
    draw(v);
    printf("vs.\n");
    draw(nn_v);
    */

    /* Module 3 
    printf("\nk-Nearest Neighbor Test\n");
    DataSet<byte> k_nn = k_nearest_neighbor(v, data, 10);
    draw(v);
    printf("vs.\n");
    for (auto it = k_nn.begin(); it != k_nn.end(); it++) {
        draw(*it);
    }
    */

    /* Module 4
    DataSet<byte> subset = data.subset({0,1,2,3,4,5,6,7,8,9,10});
    Tree<byte, KD<byte>> tree (subset);
    tree.show();
    FILE *out = fopen("tmp/kd_tree", "wb");
    tree.save(out);
    fclose(out);
    in = fopen("tmp/kd_tree", "rb");
    Tree<byte, KD<byte>> dupl (subset, in);
    fclose(in);
    dupl.show();
    */

    /* Module 5 */
    DataSet<byte> subset = data.subset({0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20});
    Tree<byte, Spill<byte, my_spill>> tree (subset);
    tree.show();
    tree.find(v);
}
