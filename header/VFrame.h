#ifndef _VFRAME_HEAD
#define _VFRAME_HEAD

#include <stdint.h>

#define MAX_VFRAME_PAYLOAD_SIZE 1200
#define MAX_FRAME_NO 65534
#define HALF_OF_MAX_FRAME_NO (MAX_FRAME_NO / 2)
#define MAX_UDP_PACKET_BUFFER_SIZE 1300

#pragma pack(push, 1)

typedef struct VFrameHdr
{
    uint16_t frame_no;
    uint8_t syn;
    uint16_t packet_id;
    uint16_t packet_count;
    uint16_t payload_size;
} VFrameHdr;

typedef struct VFrame
{
    VFrameHdr fhdr;
    uint8_t payload[MAX_VFRAME_PAYLOAD_SIZE];
} VFrame;

#pragma pack(pop)

#endif