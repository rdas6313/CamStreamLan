#include <stdio.h>
#include "../header/VFrameReceiver.h"
#include <opencv2/opencv.hpp>

#include "../test/Test_Headers.h"

void test_Vframe_receiving()
{
    Logger::dlog("Initiating test 1");
    const uint8_t *bind_ip = NULL;
    const uint16_t bind_port = MACHINE_PORT;
    const uint8_t *join_multicast_ip = (const uint8_t *)MULTICAST_IP;
    const uint8_t *join_multicast_interface_ip = (const uint8_t *)MACHINE_IP;

    auto res = [](uint8_t *pframe, uint32_t frame_size)
    {
        Logger::dlog((char *)pframe);
    };

    IVFrameExtractor *ivframe_extractor = new VFrameExtractor(res);
    MReceiver *mreceiver = new MReceiver(bind_ip, bind_port, join_multicast_ip, join_multicast_interface_ip);
    FrameReceiver framereceiver = FrameReceiver(mreceiver, ivframe_extractor);
    framereceiver.start_listening();

    Logger::dlog("End of test 1");
}

void test_live_video_receiving()
{
    Logger::dlog("Starting receiver...");

    const uint8_t *bind_ip = NULL;
    const uint16_t bind_port = MACHINE_PORT;
    const uint8_t *join_multicast_ip = (const uint8_t *)MULTICAST_IP;
    const uint8_t *join_multicast_interface_ip = (const uint8_t *)MACHINE_IP;

    // Create window for display
    cv::namedWindow("Received Video", cv::WINDOW_AUTOSIZE);

    // Define callback to handle each received frame
    auto res = [](uint8_t *pframe, uint32_t frame_size)
    {
        // Convert received buffer into std::vector<uchar>
        vector<uchar> buf(pframe, pframe + frame_size);

        // Decode JPEG back into Mat
        cv::Mat frame = cv::imdecode(buf, cv::IMREAD_COLOR);
        if (frame.empty())
        {
            cerr << "Error: Failed to decode frame\n";
            return;
        }

        // Show received frame
        cv::imshow("Received Video", frame);
        cv::waitKey(10);
        // printf("Test Received Frame size: %d\n", frame_size);
    };

    // Set up network receiver
    IVFrameExtractor *ivframe_extractor = new VFrameExtractor(res);
    MReceiver *mreceiver = new MReceiver(bind_ip, bind_port, join_multicast_ip, join_multicast_interface_ip);
    FrameReceiver framereceiver = FrameReceiver(mreceiver, ivframe_extractor);

    // Start listening for frames
    framereceiver.start_listening();

    Logger::dlog("Receiver running... Press Ctrl+C to exit.");
}

int main()
{
    // test_Vframe_receiving();
    test_live_video_receiving();
}