#include <stdio.h>
#include "../header/VFrameSender.h"
#include <opencv2/opencv.hpp>

#define MACHINE_PORT 30001
#define MACHINE_IP "192.168.0.179"
#define MULTICAST_PORT 50001
#define MULTICAST_IP "235.0.0.1"

void test_Vframe_sending()
{
    Logger::dlog("Initiating test 1");
    const uint8_t *bind_ip = (const uint8_t *)MACHINE_IP;
    const uint16_t bind_port = MACHINE_PORT;
    const uint16_t multicast_port = MULTICAST_PORT;
    const uint8_t *to_multicast_ip = (const uint8_t *)MULTICAST_IP;

    uint8_t pframe[2000];

    IVFrameBuilder *fbuilder = new VFrameBuilder();
    MSender *msender = new MSender(bind_ip, bind_port, multicast_port, to_multicast_ip);
    FrameSender fsender = FrameSender(msender, fbuilder);

    for (int alpha = 0; alpha < 26; alpha++)
    {
        for (int i = 0; i < 2000; i++)
            pframe[i] = alpha + 'a';
        pframe[1500] = '\0';
        uint16_t data_size = strlen((char *)pframe) + 1;

        bool isSuccess = fsender.send(pframe, data_size);
        if (isSuccess)
            printf("Successfully Sent frame!\n");
        else
            printf("Error while packet sent\n");
    }

    Logger::dlog("End of test 1");
}

void test_sending_live_video_frames()
{

    Logger::dlog("Initiating test 1");
    const uint8_t *bind_ip = (const uint8_t *)MACHINE_IP;
    const uint16_t bind_port = MACHINE_PORT;
    const uint16_t multicast_port = MULTICAST_PORT;
    const uint8_t *to_multicast_ip = (const uint8_t *)MULTICAST_IP;

    IVFrameBuilder *fbuilder = new VFrameBuilder();
    MSender *msender = new MSender(bind_ip, bind_port, multicast_port, to_multicast_ip);
    FrameSender fsender = FrameSender(msender, fbuilder);

    // 1. **CRITICAL FIX: Initialize the windowing system on macOS**
    cv::namedWindow("Captured Frame", cv::WINDOW_AUTOSIZE);

    // 2. Camera Initialization
    // Use the robust AVFoundation backend for macOS
    cv::VideoCapture cap(0, cv::CAP_AVFOUNDATION);

    if (!cap.isOpened())
    {
        std::cerr << "Error: Cannot open camera or device not found.\n";
        // On failure, ensure windows are closed before exit
        cv::destroyAllWindows();
        return;
    }
    cout << "Camera initialized" << endl;

    // 3. Remove the initial cv::waitKey(100). It's no longer necessary and can cause hangs.

    cv::Mat frame;
    int frame_count = 500;
    while (true)
    {
        cout << "Capturing frame" << endl;
        cap >> frame;
        if (frame.empty())
        {
            std::cerr << "Error: Captured empty frame. Exiting.\n";
            break;
        }
        // compressing to jpeg
        std::vector<uchar> buf;
        cv::imencode(".jpg", frame, buf); // compressed frame

        // sending frame
        uint8_t *pbuf = reinterpret_cast<uint8_t *>(buf.data());
        uint32_t buf_size = buf.size();

        bool success = fsender.send(pbuf, buf_size);
        printf("Frame size: %d\n", buf_size);
        if (success)
            printf("Frame sent: Success\n");
        else
            printf("Frame sent: Error\n");

        // // de-comoressing to frame
        // cv::Mat received = cv::imdecode(buf, cv::IMREAD_COLOR);
        // cv::imshow("Video", received);

        // Press 'q' to quit
        frame_count -= 1;
        if (frame_count == 0 || cv::waitKey(100) == 'q')
            break;
    }

    // 4. Cleanup
    cap.release();
    cv::destroyAllWindows();
}

int main()
{
    // test_Vframe_sending();
    test_sending_live_video_frames();
}