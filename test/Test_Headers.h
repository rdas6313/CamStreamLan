#ifndef HEAD_TEST
#define HEAD_TEST

#ifdef LINUX
#define MACHINE_IP "192.168.0.122"
#else
#define MACHINE_IP "192.168.0.179"
#endif

#define MULTICAST_PORT 50002
#define MULTICAST_IP "235.0.0.1"
#define MACHINE_PORT MULTICAST_PORT
#define SHARED_BUFFER_SIZE (10 * sizeof(VideoFrame))

#define SHARED_KEY "buf"

#endif