#pragma once
#include <stdio.h>

#define DUMP_ERR(name, code) name = code,
enum ErrorCodes
{
    #include "errors.h"
};
#undef DUMP_ERR

#ifdef LOGGING
    int OpenLogFile (FILE **file_ptr);
    int CloseLogFile (FILE **file_ptr);
    static FILE *Log_file = stderr;
#endif

#define LOG_ERROR(string, ...)                                                  \
            fprintf (Log_file, "%s: ERROR: " string "\n",                       \
                                 __FUNCTION__ __VA_ARGS__)

#define LOG_FATAL(string, ...) fprintf (Log_file, "FATAL: " string __VA_ARGS__)
