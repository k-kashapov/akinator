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

    Guess (tree);

    if (config.settings & UPDATE_BASE)
    {
        SaveBase (&config, tree);
    }

    DestructTree (tree);
    free_info (&info);

    if (config.settings & DOT_IMG)
    {
        CreateImg();
    }

    return 0;
}
