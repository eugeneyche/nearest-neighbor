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
void print_tree(kd_tree_node * m_node, int offset, bool last)
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
            printf("%s", (last ? "└" : "├"));
            continue;
        }
        printf("%s", (kd_tree_active[i + 1] ? "│":  " "));
    }
    printf("%s index: %d domain_size: %d pivot: %lg", 
            ((m_node->get_left() && m_node->get_right()) ? "┬" : "╴"), m_node->get_index(), 
            m_node->get_domain().size(), m_node->get_pivot());
    virtual_spill_tree_node * q_node;
    if (q_node = dynamic_cast <virtual_spill_tree_node *> (m_node))
    {
        printf(" pivot_l: %g pivot_r: %g", q_node->get_pivot_l(), q_node->get_pivot_r());
    }
    #ifdef DEBUG
    vector <int> m_domain = m_node->get_domain();
    printf(" domain:");
    for (int i = 0; i < m_domain.size() && i < 10; i++)
    {
        printf(" %d", m_domain[i]);
        if (i == 9)
            printf("...", m_domain[i]);
    }
    #endif
    printf("\n");
    if (last)
        kd_tree_active[offset] = false;
    if (m_node->get_left() && m_node->get_right())
    {
        kd_tree_active[offset + 1] = true;
        print_tree(m_node->get_left(), offset + 1, false);
        print_tree(m_node->get_right(), offset + 1, true);
    }
}


