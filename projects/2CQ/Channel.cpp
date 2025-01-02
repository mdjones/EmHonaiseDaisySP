#include "daisy_patch_sm.h"
#include "daisysp.h"
#include "QuantizeUtils.cpp"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

class Channel
{

private:
    DaisyPatchSM &patch;
    Switch gatePatchedSwitch_;
    int in_voct_accessor_;
    int out_voct_accessor_;
    float quant_voct_;
    int channelNum_;
    float out_voct_;
    GateIn gate_in_;
    dsy_gpio gate_out_;

public:
    Channel(DaisyPatchSM &patch) : patch(patch) {};
    ~Channel() {}

    void Init(int channelNum,
              GateIn gate_in,
              dsy_gpio gate_out,
              Pin gate_patched_pin,
              int in_voct_accessor,
              int out_voct_accessor);

    void quantize();
    void trig();
    bool gate_patched();
    GateIn GetGateIn();
    bool quant_voct_changed();
    void set_quant2voct();
    int GetChannelNum();
    float GetVoctOut();

    int rootNote;
    int scale;
    int octaveShift;
};

void Channel::Init(
    int channelNum,
    GateIn gate_in,
    dsy_gpio gate_out,
    Pin gate_patched_pin,
    int in_voct_accessor,
    int out_voct_accessor)
{
    channelNum_ = channelNum;
    gate_in_ = gate_in;
    gate_out_ = gate_out;
    gatePatchedSwitch_.Init(gate_patched_pin);
    in_voct_accessor_ = in_voct_accessor;
    out_voct_accessor_ = out_voct_accessor;

    rootNote = 0;
    scale = 0;
    octaveShift = 0;

    quantize();
    set_quant2voct();
}

int Channel::GetChannelNum()
{
    return channelNum_;
}

void Channel::trig()
{
    /** Set the gate high */
    dsy_gpio_write(&gate_out_, true);

    /** Wait 20 ms */
    patch.Delay(20);

    /** Set the gate low */
    dsy_gpio_write(&gate_out_, false);
}

GateIn Channel::GetGateIn()
{
    return gate_in_;
}

float Channel::GetVoctOut()
{
    return out_voct_;
}

bool Channel::gate_patched()
{
    gatePatchedSwitch_.Debounce();
    // TODO: Revert to !gatePatchedSwitch_.Pressed() when I have the thonk wired
    // up
    //return gatePatchedSwitch_.Pressed();
    return true;
}

/** Quantize any voltage from 0-5 */
void Channel::quantize()
{
    float in_adc = patch.GetAdcValue(in_voct_accessor_);
    float in_voct = QuantizeUtils::rescalefjw(in_adc, 0, 1, 0, 5);
    quant_voct_ = QuantizeUtils::closestVoltageInScale(
        in_voct,
        rootNote,
        scale);

    quant_voct_ += octaveShift;
}

bool Channel::quant_voct_changed()
{
    bool voct_changed = !QuantizeUtils::AlmostEqualRelative(out_voct_, quant_voct_);
    return voct_changed;
}

void Channel::set_quant2voct()
{
    out_voct_ = quant_voct_;
    patch.WriteCvOut(out_voct_accessor_, out_voct_);
}
