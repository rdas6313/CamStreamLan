#include "../header/Communicator.h"

const uint32_t MCommunicator::MAX_BUF_SIZE = 1300;

MCommunicator::MCommunicator(const uint8_t *bind_ip, const uint16_t bind_port)
{
    socket_id = socket(PF_INET, SOCK_DGRAM, 0);
    if (socket_id < 0)
    {
        Logger::elog("Socket Initialization Error");
        return;
    }

    Logger::dlog("Socket Initialized: OK");

    int optval = 1;
    socklen_t optsize = sizeof(optval);
    if (setsockopt(socket_id, SOL_SOCKET, SO_REUSEADDR, &optval, optsize) < 0)
    {
        Logger::elog("Socket address reuse error:");
        close_socket();
        return;
    }
    Logger::dlog("Socket Address Reuse: OK");

    struct sockaddr_in localaddr;
    memset(&localaddr, 0, sizeof(localaddr));
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(bind_port);
    int8_t ret = inet_pton(AF_INET, (const char *)bind_ip, &localaddr.sin_addr);
    if (ret <= 0)
    {
        Logger::elog("Socket Binding with Invalid ip");
        close_socket();
        return;
    }

    if (bind(socket_id, (const sockaddr *)&localaddr, sizeof(localaddr)) < 0)
    {
        Logger::elog("Socket binding error");
        close_socket();
        return;
    }
    Logger::dlog("Socket Binding: OK");
}

MCommunicator::~MCommunicator()
{
    close_socket();
}

void MCommunicator::close_socket()
{
    if (socket_id > -1)
    {
        close(socket_id);
        socket_id = -1;
        Logger::dlog("Socket Close: OK");
    }
}
