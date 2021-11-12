#include <stdio.h>
#include "Stack.h"

typedef char* tree_elem;

struct TNode
{
    tree_elem data;
    TNode *left;
    TNode *right;
};

struct Tree
{
    TNode *root;
    long size;
};

Tree *CreateTree (tree_elem value = NULL);

TNode *CreateNode (tree_elem value = NULL);

TNode *VisitNodePre (TNode *node, void (*action) (TNode *));
