#include "Akinator.h"

static FILE *Graph_file  = NULL;
static FILE *New_base    = NULL;

static size_t  Inputs_capacity = 2;
static size_t  Inputs_num      = 0;
static char    **User_inputs   = NULL;

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

static FILE *OpenFile (const char *name)
{
    FILE *file_ptr = fopen (name, "w");
    if (!file_ptr)
    {
        printf ("OPENING FILE FAILED\n");
        return NULL;
    }

    return file_ptr;
}

void PrintNodeDot (TNode *node)
{
    fprintf (Graph_file, "NODE%p"
            "["
                "shape=rectangle, style = \"rounded,filled\", "
                "fillcolor=\"%s\", "
                "label = \""
                "%s\""
            "]\n",
            node, node->left ? "lightblue" : "lime",
            node->data
            );

    LinkTreeNodes (node);

    return;
}

void LinkTreeNodes (TNode *src)
{
    if (src->left)
        fprintf (Graph_file, "NODE%p:sw->NODE%p:n [xlabel = \"Да\", "
                             "minlen = \"2\"]\n", src, src->left);
    if (src->right)
        fprintf (Graph_file, "NODE%p:se->NODE%p:n [xlabel = \"Нет\", "
                             "minlen = \"2\"]\n", src, src->right);

    return;
}

int BuildTreeFromFile (Tree *tree, File_info *file)
{
    TNode *root = GetRoot (tree);

    int curr_line = 0;
    CreateQuestion (root, file, &curr_line);

    User_inputs = (char **) calloc (Inputs_capacity, sizeof (char **));

    return OK;
}

int CreateQuestion (TNode *destination, File_info *file, int *curr_line)
{
    assert (destination);

    printf ("\u25BC\u25BC\u25BC\nReading a line: ");
    String str = *file->strs[(*curr_line)++];
    printf ("%s\n", str.text);
    destination->data = str.text;

    if (str.text[str.len - 1] == '?')
    {
        printf ("That's a question => creating two children:\n");
        destination->left = CreateNode ("");
        destination->right = CreateNode ("");

        CreateQuestion (destination->left,  file, curr_line);
        CreateQuestion (destination->right, file, curr_line);
    }

    printf ("Build complete (%s)\n\u25B2\u25B2\u25B2\n", str.text);

    return OK;
}

char *GetUserInput (void)
{
    char *user_input = (char *) calloc (MAX_USER_INPUT + 2, sizeof (char));
    fgets (user_input, MAX_USER_INPUT, stdin);

    if (user_input[MAX_USER_INPUT - 2] != '\0')
    {
        user_input[MAX_USER_INPUT - 2] = '\0';
        while (getchar() != '\n') ;
    }
    else
    {
        *strchr (user_input, '\n') = '\0';
    }

    printf ("%s\n", user_input);

    return user_input;
}

char UserAgrees (void)
{
    char *input = GetUserInput();
    while (*input == '\0')
    {
        free (input);
        input = GetUserInput();
    }

    char agrees = (*input == 'y' || *(wchar_t *)input == 'д');
    free (input);

    return agrees;
}

int Guess (Tree *tree)
{
    assert (tree);

    printf ("Let's dance!\n");
    for (TNode *curr = GetRoot (tree); curr;)
    {
        if (curr->left)
        {
            printf ("Answer this:\n\t%s\n\t", curr->data);
        }
        else
        {
            printf ("I guess, that's\n\t%s\n\t", curr->data);
        }
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
                AddObject (curr);
                break;
            }
        }
    }

    return OK;
}

int AddObject (TNode *source)
{
    AddNodeRight (source, source->data);

    printf ("Who was that?\n\t");
    char *input = GetUserInput();

    User_inputs[Inputs_num++] = input;
    if (Inputs_num >= Inputs_capacity - 1)
    {
        char **tmp = (char **) realloc (User_inputs, Inputs_capacity * 2 * sizeof (char **));
        if (!tmp)
        {
            printf ("MEMORY ALLOCATION ERROR");
            return MEM_ALLOC_ERR;
        }
        User_inputs = tmp;
        Inputs_capacity *= 2;
    }

    AddNodeLeft (source, input);

    printf ("What's the difference between %s and %s?\n\t", input, source->data);

    input = GetUserInput();
    unsigned long input_len = strlen (input);

    input[input_len] = '?';
    input[input_len + 1] = '\0';

    printf ("Adding to base...\n");

    User_inputs[Inputs_num++] = input;

    source->data = input;

    return OK;
}

void SaveBase (Config *config, Tree *tree)
{
    New_base = OpenFile (config->output_file);

    VisitNode (GetRoot (tree), PrintDataToFile, NULL, NULL);

    fclose (New_base);

    return;
}

void PrintDataToFile (TNode *node)
{
    fprintf (New_base, "%s\n", node->data);
    return;
}

void CreateImg (Tree *tree)
{
    Graph_file = OpenFile ("dotInput.dot");

    fprintf (Graph_file, "digraph\n{\nrankdir = \"TB\";\n"
                         "splines = true;\n");

    VisitNode (GetRoot (tree), PrintNodeDot, NULL, NULL);

    fprintf (Graph_file, "\n}");
    fclose (Graph_file);

    system ("dot dotInput.dot -Tpng -o Tree.png");
    system ("eog Tree.png");

    return;
}

void FreeInputs (void)
{
    for (size_t input = 0; input < Inputs_num; input++)
    {
        free (User_inputs[input]);
    }

    return;
}
