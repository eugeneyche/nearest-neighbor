#include "prettyprint.h"

char gradient [] = {' ', '.',':',';','+','=','x','X','$','@'};

void print_glyph(euclid_vector * to_print)
{
    for (int i = 0; i < 28 * 28; i++) {
        printf("%c", gradient[(int)((*to_print)[i] / 255 * 10)]);
        if (i % 28 == 27) printf("\n");
    }
}

bool kd_tree_active [999];

void print_kd_tree(kd_tree_node * m_node, int offset, bool last)
{
    if (offset == 0)
    {
        for (int i = 0; i < 999; i++)
        {
            kd_tree_active[i] = false;
        }
    }
    for (int i = 0; i < offset; i++)
    {
        if (i == offset - 1)
        {
            cout << (last ? "└" : "├");
            continue;
        }
            cout << (kd_tree_active[i + 1] ? "│":  " ");
    }
    
    cout << ((m_node->get_left() && m_node->get_right()) ? "┬" : "╴");
    cout << " index: " << m_node->get_index() << " domain: " << m_node->get_domain().size() << endl;
    if (last)
        kd_tree_active[offset] = false;
    if (m_node->get_left() && m_node->get_right())
    {
        kd_tree_active[offset + 1] = true;
        print_kd_tree(m_node->get_left(), offset + 1, false);
        print_kd_tree(m_node->get_right(), offset + 1, true);
    }
}


