#include "Tree.h"
#include "Logs.h"

Tree *CreateTree (tree_elem value)
{
    Tree *tree = (Tree*) calloc (1, sizeof (Tree));
    if (!tree)
    {
        LOG_ERROR (OPEN_FILE_FAIL\n);
    }

    TNode *init_node = CreateNode (value);
    tree->root = init_node;

    tree->size = 1;

    return tree;
};

TNode *CreateNode (tree_elem value)
{
    TNode *node_ptr = (TNode *) calloc (1, sizeof (TNode));
    node_ptr->data = value;
    node_ptr->left = NULL;
    node_ptr->right = NULL;

    return node_ptr;
};

TNode *VisitNodePre (TNode *node, void (*action) (TNode *))
{

    return 0;
}
