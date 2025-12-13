#ifndef HEAD_SEMAPHORE
#define HEAD_SEMAPHORE
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

using std::string;
class Semaphore
{
private:
    const string name;
    sem_t *semaphore;

public:
    Semaphore(string key, int init_value);
    bool down();
    bool up();
#ifndef __APPLE__
    int getValue();
#endif
    ~Semaphore();
};

#endif