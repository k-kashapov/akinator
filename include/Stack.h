#include <malloc.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include "Logs.h"

//#define DEBUG_INFO
//#define CANARY_PROTECTION
#define CHECK_POISON
#define HASH_PROTECTION

#ifdef DEBUG_INFO
    #define MAX_INFO
#endif

#ifdef DEBUG_INFO
    #define DEBUG_DATA(...)  __VA_ARGS__
#else
    #define DEBUG_DATA(...)
#endif

typedef int64_t type_t;

#define TO_STRING(str) #str

#ifdef DEBUG_INFO
    #define STACK_OK(stk)                                   \
        Stack_Err = StackError (stk);                       \
        StackDump (stk, Stack_Err, __FUNCTION__, __LINE__);

#else
    #define STACK_OK(stk)
#endif

#ifdef HASH_PROTECTION
    #define COUNT_STACK_HASH(stk, res)                                                           \
        unsigned long len = sizeof (*stk) - sizeof (stk->struct_hash) - sizeof (stk->data_hash); \
        res = MurmurHash (stk, len);

    #define COUNT_DATA_HASH(stk, res)                                                \
        res = MurmurHash (stk->buffer, (unsigned long) stk->size * sizeof (type_t));
#endif

#define PRINT_ERR(err, flag)                                                   \
    if (err & flag)                                                            \
        fprintf (Log_file, "<p style = \"color : red\">ERROR: %s</p>", #flag);

#define REALLOC(ptr, init_len, new_len, ptr_type)         \
    ptr_type *buff = (ptr_type *) realloc (ptr, new_len); \
    assert (buff);                                        \
    ptr = buff;

#define MurmurHashStr(str) MurmurHash (str, strlen (str))

struct stack_t
{
    #ifdef CANARY_PROTECTION
        uint64_t canary_l;
    #endif

    type_t *buffer;
    long  capacity;
    long      size;

    #ifdef DEBUG_INFO
        const char* func;
        const char* file;
        int         line;
        const char* name;
    #endif

    #ifdef CANARY_PROTECTION
        uint64_t canary_r;
    #endif

    #ifdef HASH_PROTECTION
        unsigned long struct_hash;
        unsigned long data_hash;
    #endif
};

const int POISON = 0x42;

uint64_t StackDump (stack_t *stk, uint64_t err, const char *called_from, const int line_called_from);

unsigned long MurmurHash (const void *stk, unsigned long len);

uint64_t StackInit_ (stack_t *stk
                     DEBUG_DATA (, const char *file_name, const char *func_name, const int line, const char *name));

uint64_t StackDtor (stack_t *stk);

uint64_t StackError (stack_t *stk);

uint64_t StackResize (stack_t *stk, long new_capacity);

uint64_t StackPush (stack_t* stk, type_t value);

type_t StackPop (stack_t* stk, uint64_t *err = NULL);

type_t StackTop (stack_t *stk, uint64_t *err_ptr);
