#include <stdio.h>
#include "../header/Communicator.h"

#define MACHINE_PORT 30000
#define MACHINE_IP "192.168.0.179"
#define MULTICAST_PORT 40000
#define MULTICAST_IP "235.0.0.1"

void test_sending_valid_data()
{
    char data[500] = "Hi! Multicast send working!";
    MSender sender((const uint8_t *)MACHINE_IP, MACHINE_PORT, MULTICAST_PORT, (const uint8_t *)MULTICAST_IP);
    int bytes = sender.send((const uint8_t *)data, sizeof(data));
    if (bytes < 0)
        printf("Send Error\n");
    else
        printf("%d Byte sent\n", bytes);
}

void test_binding_with_invalid_ip()
{
    const char *INVALID_MACHINE_IP = "300.200.100.234";
    char data[500] = "Hi! Multicast send working!";
    MSender sender((const uint8_t *)INVALID_MACHINE_IP, MACHINE_PORT, MULTICAST_PORT, (const uint8_t *)MULTICAST_IP);
    int bytes = sender.send((const uint8_t *)data, sizeof(data));
    if (bytes < 0)
        printf("Send Error\n");
    else
        printf("%d Byte sent\n", bytes);
}

void test_binding_with_invalid_port()
{
    const int INVALID_MACHINE_PORT = 10;
    char data[500] = "Hi! Multicast send working!";
    MSender sender((const uint8_t *)MACHINE_IP, INVALID_MACHINE_PORT, MULTICAST_PORT, (const uint8_t *)MULTICAST_IP);
    int bytes = sender.send((const uint8_t *)data, sizeof(data));
    if (bytes < 0)
        printf("Send Error\n");
    else
        printf("%d Byte sent\n", bytes);
}

void test_sending_with_invalid_multicast_ip()
{
    const char *INVALID_MULTICAST_IP = "300.200.100.30";
    char data[500] = "Hi! Multicast send working!";
    MSender sender((const uint8_t *)MACHINE_IP, MACHINE_PORT, MULTICAST_PORT, (const uint8_t *)INVALID_MULTICAST_IP);
    int bytes = sender.send((const uint8_t *)data, sizeof(data));
    if (bytes < 0)
        printf("Send Error\n");
    else
        printf("%d Byte sent\n", bytes);
}

void test_sending_data_len_greater_than_MAX_LIMIT()
{
    char data[2000] = "Hi! Multicast send working!";
    for (int i = strlen(data); i < 1990; i++)
        data[i] = '.';
    data[1990] = 'E';
    data[1991] = '\0';
    MSender sender((const uint8_t *)MACHINE_IP, MACHINE_PORT, MULTICAST_PORT, (const uint8_t *)MULTICAST_IP);
    int bytes = sender.send((const uint8_t *)data, 1992);
    if (bytes < 0)
        printf("Send Error\n");
    else
        printf("%d Byte sent\n", bytes);
}

int main()
{
    test_sending_valid_data();
    //  test_binding_with_invalid_ip();
    //  test_binding_with_invalid_port();
    //  test_sending_with_invalid_multicast_ip();
    // test_sending_data_len_greater_than_MAX_LIMIT();
    return 0;
}