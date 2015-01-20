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
#include <utility>

#include "logging.h"
#include "dataset.h"
#include "part.h"

template <class Label, class T, class P>
class Tree
{
protected:
    struct TreeNode {
        DataSet<Label, T> data;
        TreeNode * left, * right;

        TreeNode(const DataSet<Label, T> &);
        ~TreeNode(void);
    };

    TreeNode          * root;
    Part              * part;
    DataSet<Label, T>   data;

    void buildTree(void);
    void loadTree(FILE *);

public:
    Tree(const DataSet<Label, T> &);
    Tree(const DataSet<Label, T> &, FILE *);
    ~Tree(void);

    void saveTree(FILE *);
    VSpill generateVSpill(double alpha);
    DataSet<Label, T> range(const typename DataSet<Label, T>::Vector, 
            int min = 0);
};

template <class Label, class T>
Tree<Label, T>::TreeNode::TreeNode(const DataSet<Label, T> & data) 
    : data  (data) 
    , left  (NULL) 
    , right (NULL)
{ }

template <class Label, class T>
Tree<Label, T>::TreeNode::~TreeNode(void)
{ 
    delete data;
    if (left)
        delete left;
    if (right)
        delete right;
}


template <class Label, class T>
void
Tree<Label, T>::buildTree(Part<Label, T> & part)
{ 
    stack<pair<TreeNode **, DataSet<Label, T>>> toBuild;
    toBuild.push(make_pair(&root, data));

    while (!toBuild.empty()) {
        TreeNode ** tnodep = toBuild.top().first;
        const DataSet<Label, T> & tdata = toBuild.top().second;
        toBuild.pop();
        if (!tnodep || tdata.size() == 0) {
            *tnodep = NULL;
            continue;
        }
        *tnodep = new TreeNode(tdata);
        pair<DataSet<Label, T>, DataSet<Label, T>> partition = part(tdata);
        toBuild.push(make_pair(&(*tnodep)->left, partition.first));
        toBuild.push(make_pair(&(*tnodep)->right, partition.second));
    }
} 

template <class Label, class T>
void
Tree<Label, T>::loadTree(FILE* in)
{ 
    size_t size;
    stack<pair<TreeNode **, DataSet<Label, T>>> to_load;
    to_load.push(make_pair(&root, data)); 
    while (!to_load.empty()) {
        TreeNode ** tnodep = to_load.top()->first;
        const DataSet<Label, T> & tdata = to_load.top()->first;
        if (!tnodep || tdata.size() == 0) {
            *tnodep = NULL;
            continue;
        }
        fread(&size, sizeof(size_t), 1, in);
        if (!size) {
            *tnodep = NULL;
            continue;
        }
        size_t buffer [size];
        fread(buffer, sizeof(size_t), size, in);
        typename DataSet<Label, T>::DomainSpace ds;
        ds.assign(buffer, buffer + size);
        *tnodep = new TreeNode(tdata.subset(ds));
        to_load.push(make_pair(&(*tnodep)->left, (*tnodep)->data));
        to_load.push(make_pair(&(*tnodep)->right, (*tnodep)->data));
    }
}

template <class Label, class T>
Tree<Label, T>::Tree(const DataSet<Label, T> & data, Part<Label, T> & part) :
        root (NULL),
        data (data)
{ 
    buildTree(part);
}

template <class Label, class T>
Tree<Label, T>::Tree(const DataSet<Label, T> & data, FILE* in) 
    : root (NULL)
    , data (data)
{ 
    loadTree(in);
}

template <class Label, class T>
Tree<Label, T>::~Tree(void)
{
    if (root)
        delete root;
}

template <class Label, class T>
void
Tree<Label, T>::saveTree(FILE * out)
{ 
    stack<TreeNode *> to_save;
    to_save.push(root);
    while (!to_save.empty()) {
        TreeNode* tnode = to_save.top();
        size_t size = 0;
        if (!tnode) {
            fwrite(&size, sizeof(size_t), 1, out);
            continue;
        }
        to_save.pop();
        typename DataSet<Label, T>::DomainSpace & ds = tnode->data.getDomain();
        size = ds.size();
        fwrite(&size, sizeof(size_t), 1, out);
        fwrite(&ds[0], sizeof(size_t), ds.size(), out);
        to_save.push(tnode->right);
        to_save.push(tnode->left);
    }
}


template <class Label, class T>
DataSet<Label, T>
Tree<Label, T>::range(const typename DataSet<Label, T>::Vector, int min)
{
    return DataSet<Label, T>();
}

#endif /* TREE_H_ */
