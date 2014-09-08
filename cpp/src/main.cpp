#include <iostream>
#include <cstdio>
#include <unistd.h>
#include "vector_math.h"
#include "nn.h"
#include "kd_tree.h"
#include "kd_spill_tree.h"
#include "kd_virtual_spill_tree.h"
#include "data_set.h"
#include "test.h"
using namespace std;

typedef unsigned char byte;

#ifndef LABEL_T
#define LABEL_T byte
#endif

#ifndef DATA_T
#define DATA_T byte
#endif

int 
main(int argc, char* argv [])
{
    Test<LABEL_T, DATA_T> thetest (".");
    bool gen = false;
    int opt;
    while ((opt = getopt(argc, argv, "g")) != -1) {
        switch (opt) {
        case 'g':
            gen = true;
            break;
        }
    }
    string type = argv[optind++];
    if (type == "kdt") {
        double ll = atof(argv[optind++]);
        if (gen) { 
            string path = argv[optind++];
            thetest.kd_tree(ll, path);
        } else {
            string path = argv[optind++];
            Result result;
            thetest.kd_tree_data(ll, path, &result);
            printf("%lf %lf %lf\n", result.error, result.true_nn, result.subdomain);
        }
    } else if (type == "kdst") {
        double ll = atof(argv[optind++]);
        if (gen) {
            double la = atof(argv[optind++]);
            string path = argv[optind++];
            thetest.kd_spill_tree(ll, la, path);
        } else {
            double la = atof(argv[optind++]);
            string path = argv[optind++];
            Result result;
            thetest.kd_spill_tree_data(ll, path, &result);
            printf("%lf %lf %lf\n", result.error, result.true_nn, result.subdomain);
        }
    } else if (type == "kdvst") {
        double ll = atof(argv[optind++]);
        if (gen) {
            double la = atof(argv[optind++]);
            string path = argv[optind++];
            thetest.kd_v_spill_tree(ll, la, path);
        } else {
            string path = argv[optind++];
            Result result;
            thetest.kd_v_spill_tree_data(ll, path, &result);
            printf("%lf %lf %lf\n", result.error, result.true_nn, result.subdomain);
        }
    } else if (type == "bspt") {
        double ll = atof(argv[optind++]);
        if (gen) {
            string path = argv[optind++];
            thetest.bsp_tree(ll, path);
        } else {
            string path = argv[optind++];
            Result result;
            thetest.bsp_tree_data(ll, path, &result);
            printf("%lf %lf %lf\n", result.error, result.true_nn, result.subdomain);
        }
    } else if (type == "bspst") {
        double ll = atof(argv[optind++]);
        if (gen) {
            double la = atof(argv[optind++]);
            string path = argv[optind++];
            thetest.bsp_spill_tree(ll, la, path);
        } else {
            string path = argv[optind++];
            Result result;
            thetest.bsp_spill_tree_data(ll, path, &result);
            printf("%lf %lf %lf\n", result.error, result.true_nn, result.subdomain);
        }
    }
}
