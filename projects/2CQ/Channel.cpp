#include "daisy_patch_sm.h"
#include "daisysp.h"
#include "QuantizeUtils.cpp"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

class Channel
{

private:
    DaisyPatchSM patch_;
    Switch gatePatchedSwitch_;
    int in_voct_accessor_;
    int out_voct_accessor_;
    float quant_voct_;
    int channelNum_;
    float out_voct_;
    GateIn gate_in_;
    dsy_gpio gate_out_;

public:
    Channel() {}
    ~Channel() {}

    void Init(int channelNum,
              DaisyPatchSM patch,
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

    int rootNote;
    int scale;
    int octaveShift;

};

void Channel::Init(
    int channelNum,
    DaisyPatchSM patch,
    GateIn gate_in,
    dsy_gpio gate_out,
    Pin gate_patched_pin,
    int in_voct_accessor,
    int out_voct_accessor)
{
    channelNum_ = channelNum;
    patch_ = patch;
    gate_in_ = gate_in;
    gate_out_ = gate_out;
    gatePatchedSwitch_.Init(gate_patched_pin);
    in_voct_accessor_ = in_voct_accessor;
    out_voct_accessor_ = out_voct_accessor;

    quantize();
    out_voct_ = quant_voct_;
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
	patch_.Delay(20);

	/** Set the gate low */
	dsy_gpio_write(&gate_out_, false);
}

GateIn Channel::GetGateIn(){
    return gate_in_;
}

bool Channel::gate_patched()
{
    gatePatchedSwitch_.Debounce();
    return !gatePatchedSwitch_.Pressed();
}

void Channel::quantize()
{
    float in_voct = QuantizeUtils::rescalefjw(in_voct_accessor_,
                                              0, 1, 0, 5);
    float quant_voct = QuantizeUtils::closestVoltageInScale(
        in_voct,
        rootNote,
        scale);

    quant_voct += octaveShift;
    patch_.PrintLine("channelNum: %i\n", channelNum_);
    patch_.PrintLine("channelNum: %s\n", channelNum_ == 0 ? "true" : "false");
}

bool Channel::quant_voct_changed()
{
    bool voct_changed = !QuantizeUtils::AlmostEqualRelative(out_voct_, quant_voct_);
    return voct_changed;
}

void Channel::set_quant2voct()
{
    out_voct_ = quant_voct_;
    patch_.WriteCvOut(out_voct_accessor_, out_voct_);
}
