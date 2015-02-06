/* 
 * File     : tree.h
 * Date     : 14-1-2015
 * Summary  : Contains the data structure definition
 *            of a tree structure
 */
#ifndef TREE_H
#define TREE_H

#include <cstdio>

#include <stack>
#include <queue>
#include <map>
#include <set>
#include <utility>
using namespace std;

#include "logging.h"
#include "dataset.h"
#include "part.h"

template <class T, class P>
class Tree
{
protected:
    struct TreeNode {
        DataSet<T> data;
        TreeNode  *left, *right;
        P          part;

        TreeNode(const DataSet<T> &);
        TreeNode(const DataSet<T> &, FILE *in);
        virtual ~TreeNode(void);

        void save(FILE *);
    };

    DataSet<T> data;
    TreeNode  *root;

public:
    Tree(const DataSet<T> &);
    Tree(const DataSet<T> &, FILE *in);
    virtual ~Tree(void);

    void save(FILE *);
    DataSet<T> find(const typename DataSet<T>::Vector &, 
            size_t min_leaf = 0);

    void show(void); /* tentative for debugging */
};

template <class T, class P>
Tree<T, P>::TreeNode::TreeNode(const DataSet<T> & data) 
    : data  (data)
    , left  (nullptr)
    , right (nullptr)
    , part  (data)
{
    DataSet<T> ldata = data.subset(part.get_left());
    DataSet<T> rdata = data.subset(part.get_right());
    if (ldata.size())
        left = new TreeNode(ldata);
    if (rdata.size())
        right = new TreeNode(rdata);
}

template <class T, class P>
Tree<T, P>::TreeNode::TreeNode(const DataSet<T> & data, FILE *in) 
    : data  (data)
    , left  (nullptr)
    , right (nullptr)
    , part  (data, in)
{
    DataSet<T> ldata = data.subset(part.get_left());
    DataSet<T> rdata = data.subset(part.get_right());
    if (ldata.size())
        left = new TreeNode(ldata, in);
    if (rdata.size())
        right = new TreeNode(rdata, in);
}

template <class T, class P>
Tree<T, P>::TreeNode::~TreeNode(void)
{ 
    if (left) delete left;
    if (right) delete right;
}

template <class T, class P>
void
Tree<T, P>::TreeNode::save(FILE * out)
{ 
    part.save(out);
    if (left) left->save(out);
    if (right) right->save(out);
}

template <class T, class P>
Tree<T, P>::Tree(const DataSet<T> & data) 
    : data (data)
    , root (new TreeNode(data))
{ }

template <class T, class P>
Tree<T, P>::Tree(const DataSet<T> & data, FILE* in) 
    : data (data)
    , root (new TreeNode(data, in))
{ }

template <class T, class P>
Tree<T, P>::~Tree(void)
{
    if (root) delete root;
}

template <class T, class P>
void
Tree<T, P>::save(FILE * out)
{ 
    if (root) root->save(out);
}


template <class T, class P>
DataSet<T>
Tree<T, P>::find(const typename DataSet<T>::Vector & vtr, size_t min_leaf)
{
    stack<TreeNode *> explore;
    set<size_t> uniq;

    explore.push(root);

    while (!explore.empty()) {
        TreeNode * current = explore.top();
        explore.pop();

        if (current->data.size() <= min_leaf) {
            uniq.insert(current->data.get_domain().begin(), 
                    current->data.get_domain().end());
            continue;
        } 

        Action action = current->part.query(vtr);

        switch(action) {
        case LEFT:
            if (current->left)
                explore.push(current->left);
            break;
        case RIGHT:
            if (current->right)
                explore.push(current->right);
            break;
        case SPILL:
            if (current->left)
                explore.push(current->left);
            if (current->right)
                explore.push(current->right);
            break;
        }
    }
    return root->data.subset(typename DataSet<T>::DomainSpace(uniq.begin(), uniq.end()));
}

template <class T, class P>
void 
Tree<T, P>::show(void)
{
    stack<pair<TreeNode *, size_t>> explore;
    explore.push(make_pair(root, 0));
    while (!explore.empty()) {
        pair<TreeNode *, size_t> current = explore.top();
        explore.pop();
        /* visit */
        for (size_t it = 0; it < current.second; it++) {
            printf("\t");
        }
        for (auto it = current.first->data.begin(); it != current.first->data.end(); it++) {
           printf("%lu ", it.get_index());
        }
        printf("\n");
        if (current.first->left) {
            explore.push(make_pair(current.first->left, current.second + 1));
        }
        if (current.first->right) {
            explore.push(make_pair(current.first->right, current.second + 1));
        }
    }
}

#endif /* TREE_H_ */
