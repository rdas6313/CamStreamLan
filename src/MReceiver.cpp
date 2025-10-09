#include "../header/Communicator.h"

MReceiver::MReceiver(const uint8_t *bind_ip, const uint16_t bind_port, const uint8_t *join_multicast_ip, const uint8_t *join_multicast_interface_ip) : MCommunicator(((bind_ip == NULL) ? ALL_INTERFACE_IP : bind_ip), bind_port)
{
    if (socket_id < 0)
    {
        return;
    }

    int8_t ret = inet_pton(AF_INET, (const char *)join_multicast_ip, &group_addr.imr_multiaddr);
    if (ret <= 0)
    {
        Logger::elog("Invalid multicast join ip address");
        close_socket();
        return;
    }
    ret = inet_pton(AF_INET, (const char *)join_multicast_interface_ip, &group_addr.imr_interface);
    if (ret <= 0)
    {
        Logger::elog("Invalid multicast join interface ip address");
        close_socket();
        return;
    }

    Logger::dlog("Setting Multicast ip and interface:   OK");

    if (setsockopt(socket_id, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group_addr, sizeof(group_addr)) < 0)
    {
        Logger::elog("Multicast Membership request error:");
        close_socket();
        return;
    }

    Logger::dlog("Membership Add Request:   OK");
}

MReceiver::~MReceiver()
{
    if (socket_id > -1)
    {
        if (setsockopt(socket_id, IPPROTO_IP, IP_DROP_MEMBERSHIP, &group_addr, sizeof(group_addr)) < 0)
        {
            Logger::elog("Multicast Membership drop error:");
        }
        else
        {
            Logger::dlog("Membership Drop Request:   OK");
        }
    }
}

int32_t MReceiver::receive(uint8_t *const buf, const uint32_t buf_size)
{
    if (socket_id < 0)
    {
        Logger::elog("Trying to read using un-initialize socket");
        return -1;
    }
    struct sockaddr_in from;
    memset(&from, 0, sizeof(from));
    socklen_t fromlen = sizeof(from);

    ssize_t bytes = recvfrom(socket_id, (char *)buf, buf_size, 0, (struct sockaddr *)&from, &fromlen);
    if (bytes < 0)
    {
        perror("Reading Multicast packet error:");
        close_socket();
        return -1;
    }
    char ip[35];
    uint8_t *ret = (uint8_t *)inet_ntop(AF_INET, &from.sin_addr, ip, sizeof(ip));
    if (ret != NULL)
    {
        char msg[100] = {0};
        strcat(msg, "Received Data From: ");
        strcat(msg, ip);
        Logger::dlog(msg);
    }
    return bytes;
}