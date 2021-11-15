#include "Tree.h"
#include "Stack.h"
#include "files.h"

const int MAX_USER_INPUT = 16;

int BuildTreeFromFile (Tree *tree, File_info *file);

int CreateQuestion (TNode *destination, File_info *file, int *curr_line, FILE *Graph_file);

int GetArgs (int argc, const char **argv, Config *curr_config);

char *GetUserInput (void);

char UserAgrees (void);

int Guess (Tree *tree);

void PrintDataToFile (TNode *node);

void SaveBase (Config *config, Tree *tree);

void CreateImg (void);
