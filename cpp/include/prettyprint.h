#ifndef _PRETTYPRINT_H
#define _PRETTYPRINT_H

#include "nn.h"
#include <iostream>
#include <string>
using namespace std;

/* prints a glyph from mnist data */
void print_glyph(euclid_vector * to_print);
/* prints a kd_tree */
void print_tree(kd_tree_node * m_node, int offset = 0, bool last = false);

#endif

