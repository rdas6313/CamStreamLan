#ifndef HEAD_SHARED_MEM
#define HEAD_SHARED_MEM

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>

#include <stdio.h> // for perror

using std::string;

class SharedMem
{
private:
    const string name;
    int fd;
    int size; // in bytes
    void *address;

public:
    SharedMem(string key, int size);
    ~SharedMem();
    void *get_address();
};

#endif