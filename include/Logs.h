#pragma once

#include <stdio.h>

#define DUMP_ERR(name, code) name = code,
enum ErrorCodes
{
    #include "errors.h"
};
#undef DUMP_ERR

#ifdef LOGGING
    int OpenLogFile ();
    int CloseLogFile ();
    static FILE *Log_file = NULL;

    #define Log_Output Log_file
#else
    #define Log_Output stderr
#endif

#define LOG_ERROR(string, ...)                                              \
            fprintf (Log_Output, "%s: ERROR: " #string "\n",                \
                                 __FUNCTION__ __VA_ARGS__)

#define LOG_FATAL(string, ...) fprintf (Log_Output, "FATAL: " #string __VA_ARGS__)
