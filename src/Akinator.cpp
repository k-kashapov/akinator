#include "Akinator.h"

static FILE *OpenGraphFile (const char *name)
{
    FILE *file_ptr = fopen (name, "w");
    if (!file_ptr)
    {
        printf ("OPENING GRAPH FILE FAILED\n");
        return NULL;
    }
    fprintf (file_ptr,  "digraph\n{\nrankdir = \"TB\";\n"
                        "splines = false;\n");
    return file_ptr;
}

static int PrintNodeDot (FILE *Graph_file, TNode *node)
{
    int printed = fprintf (Graph_file,
                        "NODE%p"
                        "["
                            "shape=rectangle, style = \"rounded,filled\", "
                            "fillcolor=\"%s\", "
                            "label = \""
                            "%s\""
                        "]\n",
                        node, node->left ? "lightblue" : "lime",
                        node->data
            );

    return printed;
}

static int LinkTreeNodes (FILE *Graph_file, TNode *src)
{
    fprintf (Graph_file, "NODE%p:sw->NODE%p:n [label = \"Да\"]\n", src, src->left);
    fprintf (Graph_file, "NODE%p:se->NODE%p:n [label = \"Нет\"]\n", src, src->right);

    return 0;
}

int BuildTreeFromFile (Tree *tree, File_info *file)
{
    TNode *root = GetRoot (tree);

    FILE *Graph_file = OpenGraphFile ("dotInput.dot");

    int curr_line = 0;

    CreateQuestion (root, file, &curr_line, Graph_file);

    fprintf (Graph_file, "\n}");

    fclose (Graph_file);

    return OK;
}

int CreateQuestion (TNode *destination, File_info *file, int *curr_line, FILE *Graph_file)
{
    printf ("VVV\nЧитаем строчку: ");
    String str = *file->strs[(*curr_line)++];
    printf ("%s\n", str.text);
    destination->data = str.text;

    if (str.text[str.len - 1] == '?')
    {
        printf ("Это вопрос => создаём двух потомков:\n");
        destination->left = CreateNode ("");
        destination->right = CreateNode ("");
        PrintNodeDot (Graph_file, destination);

        CreateQuestion (destination->left,  file, curr_line, Graph_file);
        CreateQuestion (destination->right, file, curr_line, Graph_file);

        LinkTreeNodes (Graph_file, destination);
    }
    else
    {
        PrintNodeDot (Graph_file, destination);
    }

    printf ("Построение закончено (%s)\n^^^\n", str.text);


    return OK;
}
