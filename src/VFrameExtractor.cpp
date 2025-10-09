#include "../header/VFrameReceiver.h"

VFrameExtractor::VFrameExtractor(const function<void(uint8_t *, uint32_t)> &callback)
{
    set_callback(callback);
    frame_buf = vector<vector<VFrame>>(MAX_VFRAME_EXTRACTOR_BUFFER_SIZE, vector<VFrame>());
    cur_frame_in_buf = vector<int32_t>(MAX_VFRAME_EXTRACTOR_BUFFER_SIZE, EMPTY_FRAME);
}
void VFrameExtractor::set_callback(const function<void(uint8_t *, uint32_t)> &callback)
{
    if (callback)
        this->callback = callback;
}

bool VFrameExtractor::isArgsGood(uint8_t *pvframe_buf, uint32_t vframe_size)
{
    if (pvframe_buf == NULL)
    {
        Logger::elog("VFrameExtractor: IsArgsGood: pointer of vframe buffer is empty");
        return false;
    }
    else if (vframe_size == 0)
    {
        Logger::elog("VFrameExtractor: IsArgsGood: vframe size is zero");
        return false;
    }
    return true;
}

bool VFrameExtractor::isNewPacket(uint16_t old_frame_no, uint16_t new_frame_no)
{
    bool isNewerThanLastDelivered = ((uint16_t)(new_frame_no - last_delivered_frame) < HALF_OF_MAX_FRAME_NO);
    bool isNewerThanOldFrame = ((uint16_t)(new_frame_no - old_frame_no) < HALF_OF_MAX_FRAME_NO);
    if (old_frame_no == EMPTY_FRAME)
        return isNewerThanLastDelivered;
    else
        return isNewerThanLastDelivered && isNewerThanOldFrame;
}

VFrame *VFrameExtractor::get_vframe(uint8_t *pvframe_buf, uint32_t vframe_size)
{
    VFrame *pvframe = (VFrame *)pvframe_buf;
    if (pvframe == NULL)
    {
        Logger::elog("GET_VFRAME: pointer of vframe is NULL");
        return NULL;
    }
    else if (vframe_size != sizeof(VFrame))
    {
        Logger::elog("GET_VFRAME: Not received the full vframe");
        return NULL;
    }
    return pvframe;
}

pair<uint8_t *, uint32_t> VFrameExtractor::rebuild_frame(vector<VFrame> &vframes)
{
    if (vframes.size() == 0)
    {
        Logger::elog("VFrameExtractor: Rebuild_frame: Empty payload");
        return {NULL, 0};
    }

    uint32_t frame_size = 0;
    for (VFrame &vf : vframes)
    {
        frame_size += vf.fhdr.payload_size;
    }

    const uint8_t *pBuf = new uint8_t[frame_size];
    uint32_t cur_size = 0;
    uint16_t frame_no = vframes[0].fhdr.frame_no;
    for (uint32_t i = 0; i < vframes.size(); i++)
    {
        for (VFrame &vf : vframes)
        {
            if (vf.fhdr.packet_id == i)
            {
                memcpy((void *)(pBuf + cur_size), vf.payload, vf.fhdr.payload_size);
                cur_size += vf.fhdr.payload_size;
                break;
            }
        }
    }
    string msg = "VFrameExtractor: Rebuild_frame: Created frame no: " + to_string(frame_no) + " frame size: " + to_string(cur_size);
    Logger::dlog(msg.c_str());
    return {(uint8_t *)pBuf, cur_size};
}

void VFrameExtractor::clean_memory(uint8_t *pframe, uint32_t size)
{
    if (pframe == NULL)
        return;
    delete[] pframe;
    Logger::dlog("VFrameExtractor: cleared memory for frame buffer!");
}

void VFrameExtractor::reset_state()
{
    last_delivered_frame = EMPTY_FRAME;
    fill(cur_frame_in_buf.begin(), cur_frame_in_buf.end(), EMPTY_FRAME);
    for (vector<VFrame> &buf : frame_buf)
    {
        buf.clear();
    }
}

bool VFrameExtractor::got_all_packets(uint16_t packet_count, vector<VFrame> &vframes)
{
    for (uint16_t i = 0; i < packet_count; i++)
    {
        bool found = false;
        for (VFrame &vframe : vframes)
        {
            if (vframe.fhdr.packet_id == i)
            {
                found = true;
                break;
            }
        }
        if (!found)
            return false;
    }
    return true;
}

bool VFrameExtractor::extract(uint8_t *pvframe_buf, uint32_t vframe_size)
{
    if (!isArgsGood(pvframe_buf, vframe_size))
        return false;

    // get vframe
    VFrame *vframe = get_vframe(pvframe_buf, vframe_size);
    if (!vframe)
        return false;
    uint16_t frame_no = vframe->fhdr.frame_no;
    uint16_t packet_count = vframe->fhdr.packet_count;
    uint16_t frame_buf_index = frame_no % MAX_VFRAME_EXTRACTOR_BUFFER_SIZE;
    uint16_t packet_id = vframe->fhdr.packet_id;
    uint8_t syn = vframe->fhdr.syn;

    if (syn == 1)
    {
        reset_state();
    }

    // handle wrap around
    if (!isNewPacket(cur_frame_in_buf[frame_buf_index], frame_no))
    {

        Logger::elog(string("VFrameExtractor: Old packet received! Frame no: " + to_string(frame_no) + " Packet no: " + to_string(packet_id)).c_str());
        return false;
    }

    string msg = "VFrameExtractor: Got new Frame! Frame no: " + to_string(frame_no) + " packet id: " + to_string(packet_id);
    Logger::dlog(msg.c_str());

    if (cur_frame_in_buf[frame_buf_index] == frame_no)
    {
        frame_buf[frame_buf_index].push_back(*vframe);
    }
    else
    {
        frame_buf[frame_buf_index].clear();
        frame_buf[frame_buf_index].push_back(*vframe);
        cur_frame_in_buf[frame_buf_index] = frame_no;
    }

    if (packet_count == frame_buf[frame_buf_index].size() && got_all_packets(packet_count, frame_buf[frame_buf_index]))
    {
        // get payload and concat to make a frame and then do callback

        string msg = "VFrameExtractor: Extract: Got Full frame for Frame no: " + to_string(frame_no);
        Logger::dlog(msg.c_str());

        pair<uint8_t *, uint32_t> data = rebuild_frame(frame_buf[frame_buf_index]);

        cur_frame_in_buf[frame_buf_index] = EMPTY_FRAME;
        frame_buf[frame_buf_index].clear();
        last_delivered_frame = frame_no;

        if (callback)
            callback(data.first, data.second);

        clean_memory(data.first, data.second);
    }
    return true;
}

VFrameExtractor::~VFrameExtractor() = default;