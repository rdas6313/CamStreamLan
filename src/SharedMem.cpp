#include "SharedMem.h"

SharedMem::SharedMem(string name, int size) : name("/" + name)
{
    this->size = size;
    fd = shm_open(this->name.c_str(), O_CREAT | O_EXCL | O_RDWR, 0666);
    if (fd < 0 && errno == EEXIST)
    {
        fd = shm_open(this->name.c_str(), O_RDWR, 0666);
    }
    if (fd < 0)
    {
        perror("SharedMem: shared file object creation error!");
        return;
    }
    if (errno != EEXIST && ftruncate(fd, size) < 0)
    {
        perror("SharedMem: shared file object size increasing error");
    }
    address = NULL;
}

SharedMem::~SharedMem()
{
    if (address != NULL)
        munmap(address, this->size);
    address = NULL;
    close(fd);
    shm_unlink(this->name.c_str());
}

void *SharedMem::get_address()
{
    if (address == NULL)
    {
        address = mmap(NULL, this->size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (address == MAP_FAILED)
        {
            perror("SharedMem: unable to map shared memory to process address space");
            address = NULL;
        }
    }
    return address;
}