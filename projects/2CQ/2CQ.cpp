#include "daisy_patch_sm.h"
#include "daisysp.h"
// #include "Channel.cpp"
#include "two_cq_hw.cpp"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;
using namespace two_cq;

DaisyPatchSM hw;
two_cq::TwoCQ twoCQ = two_cq::TwoCQ(hw);

Switch ch_toggle;

bool debug = true;

uint8_t message_idx;
uint8_t oled_edit_indicator;
char symbols[5] = {'-', '\\', '|', '/', '-'};

// To detect any knob adjustments
int cur_rootNote, cur_scale, cur_octaveShift;

enum ChannelNum
{
	CH_1,
	CH_2,
	NUM_CHANNELS
};

Channel channels[NUM_CHANNELS] = {Channel(hw), Channel(hw)};

ChannelNum GetCurrentChannel()
{
	ch_toggle.Debounce();
	bool ch_toggle_pressed = ch_toggle.Pressed();
	return ch_toggle_pressed ? ChannelNum::CH_2 : ChannelNum::CH_1;
}

void UpdateOled(Channel &channel)
{
	// SPI Serial 128 * 64
	// 64/18 = 3.5
	char strbuff[128];
	twoCQ.display.Fill(false);

	twoCQ.display.SetCursor(0, 0);
	strbuff[0] = '\0';
	sprintf(strbuff, "CH_%i    [%c]", channel.GetChannelNum(), symbols[oled_edit_indicator]);
	twoCQ.display.WriteString(strbuff, Font_11x18, true);

	twoCQ.display.SetCursor(0, 20);
	strbuff[0] = '\0';
	sprintf(strbuff, "Rt %s, Oc %i", QuantizeUtils::noteName(channel.rootNote).c_str(), channel.octaveShift);
	twoCQ.display.WriteString(strbuff, Font_11x18, true);

	twoCQ.display.SetCursor(0, 40);
	strbuff[0] = '\0';
	sprintf(strbuff, "Sc %s", QuantizeUtils::scaleName(channel.scale).c_str());
	twoCQ.display.WriteString(strbuff, Font_11x18, true);

	twoCQ.display.Update();
}

bool SetCurrentChannelEdits(Channel &channel)
{
	hw.PrintLine("Edit Channel %d", channel.GetChannelNum());
	bool changed = false;

	int rootNote = twoCQ.GetRootNote();
	int scale = twoCQ.GetScale();
	int octaveShift = twoCQ.GetOctaveShift();

	// Check is a knob has been turned recently and if so update the channel
	if (cur_rootNote != rootNote)
	{
		cur_rootNote = rootNote;
		channel.rootNote = rootNote;
		changed = true;
	}
	if (cur_scale != scale)
	{
		cur_scale = scale;
		channel.scale = scale;
		changed = true;
	}
	if (cur_octaveShift != octaveShift)
	{
		cur_octaveShift = octaveShift;
		channel.octaveShift = octaveShift;
		changed = true;
	}
	return changed;
}

// TODO: I can probably remove this function
void AudioCallback(AudioHandle::InputBuffer in,
				   AudioHandle::OutputBuffer out,
				   size_t size)
{
	hw.ProcessAllControls();
	for (size_t i = 0; i < size; i++)
	{
		//OUT_L[i] = channels[CH_1].GetVoctOut();
		//OUT_R[i] =channels[CH_2].GetVoctOut();
	}
}

int main(void)
{

	hw.Init();
	hw.StartAudio(AudioCallback);

	twoCQ.Init();

	// Save the current values
	cur_rootNote = twoCQ.GetRootNote();
	cur_scale = twoCQ.GetScale();
	cur_octaveShift = twoCQ.GetOctaveShift();

	if (debug)
	{
		bool wait_for_pc = true;
		hw.StartLog(wait_for_pc);
		hw.PrintLine("Start logging");
	}

	channels[CH_1].Init(
		1,
		hw.gate_in_1,
		hw.gate_out_1,
		CH1_GATE_PATCHED,
		CH1_IN_VOCT,
		CH1_OUT_VOCT);

	channels[CH_2].Init(
		2,
		hw.gate_in_2,
		hw.gate_out_2,
		CH2_GATE_PATCHED,
		CH2_IN_VOCT,
		CH2_OUT_VOCT);

	ch_toggle.Init(CH_SELECT);

	int cnt = 0;
	message_idx = 0;
	oled_edit_indicator = 0;

	ChannelNum init_channel_num = GetCurrentChannel();
	while (1)
	{
		cnt += 1;

		// Set channel inputs
		ChannelNum edit_ch_num = GetCurrentChannel();
		if (cnt == 1)
		{
			UpdateOled(channels[edit_ch_num]);
		}

		if (init_channel_num != edit_ch_num)
		{
			UpdateOled(channels[edit_ch_num]);
			init_channel_num = edit_ch_num;
		}

		if (SetCurrentChannelEdits(channels[edit_ch_num]))
		{
			oled_edit_indicator = (oled_edit_indicator + 1) % 5;
			UpdateOled(channels[edit_ch_num]);
		}

		for (size_t i = 0; i < NUM_CHANNELS; i++)
		{
			channels[i].quantize();
			if (channels[i].scale == QuantizeUtils::ScaleEnum::NONE)
			{
				channels[i].set_quant2voct();
			}
			else if (channels[i].gate_patched() && channels[i].GetGateIn().State())
			{
				// requant incase it has changed very recently
				channels[i].quantize();
				channels[i].set_quant2voct();
				channels[i].trig();
			}
			else if (!channels[i].gate_patched() && channels[i].quant_voct_changed())
			{
				channels[i].set_quant2voct();
				channels[i].trig();
			}
		}

		if (debug)
		{
			//hw.PrintLine("~########## %d #############", cnt);
			//hw.PrintLine("Channel Num: %i", edit_ch_num);
			hw.PrintLine("GetVoctOut[%i]: %f", edit_ch_num, channels[edit_ch_num].GetVoctOut());
			hw.PrintLine("%s", edit_ch_num == CH_1 ? "CH_1" : "CH_2");
		}
		message_idx = (message_idx + 1) % 5;
	}
}
