#include <stdio.h>
#include "../header/VFrameReceiver.h"
#include "../header/VideoFrame.h"
#include "../header/SPCBuffer.h"

#include "../test/Test_Headers.h"

void test_live_video_receiving()
{
    Logger::dlog("Starting Receiver process...");

    const uint8_t *bind_ip = NULL;
    const uint16_t bind_port = MACHINE_PORT;
    const uint8_t *join_multicast_ip = (const uint8_t *)MULTICAST_IP;
    const uint8_t *join_multicast_interface_ip = (const uint8_t *)MACHINE_IP;

    // create shared buffer
    string key = SHARED_KEY;
    int size = SHARED_BUFFER_SIZE;
    SPCBuffer<VideoFrame> buffer(key, size);

    // Define callback to handle each received frame
    auto res = [&buffer](uint8_t *pframe, uint32_t frame_size)
    {
        VideoFrame frame;
        frame.size = frame_size;
        memcpy(frame.data, pframe, frame_size);
        buffer.write(&frame);
    };

    // Set up network receiver
    IVFrameExtractor *ivframe_extractor = new VFrameExtractor(res);
    MReceiver *mreceiver = new MReceiver(bind_ip, bind_port, join_multicast_ip, join_multicast_interface_ip);
    FrameReceiver framereceiver = FrameReceiver(mreceiver, ivframe_extractor);

    // Start listening for frames
    framereceiver.start_listening();

    Logger::dlog("Receiver Process running... Press Ctrl+C to exit.");
}

int main()
{
    // test_Vframe_receiving();
    test_live_video_receiving();
}