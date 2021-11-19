#include "Akinator.h"
#include <stdarg.h>

static FILE *Graph_file  = NULL;
static FILE *New_base    = NULL;

#define DOWN_ARROWS "\u25BC\u25BC\u25BC"
#define UP_ARROWS   "\u25B2\u25B2\u25B2"

int meowprintf (const char* format, ...);

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

int CreateQuestion (TNode *destination, File_info *file, int *curr_line)
{
    assert (destination);

    printf (DOWN_ARROWS "\nReading a line: ");
    String str = *file->strs[(*curr_line)++];
    printf ("%s\n", str.text);
    destination->data = str.text;

    if (str.text[str.len - 1] == '?')
    {
        str.text[str.len - 1] = '\0';
        printf ("That's a question => creating two children:\n");
        destination->left = CreateNode ("");
        destination->right = CreateNode ("");

        CreateQuestion (destination->left,  file, curr_line);
        CreateQuestion (destination->right, file, curr_line);
    }

    printf ("Build complete (%s)\n" UP_ARROWS "\n", str.text);

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

    char agrees = (*input == 'y' || strncmp (input, "д", 3));
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
            meowprintf ("The question is:\n\t%s?\n\t", curr->data);
        }
        else
        {
            printf ("I guess, that's\n\t%s\n\t", curr->data);
            char command[100] = {};
            sprintf (command, "espeak -s 90 -vru \"I guess that's  %s\"", curr->data);
            system (command);
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
                printf ("Ochev\n");
                system ("espeak -vru \"Ochev\"");
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

int meowprintf (const char* format, ...)
{
    char command[100] = "espeak -vru \"";

    va_list arg = {};
    va_start (arg, format);
    vsprintf (command + strlen (command), format, arg);
    va_end (arg);

    va_start (arg, format);
    vprintf (format, arg);
    va_end (arg);

    strcat (command, "\"");

    return system (command);
}

TNode *SearchNode (const char *key, TNode *node, stack_t *stk)
{
    StackPush (stk, (type_t)node);

    if (!strcmp (key, node->data) && !(node->left))
    {
        return node;
    }

    if (node->left)
    {
        TNode *result = SearchNode (key, node->left, stk);
        if (result) return result;
    }

    if (node->right)
    {
        TNode *result = SearchNode (key, node->right, stk);
        if (result) return result;
    }

    uint64_t stk_err = 0;
    StackPop (stk, &stk_err);

    if (stk_err)
    {
        printf ("ERROR: Stack POP: %06lx\n", stk_err);
        return NULL;
    }

    return NULL;
}

TNode *FindByData (const char *key, Tree *tree, stack_t *stk)
{
    StackPush (stk, 0);

    TNode *result = SearchNode (key, GetRoot (tree), stk);
    if (result)
    {
        return result;
    }

    return NULL;
}

TNode *GetDefinition (Tree *tree)
{
    printf ("Enter a name:\n\t");
    char *key = GetUserInput();

    stack_t stk = {};

    StackInit_ (&stk);

    TNode *found = FindByData (key, tree, &stk);

    if (!found)
    {
        printf ("%s not found!\n", key);
        StackDtor (&stk);
        free (key);
        return NULL;
    }

    printf ("%s is ", key);

    for (long item = 1; item < stk.size - 2; item++)
    {
        TNode *current = (TNode *) stk.buffer[item];
        TNode *next    = (TNode *) stk.buffer[item + 1];
        int   is_child = (current->left == next);
        printf ("%s%s, ", is_child ? "" : "not ", current->data);
    }

    TNode *current = (TNode *) stk.buffer[stk.size - 2];
    TNode *next    = (TNode *) stk.buffer[stk.size - 1];
    int   is_child = (current->left == next);

    printf ("and %s%s\n", is_child ? "" : "not ", current->data);

    StackDtor (&stk);
    free (key);

    return found;
}

TNode *Compare (Tree *tree)
{
    printf ("Enter first name:\n\t");
    char *key = GetUserInput();

    stack_t stk1 = {};

    StackInit_ (&stk1);

    TNode *found1 = FindByData (key, tree, &stk1);
    if (!found1)
    {
        printf ("%s not found!\n", key);
        StackDtor (&stk1);
        free (key);
        return NULL;
    }
    free (key);

    printf ("Enter second name:\n\t");
    key = GetUserInput();

    stack_t stk2 = {};

    StackInit_ (&stk2);

    TNode *found2 = FindByData (key, tree, &stk2);
    if (!found2)
    {
        printf ("%s not found!\n", key);
        StackDtor (&stk1);
        StackDtor (&stk2);
        free (key);
        return NULL;
    }
    free (key);

    for (long item = 1; item < stk1.size - 1 && item < stk2.size - 1; item++)
    {
        if (stk2.buffer[item] != stk1.buffer[item])
        {
            printf ("%s and %s, we are not the same. %s is %s and %s is %s\n",
                    found1->data, found2->data, found1->data,
                    ((TNode *)stk1.buffer[item])->data, found2->data,
                    ((TNode *)stk2.buffer[item])->data
                    );
            break;
        }
        else
        {
            printf ("%s and %s are both %s\n",
                    found1->data, found2->data,
                    ((TNode *) stk1.buffer[item])->data);
        }
    }

    StackDtor (&stk1);
    StackDtor (&stk2);
    return NULL;
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
    fprintf (New_base, "%s%s\n", node->data, node->left ? "?" : "");
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
