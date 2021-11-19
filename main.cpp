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
        Guess (tree);

        printf ("Wanna see the base?\n\t");
        if (UserAgrees())
        {
            CreateImg(tree);
        }

        printf ("Wanna save the base?\n\t");
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
