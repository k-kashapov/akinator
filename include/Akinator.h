#include "Tree.h"
#include "Stack.h"
#include "files.h"

const int MAX_USER_INPUT = 32;

int BuildTreeFromFile (Tree *tree, File_info *file);

int CreateQuestion (TNode *destination, File_info *file, int *curr_line);

int GetArgs (int argc, const char **argv, Config *curr_config);

char *GetUserInput (void);

char UserAgrees (void);

int Guess (Tree *tree);

int AddObject (TNode *source);

void SaveBase (Config *config, Tree *tree);

void PrintDataToFile (TNode *node);

void LinkTreeNodes (TNode *src);

void PrintNodeDot (TNode *node);

void CreateImg (Tree *tree);

void FreeInputs (void);
