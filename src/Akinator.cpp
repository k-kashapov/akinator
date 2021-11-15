#include "Akinator.h"

int GetArgs (int argc, const char **argv, Config *curr_config)
{
    while (--argc > 0)
    {
        argv++;

        if (!strncmp (*argv, "-i", 3))
        {
            curr_config->input_file = *(++argv);
            argc--;
        }
        else if (!strncmp (*argv, "-o", 3))
        {
            curr_config->output_file = *(++argv);
            argc--;
        }
        else if (!strncmp (*argv, "--Dot", 6))
        {
            curr_config->settings |= DOT_IMG;
        }
        else if (!strncmp (*argv, "--Save", 7))
        {
            curr_config->settings |= UPDATE_BASE;
        }
    }
    return 0;
}

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
    printf ("\u25BC\u25BC\u25BC\nЧитаем строчку: ");
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

    printf ("Построение закончено (%s)\n\u25B2\u25B2\u25B2\n", str.text);

    return OK;
}

char *GetUserInput (void)
{
    char *user_input = (char *) calloc (MAX_USER_INPUT, sizeof (char));
    fgets (user_input, MAX_USER_INPUT, stdin);
    if (user_input[MAX_USER_INPUT - 2] != '\0')
    {
        while (getchar() != '\n') ;
    }

    return user_input;
}

char UserAgrees (void)
{
    char *input = GetUserInput();
    char agrees = *input == 'y';
    free (input);

    return agrees;
}

int Guess (Tree *tree)
{
    assert (tree);

    printf ("Let's dance!\n");
    for (TNode *curr = GetRoot (tree); curr;)
    {
        printf ("Это %s\n", curr->data);
        char agrees = UserAgrees();
        if (agrees)
        {
            if (curr->left)
            {
                curr = curr->left;
            }
            else
            {
                printf ("Очев\n");
                break;
            }
        }
        else
        {
            if (curr->right)
            {
                curr = curr->right;
            }
            else
            {
                printf ("Тогда не знаю\n");
                break;
            }
        }
    }

    return OK;
}

void SaveBase (Config *config, Tree *tree)
{
    int stdout_old = fileno (stdout);
    freopen (config->output_file, "wt", stdout);

    VisitNodePre (GetRoot (tree), PrintDataToFile);
    fclose (stdout);

    stdout = fdopen (stdout_old, "wt");

    return;
}

void PrintDataToFile (TNode *node)
{
    fprintf (stdout, "%s\n", node->data);
    return;
}

void CreateImg (void)
{
    system ("dot dotInput.dot -Tpng -o Tree.png");
    system ("eog Tree.png");
    return;
}
