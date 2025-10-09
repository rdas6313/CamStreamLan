#include "../header/VFrameReceiver.h"

void FrameReceiver::set_callback(const function<void(uint8_t *, uint32_t)> &callback)
{
    if (ivframe_extractor)
        ivframe_extractor->set_callback(callback);
}

void FrameReceiver::start_listening()
{
    // implement signal handling
    if (mreceiver == NULL)
    {
        Logger::elog("FrameReceiver: Start_listening: mreceiver is NULL");
        return;
    }
    if (ivframe_extractor == NULL)
    {
        Logger::elog("FrameReceiver: Start_listening: ivframe_extractor is NULL");
        return;
    }

    uint8_t buf[MAX_UDP_PACKET_BUFFER_SIZE];
    int32_t bytes_received = 0;
    Logger::dlog("FrameReceiver: Starting listening..");
    while (1)
    {
        bytes_received = mreceiver->receive(buf, MAX_UDP_PACKET_BUFFER_SIZE);
        if (bytes_received < 0)
        {
            break;
        }
        bool success = ivframe_extractor->extract(buf, bytes_received);
        if (!success)
        {
            Logger::elog(
                string("FrameReceiver: Start_listening: Error duing frame extraction! Bytes Received:" + to_string(bytes_received))
                    .c_str());
        }
    }
    Logger::dlog("FrameReceiver: Stoping listening..");
}
FrameReceiver::FrameReceiver(MReceiver *mreceiver, IVFrameExtractor *ivframe_extractor)
{
    this->mreceiver = mreceiver;
    this->ivframe_extractor = ivframe_extractor;
}

FrameReceiver::~FrameReceiver()
{
    if (mreceiver)
    {
        delete mreceiver;
        mreceiver = NULL;
    }
    if (ivframe_extractor)
    {
        delete ivframe_extractor;
        ivframe_extractor = NULL;
    }
    Logger::dlog("FrameReceiver: released all memory!");
}