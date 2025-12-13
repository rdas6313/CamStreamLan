#ifndef HEAD_VIDEOFRAME
#define HEAD_VIDEOFRAME
#include <stdint.h>

#define MAX_FRAME_SIZE ((1 << 20) + sizeof(uint32_t))

class VideoFrame
{
public:
    uint32_t size;
    uint8_t data[MAX_FRAME_SIZE];
};

#endif