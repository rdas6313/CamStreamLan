#ifndef HEAD_BUF
#define HEAD_BUF

#include <string>
#include <optional>
using std::string;

template <typename T>
class Buffer
{
public:
    virtual std::optional<T> read() = 0;
    virtual void write(T *data) = 0;
};

#endif