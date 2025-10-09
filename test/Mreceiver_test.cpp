#include <stdio.h>
#include "../header/Communicator.h"

#define MACHINE_PORT 30000
#define MACHINE_IP "192.168.0.179"
#define MULTICAST_PORT 40000
#define MULTICAST_IP "235.0.0.1"

void test_data_receiving()
{
    char buf[MReceiver::MAX_BUF_SIZE];
    MReceiver receiver((const uint8_t *)NULL, MULTICAST_PORT, (const uint8_t *)MULTICAST_IP, (const uint8_t *)MACHINE_IP);
    printf("Waiting for data...\n");
    int bytes = receiver.receive((unsigned char *const)buf, MReceiver::MAX_BUF_SIZE);
    if (bytes < 0)
    {
        printf("Receive Error!\n");
        return;
    }
    printf("Received data: %s\n", buf);
}

int main()
{
    test_data_receiving();
    return 0;
}