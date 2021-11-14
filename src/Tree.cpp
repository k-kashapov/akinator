#define LOGGING
#include "Tree.h"

const int POISON = 0x42;

Tree *CreateTree (tree_elem value)
{
    Tree *tree = (Tree*) calloc (1, sizeof (Tree));
    if (!tree)
    {
        LOG_ERROR ("OPEN_FILE_FAIL\n");
    }

    TNode *init_node = CreateNode (value);
    tree->root = init_node;

    tree->size = 1;
    #ifdef LOGGING
        OpenLogFile (&Log_file);
    #endif

    return tree;
}

TNode *GetRoot (Tree *tree)
{
    return tree->root;
}

TNode *CreateNode (tree_elem value)
{
    TNode *node_ptr = (TNode *) calloc (1, sizeof (TNode));
    if (!node_ptr)
    {
        LOG_FATAL ("MEM_ALLOC_ERR\n");
    }

    node_ptr->data = value;
    node_ptr->left = NULL;
    node_ptr->right = NULL;

    return node_ptr;
}

TNode *AddNodeLeft (TNode *node, tree_elem value)
{
    TNode *node_ptr = CreateNode (value);
    node->left      = node_ptr;

    return node_ptr;
}

TNode *AddNodeRight (TNode *node, tree_elem value)
{
    TNode *node_ptr = CreateNode (value);
    node->right     = node_ptr;

    return node_ptr;
}

void TreeNodePrint (TNode *node)
{
    printf (TYPE_SPEC"\n", node->data);
    return;
}

TNode *VisitNodePre (TNode *node, void (*action) (TNode *))
{
    if (!node) return 0;
    action (node);
    if (node->left)
    {
        VisitNodePre (node->left, action);
    }

    if (node->right)
    {
        VisitNodePre (node->right, action);
    }

    return 0;
}

int64_t TreeOk (Tree *tree)
{
    int64_t err = 0;

    err |= NodeOk (GetRoot (tree));

    return err;
}

int64_t NodeOk (TNode *node)
{
    int64_t err = 0;

    if (node->left && node->right)
    {
        err |= NodeOk (node->left);
        err |= NodeOk (node->right);
    }
    else
    {
        if (node->left || node->right)
        {
            LOG_ERROR ("LR_NOT_SAME\nnode: %p data: " TYPE_SPEC "\n",
                       , node, node->data);
            err |= LR_NOT_SAME;
        }
    }

    return err;
}

int DestructNode (TNode *node)
{
    if (node->left)
    {
        DestructNode (node->left);
    }
    node->left = (TNode *) POISON;

    if (node->right)
    {
        DestructNode (node->right);
    }
    node->right = (TNode *) POISON;

    free (node);
    return 0;
}

int DestructTree (Tree *tree)
{
    DestructNode (GetRoot (tree));
    free (tree);

    #ifdef LOGGING
        CloseLogFile (&Log_file);
    #endif
    return 0;
}
