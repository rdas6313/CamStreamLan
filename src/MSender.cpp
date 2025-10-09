#include "../header/Communicator.h"

bool MSender::set_multicast_addr(const uint8_t *multicast_ip, const uint16_t multicast_port)
{
    memset(&multicast_addr, 0, sizeof(multicast_addr));
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_port = htons(multicast_port);
    int8_t ret = inet_pton(AF_INET, (const char *)multicast_ip, &multicast_addr.sin_addr);
    if (ret <= 0)
    {
        Logger::elog("Setting Invalid multicast ip address");
        return false;
    }
    Logger::dlog("Setting Multicast address: OK");
    return true;
}

MSender::MSender(const uint8_t *bind_ip, const uint16_t bind_port, const uint16_t multicast_port, const uint8_t *to_multicast_ip) : MCommunicator(bind_ip, bind_port)
{
    if (socket_id < 0)
    {
        return;
    }
    bool isValid = set_multicast_addr(to_multicast_ip, multicast_port);
    if (!isValid)
    {
        close_socket();
    }
}

int32_t MSender::send(const uint8_t *buf, const uint32_t buf_size)
{
    if (socket_id < 0)
    {
        Logger::elog("Trying to send with un-intialize socket");
        return -1;
    }

    uint32_t sent_bytes = 0;
    uint8_t cur_try = 0;

    while (sent_bytes < buf_size && cur_try < MAX_SEND_TRY)
    {
        uint32_t cur_buf_size = min(MAX_BUF_SIZE, (buf_size - sent_bytes));
        uint32_t cur_sent_bytes = sendto(socket_id, buf + sent_bytes, cur_buf_size, 0, (struct sockaddr *)&multicast_addr, sizeof(multicast_addr));

        if (cur_buf_size < 0)
        {
            Logger::elog("Data Send error");

            if (cur_try < MAX_SEND_TRY)
                Logger::dlog("Trying to send sent-error data again!");
            cur_try += 1;
        }
        else
        {
            sent_bytes += cur_sent_bytes;
        }
    }
    Logger::dlog("Sent Packet: OK");
    return sent_bytes;
}
