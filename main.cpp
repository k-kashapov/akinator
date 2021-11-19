#define LOGGING
#include "Akinator.h"

int main (int argc, const char **argv)
{
    Config config = {};
    File_info info = {};

    GetArgs (argc, argv, &config);

    long lines_num = read_all_lines (&info, config.input_file);
    if (lines_num < 1)
    {
        LOG_ERROR ("ZERO LINES READ\n");
        return OPEN_FILE_FAILED;
    }

    Tree *tree = CreateTree ("Unknown");

    BuildTreeFromFile (tree, &info);

    while (true)
    {
        printf ("Pick an option:\n\t1) Guess\n\t2) Definiton\n\t3) Compare\n\t");
        char *input = GetUserInput();

        if (!input)
        {
            printf ("INVALID INPUT\n");
            return -1;
        }

        switch (*input)
        {
            case 49: // ASCII code '1'
                Guess (tree);
                break;
            case 50: // ASCII code '2'
                GetDefinition (tree);
                break;
            case 51: // ASCII code '3'
                Compare (tree);
                break;
            default:
                printf ("No such option\n");
        }
        free (input);

        printf ("Do you want to see the base?\n\t");
        if (UserAgrees())
        {
            CreateImg(tree);
        }

        printf ("Do you want to save the base?\n\t");
        if (UserAgrees())
        {
            SaveBase (&config, tree);
        }

        printf ("Play again?\n\t");
        if (!UserAgrees())
        {
            printf ("Commencing self-destruct\n");
            break;
        }
    }

    DestructTree (tree);
    FreeInputs ();
    free_info (&info);

    return 0;
}
