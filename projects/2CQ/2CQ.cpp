#include "daisy_patch_sm.h"
#include "daisysp.h"
#include "Channel.cpp"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

DaisyPatchSM patch;

// Switch gate1PatchedSwitch;
// Switch gate2PatchedSwitch;
Switch button, ch_toggle;

bool debug = true;

enum ChannelNum
{
	CH_1,
	CH_2,
	NUM_CHANNELS
};

/**struct Channel
{
	int rootNote;
	int scale;
	int octaveShift;
	float in_voct;
	float quant_voct;
	float out_voct;
	bool gate_patched;
	int out_channel;
	GateIn gate_in;
	dsy_gpio gate_out;
};
*/



bool AlmostEqualRelative(float A, float B,
						 float maxRelDiff = 1.0 / 12)
{
	// Calculate the difference.
	float diff = fabs(A - B);
	A = fabs(A);
	B = fabs(B);
	// Find the largest
	float largest = (B > A) ? B : A;

	if (diff <= largest * maxRelDiff)
		return true;
	return false;
}


void SetCurrentChannelEdits(Channel &channel)
{
	float root_knob = patch.GetAdcValue(CV_1);
	float scale_knob = patch.GetAdcValue(CV_2);
	float octave_knob = patch.GetAdcValue(CV_3);

	int rootNote = rescalefjw(root_knob, 0, 1, 0, QuantizeUtils::NUM_NOTES);
	int scale = rescalefjw(scale_knob, 0, 1, 0, QuantizeUtils::NUM_SCALES);
	int octaveShift = QuantizeUtils::rescalefjw(octave_knob, 0, 1, 0, 5);

	patch.PrintLine("rootNote: %i", rootNote);

	channel.rootNote = rootNote;
	channel.scale = scale;
	channel.octaveShift = octaveShift;

	patch.PrintLine("channel[%i].rootNote: %i", channel.GetChannelNum(), channel.rootNote);

}


void AudioCallback(AudioHandle::InputBuffer in,
				   AudioHandle::OutputBuffer out,
				   size_t size)
{
	patch.ProcessAllControls();
	for (size_t i = 0; i < size; i++)
	{
		OUT_L[i] = IN_L[i];
		OUT_R[i] = IN_R[i];
	}
}

int main(void)
{
	patch.Init();
	patch.StartAudio(AudioCallback);
	if (debug)
	{
		bool wait_for_pc = true;
		patch.StartLog(wait_for_pc);
		patch.PrintLine("Start logging");
	}

	Channel channels[NUM_CHANNELS] = {};

	channels[CH_1].Init(
		CV_1,
		patch,
		patch.gate_in_1,
		patch.gate_out_1,
		patch.B7,
		CV_5,
		CV_OUT_1);

	channels[CH_2].Init(
		CV_2,
		patch,
		patch.gate_in_2,
		patch.gate_out_2,
		patch.B8,
		CV_6,
		CV_OUT_2);

	ch_toggle.Init(patch.B8);
	
	
	int cnt = 0;
	while (1)
	{	
		cnt += 1;
		// Set unique channel inputs
		ch_toggle.Debounce();
		bool ch_toggle_pressed = ch_toggle.Pressed();
		ChannelNum ch_num = ch_toggle_pressed ? ChannelNum::CH_1 : ChannelNum::CH_2;
		Channel edit_channel = channels[ch_num];
		SetCurrentChannelEdits(edit_channel);

		
		for (size_t i = 0; i < NUM_CHANNELS - 1; i++)
		{
			channels[i].quantize();
			if (channels[i].scale == QuantizeUtils::ScaleEnum::NONE)
			{
				channels[i].set_quant2voct();
				patch.PrintLine("HERE1LNONE");
			}
			else if (channels[i].gate_patched() && channels[i].GetGateIn().State())
			{
				//requant incase it has changed very recently
				channels[i].quantize(); 
				channels[i].set_quant2voct();
				channels[i].trig();
			}
			else if (!channels[i].gate_patched() && channels[i].quant_voct_changed())
			{
				channels[i].set_quant2voct();
				channels[i].trig();
				patch.PrintLine("HERE1");
			}
			patch.PrintLine("HERE2");
		}
		
		if (debug)
		{
			std::string ecStr = (edit_channel.GetChannelNum() == ChannelNum::CH_1) ? "CH_1" : "CH_2";
			
			//patch.PrintLine("channels[%s] get_patched: %s\n", ecStr.c_str(),
			//				edit_channel.gate_patched ? "True" : "False");
			patch.PrintLine("channels[%s] channelNum: %i\n", ecStr.c_str(), edit_channel.GetChannelNum());
			patch.PrintLine("channels[%s] rootNote: %i\n", ecStr.c_str(), edit_channel.rootNote);
			//patch.PrintLine("channels[%s] in_voct: %i\n", ecStr.c_str(), (int)edit_channel.in_voct*1000);
			//patch.PrintLine("channels[%s] out_voct: %i\n", ecStr.c_str(), (int)edit_channel.out_voct*1000);
			//  patch.PrintLine("channels[%s] editChannel: %i, CH_1: %i", ecStr.c_str(), editChannel, CH_1);
			//  patch.PrintLine("editChannel, == CH_1: %s", editChannel == CH_1 ? "true" : "false");
			//  patch.PrintLine("channels[%s] eq? %s", ecStr.c_str(), channels[CH_1].in_voct == channels[0].in_voct ? "true" : "false");
		//	patch.PrintLine("#######################");

		//	patch.PrintLine("channels[%s] scale: %s", ecStr.c_str(), QuantizeUtils::scaleName(edit_channel.scale).c_str());

			patch.PrintLine("########## %d #############", cnt);

			patch.Delay(200);
		}
	}
}
