#include "Akinator.h"

int main (int argc, const char **argv)
{
    Config io_config = {};
    File_info info = {};

    get_io_args (argc, argv, &io_config);

    long lines_num = read_all_lines (&info, io_config.input_file);

    Tree *tree = CreateTree ("Unknown");

    BuildTreeFromFile (tree, &info);
    VisitNodePre (GetRoot (tree), TreeNodePrint);
    putc ('\n', stdout);

    DestructTree (tree);
    free_info (&info);

    system ("dot dotInput.dot  -Tpng -o Img.png");

    return 0;
}
