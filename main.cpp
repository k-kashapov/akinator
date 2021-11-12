#include "Tree.h"
#include "Stack.h"

int main (int argc, const char **argv)
{
    Tree *tree = CreateTree ("Unknown");
    AddNodeLeft (GetRoot (tree), "jojo");

    VisitNodePre (GetRoot (tree), TreeNodePrint);

    printf ("%ld\n", TreeOk (tree));

    DestructTree (tree);

    return 0;
}
