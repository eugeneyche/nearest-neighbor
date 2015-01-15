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

template <class Label, class T>
class Tree
{
protected:
    struct TreeNode {
        DataSet<Label, T> * data;
        TreeNode * left, * right;

        TreeNode(DataSet<Label, T>*);
        ~TreeNode(void);
    };

    TreeNode          * root;
    DataSet<Label, T> * ref_DataSet;

    void buildTree(Part<Label, T> &);
    void loadTree(FILE *);

public:
    Tree(DataSet<Label, T> *, Part<Label, T> *);
    Tree(DataSet<Label, T> *, FILE *);
    ~Tree(void);

    void saveTree(FILE *);
    DataSet<Label, T>::Vector queryNN(DataSet<Label, T>::Vector);
    DataSet<Label, T>::Vector querySpillNN(DataSet<Label, T>::Vector, double);
};

template <class Label, class T>
Tree<Label, T>::TreeNode::TreeNode(DataSet<Label, T> * data) 
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
    if (!refDataSet)
        return;
    stack<pair<TreeNode **, DataSet<Label, T> *> > toBuild;
    toBuild.push(make_pair(&root, refDataSet->subset(refDataSet->getDomain())));

    while (!toBuild.empty()) {
        TreeNode ** tnodep = toBuild.top()->first;
        DataSet<Label, T> * tdata = toBuild.top()->second;
        toBuild.pop();
        if (!tnodep || !tdata)
            continue;
        *tnodep = new TreeNode(tdata);
        pair<DataSet<Label, T> *, DataSet<Label, T>*> partition = 
                part(tdata);
        toBuild.push(make_pair(&(*tnodep)->left, partition->first));
        toBuild.push(make_pair(&(*tnodep)->right, partition->second));
    }
} 

template <class Label, class T>
void
Tree<Label, T>::loadTree(FILE* in)
{ 
    size_t size;
    stack<pair<TreeNode **, DataSet<Label, T> *> > to_load;
    to_load.push(make_pair(&root, refDataSet)); 
    while (!to_load.empty()) {
        TreeNode ** tnodep = to_load.top()->first;
        DataSet<Label, T> * tdata = to_load.top()->first;
        if (!tnodep || !tdata)
            continue;
        fread(&size, sizeof(size_t), 1, in);
        if (!size) {
            *tnodep = nullptr;
            continue;
        }
        size_t buffer [size];
        fread(buffer, sizeof(size_t), size, in);
        typename DataSet<Label, T>::DomainSpace ds;
        ds.assign(buffer, buffer + size);
        *tnodep = new TreeNode(tdata->subset(ds));
        to_load.push(make_pair(&(*tnodep)->left, (*tnodep)->data));
        to_load.push(make_pair(&(*tnodep)->right, (*tnodep)->data));
    }
}

template <class Label, class T>
Tree<Label, T>::Tree(DataSet<Label, T>* refDataSet, Part<Label, T>* part) :
        root (NULL),
        refDataSet (refDataSet)
{ 
    buildTree(part);
}

template <class Label, class T>
Tree<Label, T>::Tree(DataSet<Label, T>* refDataSet, FILE* in) 
    : root (NULL)
    , refDataSet (refDataSet)
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
Tree<Label, T>::saveTree(FILE* out)
{ 
    stack<TreeNode*> to_save;
    to_save.push(root);
    while (!to_save.empty()) {
        TreeNode* tnode = to_save.top();
        size_t size = 0;
        if (!tnode) {
            fwrite(&size, sizeof(size_t), 1, out);
            continue;
        }
        to_save.pop();
        typename DataSet<Label, T>::DomainSpace ds = tnode->data->getDomain();
        size = ds.size();
        fwrite(&size, sizeof(size_t), 1, out);
        fwrite(&ds[0], sizeof(size_t), ds.size(), out);
        to_save.push(tnode->right);
        to_save.push(tnode->left);
    }
}

#endif /* TREE_H_ */
