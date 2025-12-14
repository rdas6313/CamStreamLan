#ifndef HEAD_VIDEOFRAME
#define HEAD_VIDEOFRAME
#include <stdint.h>

#define MAX_FRAME_SIZE (500000)

class VideoFrame
{
public:
    uint32_t size;
    uint8_t data[MAX_FRAME_SIZE];
};

#endif