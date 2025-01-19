#include "daisy_patch_sm.h"
#include "daisysp.h"
#include "two_cq_hw.cpp"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;
using namespace two_cq;

DaisyPatchSM hw;
two_cq::TwoCQ twoCQ = two_cq::TwoCQ(hw);

Switch ch_select, ch_reset;

bool debug = false;

uint8_t message_idx;

// To detect any knob adjustments
int cur_rootNote, cur_scale, cur_octaveShift, cur_mask;

enum ChannelNum
{
	CH_1,
	CH_2,
	NUM_CHANNELS
};

Channel channels[NUM_CHANNELS] = {Channel(hw), Channel(hw)};

bool cur_gate_patch[NUM_CHANNELS] = {false, false};

ChannelNum GetCurrentChannel()
{
	return ch_select.Pressed() ? ChannelNum::CH_2 : ChannelNum::CH_1;
}

void Splash()
{
	std::string progress = "";
	for (int i=0; i<5; i++)
	{
		progress += ".";
		twoCQ.display.Fill(false);
		twoCQ.display.SetCursor(0, 0);
		twoCQ.display.WriteString("O2CQ", Font_16x26, true);
		twoCQ.display.SetCursor(0, 35);
		twoCQ.display.WriteString(progress.c_str(), Font_16x26, true);
		twoCQ.display.Update();
		hw.Delay(75);
	}
	twoCQ.display.Fill(false);
	twoCQ.display.SetCursor(0, 0);
	twoCQ.display.WriteString("O2CQ", Font_16x26, true);
	twoCQ.display.SetCursor(0, 35);
	twoCQ.display.WriteString("by EmHonaise", Font_11x18, true);
	twoCQ.display.Update();
	hw.Delay(500);
}

void UpdateOled(Channel &channel)
{
	// SPI Serial 128 * 64
	// 64/18 = 3.5
	char strbuff[128];
	twoCQ.display.Fill(false);

	int ch_num = channel.GetChannelNum();
	std::string note = QuantizeUtils::noteName(channel.rootNote) + std::to_string(channel.octaveShift);
	note = QuantizeUtils::PadString(note, 3);
	std::string scale = QuantizeUtils::scaleName(channel.scale);
	std::string mask = QuantizeUtils::maskName(channel.mask, channel.scale);
	std::string patched = channel.gate_patched() ? "GIN" : "";

	twoCQ.display.SetCursor(0, 0);
	strbuff[0] = '\0';
	sprintf(strbuff, "CH%i %s %s", ch_num, note.c_str(), patched.c_str());
	twoCQ.display.WriteString(strbuff, Font_11x18, true);

	twoCQ.display.SetCursor(0, 20);
	strbuff[0] = '\0';
	sprintf(strbuff, "%s", scale.c_str());
	twoCQ.display.WriteString(strbuff, Font_11x18, true);

	twoCQ.display.SetCursor(0, 40);
	strbuff[0] = '\0';
	sprintf(strbuff, "%s", mask.c_str());
	twoCQ.display.WriteString(strbuff, Font_11x18, true);

	twoCQ.display.Update();
}

bool SetCurrentChannelEdits(Channel &channel, bool force = false)
{
	bool changed = false;

	int rootNote = twoCQ.GetRootNote();
	int scale = twoCQ.GetScale();
	int octaveShift = twoCQ.GetOctaveShift();
	int mask = twoCQ.GetScaleMask();

	// Check is a knob has been turned recently and if so update the channel
	if(cur_gate_patch[channel.GetChannelNum()] != channel.gate_patched()){
		cur_gate_patch[channel.GetChannelNum()] = channel.gate_patched();
		changed = true;
	}
	if ((cur_rootNote != rootNote) || force)
	{
		cur_rootNote = rootNote;
		channel.rootNote = rootNote;
		changed = true;
	}
	if ((cur_scale != scale) || force)
	{
		cur_scale = scale;
		channel.scale = scale;
		changed = true;
	}
	if ((cur_octaveShift != octaveShift) || force)
	{
		cur_octaveShift = octaveShift;
		channel.octaveShift = octaveShift;
		changed = true;
	}
	if ((cur_mask != mask) || force)
	{
		cur_mask = mask;
		channel.mask = mask;
		changed = true;
	}
	return changed;
}

/**
 * 
 * patch.Init() uses CV_OUT_2 for an LED
 * So I am doing a hack and sending these to
 * OUT_L[i] and OUT_R[i] too. Will not need this
 * For a pure patch_sm implementation. Maybe use
 * for random quantized voltage.
 * I expected `hw.audio.SetPostGain(-0.1f);` to work to invert the audio cignal
 * but it did not.
 * See: https://github.com/electro-smith/libDaisy/issues/575
 * So I am doing it manually here.
 * OUT_L is not a perfect match to CV_OUT_1 but it's close enough for
 * functional testing but will be a little flat or sharp.
 */
void AudioCallback(AudioHandle::InputBuffer in,
				   AudioHandle::OutputBuffer out,
				   size_t size)
{
	hw.ProcessAllControls();
	ch_reset.Debounce();
	ch_select.Debounce();
	float gain_adj = -1.0f / 10.0f;
	float min = 5.0f * gain_adj;
	for (size_t i = 0; i < size; i++)
	{
		OUT_L[i] = std::max(channels[CH_1].GetVoctOut() * gain_adj, min);
		OUT_R[i] = std::max(channels[CH_2].GetVoctOut() * gain_adj, min);
	}
}

int main(void)
{

	hw.Init();
	hw.StartAudio(AudioCallback);

	twoCQ.Init();

	

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
		CV_IN_1,
		CV_OUT_2);

	channels[CH_2].Init(
		2,
		hw.gate_in_2,
		hw.gate_out_2,
		CH2_GATE_PATCHED,
		CV_IN_1,
		CV_OUT_2);

	ch_reset.Init(CH_RESET, hw.AudioCallbackRate());
	ch_select.Init(CH_SELECT, hw.AudioCallbackRate());

	Splash();

	int cnt = 0;
	message_idx = 0;

	ChannelNum init_channel_num = GetCurrentChannel();
	hw.ProcessAllControls();
	// Save the current values
	cur_rootNote = twoCQ.GetRootNote();
	cur_scale = twoCQ.GetScale();
	cur_octaveShift = twoCQ.GetOctaveShift();
	cur_mask = twoCQ.GetScaleMask();

	SetCurrentChannelEdits(channels[init_channel_num], true);
	UpdateOled(channels[init_channel_num]);

	while (1)
	{
		cnt += 1;

		// Set channel inputs
		ChannelNum edit_ch_num = GetCurrentChannel();

		if (init_channel_num != edit_ch_num)
		{
			UpdateOled(channels[edit_ch_num]);
			init_channel_num = edit_ch_num;
		}

		//hw.SetLed(ch_select.Pressed());

		if (SetCurrentChannelEdits(channels[edit_ch_num], ch_reset.Pressed()))
		{
			if(ch_reset.Pressed())
			{
				twoCQ.Init();
			}
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

			// hw.PrintLine("Channel Num: %i", edit_ch_num);
			// hw.PrintLine("GetVoctOut[%i]: %f", edit_ch_num, channels[edit_ch_num].GetVoctOut());
			// hw.PrintLine("%s", edit_ch_num == CH_1 ? "CH_1" : "CH_2");
		}
		message_idx = (message_idx + 1) % 5;
	}
}
