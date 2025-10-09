#include "../header/VFrameSender.h"

bool VFrameBuilder::isArgsError(const uint8_t *pframe, const uint32_t frame_size)
{
    if (pframe == NULL)
    {
        Logger::elog("VFrameBuilder: frame address is null");
        return true;
    }
    else if (frame_size == 0)
    {
        Logger::elog("VFrameBuilder: frame size is zero");
        return true;
    }
    return false;
}

vector<VFrame> VFrameBuilder::build(const uint8_t *pframe, const uint32_t frame_size, const uint16_t frame_no)
{
    if (isArgsError(pframe, frame_size))
    {
        Logger::elog("VFrameBuilder: args error");
        return vector<VFrame>();
    }

    uint32_t packet_count = (frame_size / MAX_VFRAME_PAYLOAD_SIZE) + ((frame_size % MAX_VFRAME_PAYLOAD_SIZE) != 0 ? 1 : 0);
    vector<VFrame> vframes;
    uint32_t sent_bytes = 0;

    string msg = "VFrameBuilder: frame no: " + to_string(frame_no) + ", packet_count: " + to_string(packet_count) + ", frame size: " + to_string(frame_size);
    Logger::dlog(msg.c_str());

    for (int i = 0; i < packet_count; i++)
    {
        VFrame frame;
        frame.fhdr.frame_no = frame_no;
        frame.fhdr.packet_count = packet_count;
        frame.fhdr.packet_id = i;
        frame.fhdr.syn = 0;
        if(i == 0)
            frame.fhdr.syn = 1;
        if (i == (packet_count - 1))
            frame.fhdr.payload_size = frame_size - (i * MAX_VFRAME_PAYLOAD_SIZE);
        else
            frame.fhdr.payload_size = MAX_VFRAME_PAYLOAD_SIZE;

        memcpy(frame.payload, pframe + sent_bytes, frame.fhdr.payload_size);
        sent_bytes += frame.fhdr.payload_size;
        vframes.push_back(frame);

        string msg = "VFrameBuilder: Frame no: " + to_string(frame.fhdr.frame_no) + ", packet id: " + to_string(frame.fhdr.packet_id) + ", Payload size: " + to_string(frame.fhdr.payload_size);
        Logger::dlog(msg.c_str());
    }

    return vframes;
}

VFrameBuilder::~VFrameBuilder() {}
