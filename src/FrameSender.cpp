#include "../header/VFrameSender.h"

FrameSender::FrameSender(MSender *msender, IVFrameBuilder *fbuilder)
{
    this->msender = msender;
    this->fbuilder = fbuilder;
    current_frame_no = 0;
}

bool FrameSender::isArgError(const uint8_t *pframe, const uint32_t frame_size)
{
    if (msender == NULL)
    {
        Logger::elog("Multicast sender address is NULL");
        return true;
    }
    else if (fbuilder == NULL)
    {
        Logger::elog("Frame builder address is NULL");
        return true;
    }
    else if (pframe == NULL)
    {
        Logger::elog("Frame address is NULL");
        return true;
    }
    else if (frame_size == 0)
    {
        Logger::elog("Frame size is zero");
        return true;
    }
    return false;
}

bool FrameSender::send(const uint8_t *pframe, const uint32_t frame_size)
{
    if (isArgError(pframe, frame_size))
        return false;
    vector<VFrame> vframes = fbuilder->build(pframe, frame_size, current_frame_no);
    current_frame_no = (current_frame_no + 1) % (MAX_FRAME_NO + 1);
    for (VFrame &vframe : vframes)
    {
        uint32_t bytes_sent = msender->send((uint8_t *)&vframe, sizeof(vframe));
        if (bytes_sent != sizeof(vframe))
        {
            string msg = "FrameSender: VFrame error for frame: " + to_string(vframe.fhdr.frame_no) + " and packet id: " + to_string(vframe.fhdr.packet_id);
            Logger::elog(msg.c_str());
            continue;
        }
        string msg = "FrameSender: VFrame Frame no: " + to_string(vframe.fhdr.frame_no) + " and packet no: " + to_string(vframe.fhdr.packet_id) + " and VFrame size: " + to_string(sizeof(vframe.fhdr) + vframe.fhdr.payload_size) + "! Sent: OK";
        Logger::dlog(msg.c_str());
    }
    return true;
}

FrameSender::~FrameSender()
{
    delete msender;
    delete fbuilder;
    msender = NULL;
    fbuilder = NULL;
}