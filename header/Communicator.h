#ifndef _COMM_HEAD
#define _COMM_HEAD

#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Logger.h"

#define min(x, y) (x < y ? x : y)
#define max(x, y) (x < y ? y : x)
#define ALL_INTERFACE_IP ((const uint8_t *)"0.0.0.0")
#define MAX_SEND_TRY 3

class MCommunicator
{
protected:
    int32_t socket_id;
    MCommunicator(const uint8_t *bind_ip, const uint16_t bind_port);
    void close_socket();

public:
    static const uint32_t MAX_BUF_SIZE;
    virtual ~MCommunicator();
};

class MSender : public MCommunicator
{
private:
    struct sockaddr_in multicast_addr;

    bool set_multicast_addr(const uint8_t *multicast_ip, const uint16_t multicast_port);

public:
    MSender(const uint8_t *bind_ip, const uint16_t bind_port, const uint16_t multicast_port, const uint8_t *to_multicast_ip);
    int32_t send(const uint8_t *buf, const uint32_t buf_size);
};

class MReceiver : public MCommunicator
{
private:
    struct ip_mreq group_addr;

public:
    MReceiver(const uint8_t *bind_ip, const uint16_t bind_port, const uint8_t *join_multicast_ip, const uint8_t *join_multicast_interface_ip);
    ~MReceiver();
    int32_t receive(uint8_t *const buf, const uint32_t buf_size);
};

#endif