#ifndef HEAD_SPCBuffer
#define HEAD_SPCBuffer

#include <string>
#include "Buffer.h"

#include "../header/SharedMem.h"
#include "../header/CSemaphore.h"
using std::string;

/*
 *   single producer and single consumer buffer
 */
template <typename T>
class SPCBuffer : public Buffer<T>
{
private:
    SharedMem *sharedMem;
    Semaphore *freeSemaphore;
    Semaphore *occupiedSemaphore;
    int *front;
    int *rear;
    int element_count;

public:
    SPCBuffer(string key, int size);
    ~SPCBuffer();
    std::optional<T> read();
    void write(T *data);
};

// <Implementations>

template <typename T>
SPCBuffer<T>::SPCBuffer(string key, int size)
{
    int element_size = sizeof(T);
    this->element_count = size / element_size;
    int queue_info_size = 2 * sizeof(int);
    sharedMem = new SharedMem(key, size + queue_info_size);
    freeSemaphore = new Semaphore(key + "_free", element_count);
    occupiedSemaphore = new Semaphore(key + "_occupied", 0);
    front = (int *)((T *)sharedMem->get_address() + this->element_count);
    rear = front + 1;

    if (*front < 0 || *rear < 0 || *front >= this->element_count || *rear >= this->element_count)
    {
        *front = 0;
        *rear = 0;
    }
}

template <typename T>
std::optional<T> SPCBuffer<T>::read()
{
    if (occupiedSemaphore == NULL || freeSemaphore == NULL || sharedMem == NULL)
    {
        perror("SPCBuffer Read: free or occupied semaphore or shared memory not initialized");
        return std::nullopt;
    }
    occupiedSemaphore->down();

    T *queue = (T *)sharedMem->get_address();
    T data = *(queue + *front);
    *front = (*front + 1) % element_count;

    freeSemaphore->up();
    return data;
}

template <typename T>
void SPCBuffer<T>::write(T *data)
{
    if (occupiedSemaphore == NULL || freeSemaphore == NULL || sharedMem == NULL)
    {
        perror("SPCBuffer Write: free or occupied semaphore or shared memory not initialized");
        return;
    }

    freeSemaphore->down();

    T *queue = (T *)sharedMem->get_address();
    *(queue + *rear) = *data;
    *rear = (*rear + 1) % element_count;

    occupiedSemaphore->up();
}

template <typename T>
SPCBuffer<T>::~SPCBuffer()
{
    if (sharedMem != NULL)
        delete sharedMem;
    if (freeSemaphore != NULL)
        delete freeSemaphore;
    if (occupiedSemaphore != NULL)
        delete occupiedSemaphore;
}

#endif