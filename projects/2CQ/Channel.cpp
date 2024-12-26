#include "daisy_patch_sm.h"
#include "daisysp.h"
#include "QuantizeUtils.cpp"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

class Channel
{

private:
    static DaisyPatchSM patch;
    Switch gatePatchedSwitch;
    static int in_voct_accessor;
    float quant_voct;

public:
    Channel() {}
    ~Channel() {}

    void Init(int channelNum,
              DaisyPatchSM patch,
              GateIn gate_in,
              dsy_gpio gate_out,
              Pin gate_patched_pin,
              int in_voct_accessor);

    void quantize();
    bool gate_patched();
    bool quant_voct_changed();
    void set_quant2voct();

    int rootNote;
    int scale;
    int octaveShift;
    float out_voct;
    static GateIn gate_in;
    static dsy_gpio gate_out;
    static int channelNum;
};

void Channel::Init(
    int channelNum,
    DaisyPatchSM patch,
    GateIn gate_in,
    dsy_gpio gate_out,
    Pin gate_patched_pin,
    int in_voct_accessor)
{
    channelNum = channelNum;
    patch = patch;
    gate_in = gate_in;
    gate_out = gate_out;
    gate_patched_pin = gate_patched_pin;
    gatePatchedSwitch.Init(gate_patched_pin);
    in_voct_accessor = in_voct_accessor;

    quantize();
    out_voct = quant_voct;
}

bool Channel::gate_patched()
{
    gatePatchedSwitch.Debounce();
    return !gatePatchedSwitch.Pressed();
}

void Channel::quantize()
{
    float in_voct = QuantizeUtils::rescalefjw(in_voct_accessor,
                                              0, 1, 0, 5);
    float quant_voct = QuantizeUtils::closestVoltageInScale(
        in_voct,
        rootNote,
        scale);

    quant_voct += octaveShift;
}

bool Channel::quant_voct_changed()
{
    bool voct_changed = !QuantizeUtils::AlmostEqualRelative(out_voct, quant_voct);
    return voct_changed;
}

void Channel::set_quant2voct()
{
    out_voct = quant_voct;
}
