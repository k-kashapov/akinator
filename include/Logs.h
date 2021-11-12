#include <stdio.h>
#include "Stack.h"

#ifdef LOGGING
    static int OpenLogFile ();
    static int CloseLogFile ();
    static FILE *Log_file   = NULL;

    #define Log_Output Log_file
    #define DUMP_ERR(name, code)                                                \
        if (err & code)                                                         \
        {                                                                       \
            LOG_ERROR (name\n);                                                 \
        }
#else
    #define Log_Output stderr
#endif

#define LOG_ERROR(string, ...)                                              \
            fprintf (Log_Output, "%s: ERROR: " #string "\n",                \
                                 __FUNCTION__ __VA_ARGS__)

#define LOG_FATAL(string) fprintf (Log_Output, "FATAL: " string)
