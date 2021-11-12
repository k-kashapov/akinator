#ifdef LOGGING
    static int OpenLogFile ()
    {
        Log_file = fopen ("ListLog.html", "w");
        if (!Log_file)
        {
            printf ("OPENING LOG FILE FAILED\n");
            return OPEN_FILE_FAIL;
        }

        fprintf (Log_file, "<style>\
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

    static int CloseLogFile ()
    {
        if (Log_file)
        {
            fprintf (Log_file, "</pre></body>");
            fclose (Log_file);
            Log_file = NULL;
        }

        return OK;
    }
#endif
