#ifndef TREE_H_
#define TREE_H_

#include <stdio.h>

#include <stack>
#include <queue>
#include <map>
#include <utility>

#include "logging.h"
#include "dataset.h"
#include "part.h"

template <class Label, class T>
class Tree;

template <class Label, class T>
class Tree
{
protected:
    struct TreeNode {
        DataSet<Label, T>* data;
        TreeNode* left, * right;

        TreeNode(DataSet<Label, T>*);
        ~TreeNode(void);
    };

    TreeNode*                   root;
    DataSet<Label, T>*          refDataSet;

    void buildTree(Part<Label, T>*);
    void loadTree(FILE*);

public:
    Tree(DataSet<Label, T>*, Part<Label, T>*);
    Tree(DataSet<Label, T>*, FILE*);
    ~Tree(void);

    void saveTree(FILE*);
};

template <class Label, class T>
Tree<Label, T>::TreeNode::TreeNode(DataSet<Label, T>* data) :
        data  (data), 
        left  (nullptr), 
        right (nullptr)
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
Tree<Label, T>::buildTree(Part<Label, T>* part)
{ 
    if (!refDataSet)
        return;

    stack<pair<TreeNode**, DataSet<Label, T>*> > toBuild;
    toBuild.push(make_pair(&root, refDataSet->subset(refDataSet->getDomain())));

    while (!toBuild.empty()) {
        TreeNode** tNodep = toBuild.top()->first;
        DataSet<Label, T>* tData = toBuild.top()->second;
        toBuild.pop();
        if (!tNodep || !tData)
            continue;
        *tNodep = new TreeNode(tData);
        pair<DataSet<Label, T>*, DataSet<Label, T>*> partition = 
                *part << tData;
        toBuild.push(make_pair(&(*tNodep)->left, partition->first));
        toBuild.push(make_pair(&(*tNodep)->right, partition->second));
    }
} 

template <class Label, class T>
void
Tree<Label, T>::loadTree(FILE* in)
{ 
    size_t size;
    stack<pair<TreeNode**, DataSet<Label, T>*> > toLoad;
    toLoad.push(make_pair(&root, refDataSet)); 
    while (!toLoad.empty()) {
        TreeNode** tNodep = toLoad.top()->first;
        DataSet<Label, T>* tData = toLoad.top()->first;
        if (!tNodep || !tData)
            continue;
        fread(&size, sizeof(size_t), 1, in);
        if (!size) {
            *tNodep = nullptr;
            continue;
        }
        size_t buffer [size];
        fread(buffer, sizeof(size_t), size, in);
        typename DataSet<Label, T>::DomainSpace ds;
        ds.assign(buffer, buffer + size);
        *tNodep = new TreeNode(tData->subset(ds));
        toLoad.push(make_pair(&(*tNodep)->left, (*tNodep)->data));
        toLoad.push(make_pair(&(*tNodep)->right, (*tNodep)->data));
    }
}

template <class Label, class T>
Tree<Label, T>::Tree(DataSet<Label, T>* refDataSet, Part<Label, T>* part) :
        root (nullptr),
        refDataSet (refDataSet)
{ 
    buildTree(part);
}

template <class Label, class T>
Tree<Label, T>::Tree(DataSet<Label, T>* refDataSet, FILE* in) :
        root (nullptr),
        refDataSet (refDataSet)
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
    stack<TreeNode*> toSave;
    toSave.push(root);
    while (!toSave.empty()) {
        TreeNode* tNode = toSave.top();
        size_t size = 0;
        if (!tNode) {
            fwrite(&size, sizeof(size_t), 1, out);
            continue;
        }
        toSave.pop();
        typename DataSet<Label, T>::DomainSpace ds = tNode->data->getDomain();
        size = ds.size();
        fwrite(&size, sizeof(size_t), 1, out);
        fwrite(&ds[0], sizeof(size_t), ds.size(), out);
        toSave.push(tNode->right);
        toSave.push(tNode->left);
    }
}

#endif /* TREE_H_ */
