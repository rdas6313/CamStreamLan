#ifndef _VFRAME_RECEIVER_HEAD
#define _VFRAME_RECEIVER_HEAD

#include <stdint.h>
#include <functional>
#include <string>
#include <utility>
#include <vector>

#include "../header/Communicator.h"
#include "../header/VFrame.h"
#include "../header/Logger.h"

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include <algorithm>

#define MAX_VFRAME_EXTRACTOR_BUFFER_SIZE 6
#define EMPTY_FRAME (MAX_FRAME_NO + 1)

using namespace std;

class IVFrameExtractor
{
public:
    virtual void set_callback(const function<void(uint8_t *, uint32_t)> &callback) = 0;
    virtual bool extract(uint8_t *pvframe_buf, uint32_t vframe_size) = 0;
    virtual ~IVFrameExtractor() = default;
};

class VFrameExtractor : public IVFrameExtractor
{
private:
    function<void(uint8_t *, uint32_t)> callback;
    vector<vector<VFrame>> frame_buf;
    vector<int32_t> cur_frame_in_buf;
    uint16_t last_delivered_frame;

    bool isArgsGood(uint8_t *pvframe_buf, uint32_t vframe_size);
    VFrame *get_vframe(uint8_t *pvframe_buf, uint32_t vframe_size);
    bool isNewPacket(uint16_t old_frame_no, uint16_t new_frame_no);
    pair<uint8_t *, uint32_t> rebuild_frame(vector<VFrame> &vframes);
    void clean_memory(uint8_t *pframe, uint32_t size);
    void reset_state();
    bool got_all_packets(uint16_t packet_count, vector<VFrame> &vframes);

public:
    VFrameExtractor(const function<void(uint8_t *, uint32_t)> &callback);
    void set_callback(const function<void(uint8_t *, uint32_t)> &callback);
    bool extract(uint8_t *pvframe_buf, uint32_t vframe_size);
    ~VFrameExtractor();
};

class FrameReceiver
{
private:
    MReceiver *mreceiver;
    IVFrameExtractor *ivframe_extractor;

public:
    FrameReceiver(MReceiver *mreceiver, IVFrameExtractor *ivframe_extractor);
    void set_callback(const function<void(uint8_t *, uint32_t)> &callback);
    void start_listening();
    ~FrameReceiver();
};

#endif