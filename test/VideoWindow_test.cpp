#include <stdio.h>
#include <vector>
#include <opencv2/opencv.hpp>
#include "../header/VideoFrame.h"
#include "../header/SPCBuffer.h"
#include "../header/Logger.h"

#include "../test/Test_Headers.h"

using namespace std;

void test_live_video_receiving()
{
    Logger::dlog("Starting VideoFrame Showing Process...");

    // Create window for display
    cv::namedWindow("Received Video", cv::WINDOW_AUTOSIZE);

    // create shared buffer
    string key = SHARED_KEY;
    int size = SHARED_BUFFER_SIZE;
    SPCBuffer<VideoFrame> buffer(key, size);

    while (1)
    {
        std::optional<VideoFrame> received_frame = buffer.read();
        if (!received_frame)
        {
            cerr << "Error: buffer read error\n";
            break;
        }
        VideoFrame video_frame = *received_frame;
        // Convert received buffer into std::vector<uchar>
        vector<uchar> buf(video_frame.data, video_frame.data + video_frame.size);

        // Decode JPEG back into Mat
        cv::Mat frame = cv::imdecode(buf, cv::IMREAD_COLOR);
        if (frame.empty())
        {
            cerr << "Error: Failed to decode frame\n";
            return;
        }

        // Show received frame
        cv::imshow("Received Video", frame);
        cv::waitKey(1);
        // printf("Test Received Frame size: %d\n", frame_size);
    }

    Logger::dlog("VideoFrame Showing Process running... Press Ctrl+C to exit.");
}

int main()
{
    // test_Vframe_receiving();
    test_live_video_receiving();
}