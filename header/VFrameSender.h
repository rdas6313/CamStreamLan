#ifndef _VFRAME_HEADER
#define _VFRAME_HEADER
#include <stdint.h>
#include <functional>
#include <algorithm>
#include "../header/Communicator.h"
#include "../header/VFrame.h"

using namespace std;
class IVFrameBuilder
{
public:
    virtual vector<VFrame> build(const uint8_t *pframe, uint32_t frame_size, uint16_t frame_no) = 0;
    virtual ~IVFrameBuilder() {}
};

class VFrameBuilder : public IVFrameBuilder
{
private:
    bool isArgsError(const uint8_t *pframe, const uint32_t frame_size);

public:
    vector<VFrame> build(const uint8_t *pframe, const uint32_t frame_size, const uint16_t frame_no);
    ~VFrameBuilder();
};

class FrameSender
{
private:
    MSender *msender;
    IVFrameBuilder *fbuilder;
    uint16_t current_frame_no;
    bool isArgError(const uint8_t *pframe, const uint32_t frame_size);

public:
    FrameSender(MSender *msender, IVFrameBuilder *fbuilder);
    bool send(const uint8_t *pframe, const uint32_t frame_size);
    virtual ~FrameSender();
};

#endif