#define LOGGING
#include "Tree.h"
#include "Stack.h"
#include "files.h"

int BuildTreeFromFile (Tree *tree, File_info *file);

int CreateQuestion (TNode *destination, File_info *file, int *curr_line, FILE *Graph_file);
