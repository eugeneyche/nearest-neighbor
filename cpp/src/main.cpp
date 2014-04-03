#include "vector_math.h"
#include "nn.h"
#include "kd_tree.h"
#include "spill_tree.h"
#include "virtual_spill_tree.h"
#include "data_set.h"
#include "test.h"
#include <iostream>
#include <cstdio>
using namespace std;

typedef unsigned char byte;

char gradient [] = {' ', '.',':',';','+','=','x','X','$','@'};

void print_glyph(vector<byte> * to_print)
{
    for (int i = 0; i < 28 * 28; i++) {
        printf("%c", gradient[(int)((double)(*to_print)[i] / 255 * 10)]);
        if (i % 28 == 27) printf("\n");
    }
}

template<class Label, class T>
void print_tree(KDTreeNode<Label, T> * m_node, int offset = 0, bool last = false)
{
    static bool active [999]; 
    if (offset == 0)
    {
        for (int i = 0; i < 999; i++)
        {
            active[i] = false;
        }
    }
    for (int i = 0; i < offset; i++)
    {
        if (i == offset - 1)
        {
            printf("%s", (last ? "└" : "├"));
            continue;
        }
        printf("%s", (active[i + 1] ? "│":  " "));
    }
    if (m_node)
    {
        printf("%s index: %d domain_size: %d", 
                ((m_node->get_left() && m_node->get_right()) ? "┬" : "╴"), 
                m_node->get_index(), 
                m_node->get_domain().size(), m_node->get_pivot());
    }
    printf("\n");
    if (last)
        active[offset] = false;
    if (m_node->get_left() && m_node->get_right())
    {
        active[offset + 1] = true;
        print_tree(m_node->get_left(), offset + 1, false);
        print_tree(m_node->get_right(), offset + 1, true);
    }
}

int main() 
{
    Test<byte, byte> mTest ("data/mnist");
}
