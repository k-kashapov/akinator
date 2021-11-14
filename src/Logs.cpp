#define LOGGING
#include "Logs.h"

#ifdef LOGGING
    int OpenLogFile (FILE **file_ptr)
    {
        *file_ptr = fopen ("Log.html", "w");
        if (!*file_ptr)
        {
            printf ("OPENING LOG FILE FAILED\n");
            return OPEN_FILE_FAIL;
        }

        fprintf (*file_ptr, "<style>\
                            table, th, td\
                            {\
                                border:1px solid black;\
                                border-radius:6px;\
                                margin-left:100px;\
                                margin-right:auto;\
                                margin-top:1%%;\
                                table-layout:fixed;\
                                width:100px;\
                                text-align:center;\
                            }\
                            rect\
                            {\
                                position: relative;\
                                overflow: hidden;\
                                border:1px solid black;\
                                border-radius:6px;\
                                margin-left:100px;\
                                margin-top:10px;\
                                font-size:18px;\
                                width:80px;\
                                max-width:80px;\
                                text-align:center;\
                                display: inline-block;\
                            }\
                            </style>\
                            <body>\
                            <pre>");
        return OK;
    }

    int CloseLogFile (FILE **file_ptr)
    {
        if (file_ptr)
        {
            fprintf (*file_ptr, "</pre></body>");
            fclose (*file_ptr);
            *file_ptr = NULL;
        }

        return OK;
    }
#endif
