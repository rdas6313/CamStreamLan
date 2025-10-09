#ifndef _LOGGER_HEAD
#define _LOGGER_HEAD

#include <stdio.h>
#include <cstring>
#include <errno.h>

class Logger
{
public:
    static void dlog(const char *data);
    static void elog(const char *data);
};

#endif