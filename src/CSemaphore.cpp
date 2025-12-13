#include "CSemaphore.h"

Semaphore::Semaphore(string name, int init_value) : name("/Sm_" + name)
{
    semaphore = sem_open(this->name.c_str(), O_CREAT, 0666, init_value);
    if (semaphore == SEM_FAILED)
    {
        perror("Semaphore: semaphore creation failed!");
        return;
    }
}
bool Semaphore::down()
{
    if (semaphore == SEM_FAILED)
    {
        perror("Semaphore: semaphore not initialized!");
        return false;
    }
    else if (sem_wait(semaphore) == -1)
    {
        perror("Semaphore: semaphore down failed");
        return false;
    }
    return true;
}
bool Semaphore::up()
{
    if (semaphore == SEM_FAILED)
    {
        perror("Semaphore: semaphore not initialized!");
        return false;
    }
    else if (sem_post(semaphore) == -1)
    {
        perror("Semaphore: semaphore up failed");
        return false;
    }
    return true;
}

#ifndef __APPLE__
int Semaphore::getValue()
{
    int value;
    if (semaphore == SEM_FAILED)
    {
        perror("Semaphore: semaphore not initialized!");
        return -1;
    }
    else if (sem_getvalue(semaphore, &value) == -1)
    {
        perror("Semaphore: semaphore get_value failed");
        return -1;
    }
    return value;
}
#endif

Semaphore::~Semaphore()
{
    if (semaphore != SEM_FAILED)
    {
        sem_unlink(this->name.c_str());
    }
}