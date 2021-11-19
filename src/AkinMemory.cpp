#include "Akinator.h"
static size_t  Inputs_capacity = 2;
static size_t  Inputs_num      = 0;
static char    **User_inputs   = NULL;

int BuildTreeFromFile (Tree *tree, File_info *file)
{
    TNode *root = GetRoot (tree);

    int curr_line = 0;
    CreateQuestion (root, file, &curr_line);

    User_inputs = (char **) calloc (Inputs_capacity, sizeof (char **));

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

    input[input_len] = '\0';

    printf ("Adding to base...\n");

    User_inputs[Inputs_num++] = input;

    source->data = input;

    return OK;
}

void FreeInputs (void)
{
    for (size_t input = 0; input < Inputs_num; input++)
    {
        free (User_inputs[input]);
    }

    return;
}
