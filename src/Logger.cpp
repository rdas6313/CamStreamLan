#include "../header/Logger.h"

void Logger::elog(const char *data)
{
    printf("Error LOG: %s :(%s)\n", data, strerror(errno));
    fflush(stdout);
}

void Logger::dlog(const char *data)
{
    printf("DEBUG LOG: %s\n", data);
    fflush(stdout);
}