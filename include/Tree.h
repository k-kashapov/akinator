#define LOGGING

#include "Logs.h"
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

typedef const char* tree_elem;
#define TYPE_SPEC "%s"

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

TNode *GetRoot (Tree *tree);

TNode *CreateNode (tree_elem value = NULL);

TNode *AddNodeLeft (TNode *node, tree_elem value);

TNode *AddNodeRight (TNode *node, tree_elem value);

void TreeNodePrint (TNode *node);

TNode *VisitNodePre (TNode *node, void (*action) (TNode *));

int64_t TreeOk (Tree *tree);

int64_t NodeOk (TNode *node);

int DestructNode (TNode *node);

int DestructTree (Tree *tree);
