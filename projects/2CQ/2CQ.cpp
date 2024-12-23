#include "daisy_patch_sm.h"
#include "daisysp.h"
#include "QuantizeUtils.cpp"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

DaisyPatchSM patch;

Switch gate1PatchedSwitch;
Switch gate2PatchedSwitch;
Switch button, ch_toggle;

bool debug = false;

enum ChannelNum
{
	CH_1,
	CH_2,
	NUM_CHANNELS
};

struct Channel
{
	int rootNote;
	int scale;
	int octaveShift;
	float in_voct;
	float out_voct;
	bool in_gate;
	bool gate_patched;
	int out_channel;
};

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

	Channel channels[NUM_CHANNELS] = {};
	channels[CH_1].out_channel = CV_OUT_1;
	channels[CH_2].out_channel = CV_OUT_2;

	ch_toggle.Init(patch.B8);

	if (debug)
	{
		bool wait_for_pc = true;
		patch.StartLog(wait_for_pc);
		patch.PrintLine("Start logging");
	}

	gate1PatchedSwitch.Init(patch.B7);
	gate2PatchedSwitch.Init(patch.B8);

	while (1)
	{
		// Set unique channel inputs
		gate1PatchedSwitch.Debounce();
		gate2PatchedSwitch.Debounce();

		float in_voct_1 = patch.GetAdcValue(CV_5);
		float in_voct_2 = patch.GetAdcValue(CV_6);

		channels[ChannelNum::CH_1].in_voct = in_voct_1;
		channels[ChannelNum::CH_2].in_voct = in_voct_2;

		channels[ChannelNum::CH_1].in_gate = false; // These should come from B10 and B9
		channels[ChannelNum::CH_2].in_gate = false;

		channels[ChannelNum::CH_1].gate_patched = !gate1PatchedSwitch.Pressed();
		channels[ChannelNum::CH_2].gate_patched = !gate2PatchedSwitch.Pressed();

		// Set curent edit inputs
		ch_toggle.Debounce();
		bool ch_toggle_pressed = ch_toggle.Pressed();
		ChannelNum editChannel = ch_toggle_pressed ? ChannelNum::CH_1 : ChannelNum::CH_2;
		float root_knob = patch.GetAdcValue(CV_1);
		float scale_knob = patch.GetAdcValue(CV_2);
		float octave_knob = patch.GetAdcValue(CV_3);

		int rootNote = rescalefjw(root_knob, 0, 1, 0, QuantizeUtils::NUM_NOTES);
		int scale = rescalefjw(scale_knob, 0, 1, 0, QuantizeUtils::NUM_SCALES);
		int octaveShift = QuantizeUtils::rescalefjw(octave_knob, 0, 1, 0, 5);
		channels[editChannel].rootNote = rootNote;
		channels[editChannel].scale = scale;
		channels[editChannel].octaveShift = octaveShift;

		// Set quantized voct and send to out
		// For now HW voct out will always track whatever out_voct is set to
		// May want to consider only changing hardware out with a trigger
		for (size_t i = 0; i < NUM_CHANNELS - 1; i++)
		{
			float in_voct = QuantizeUtils::rescalefjw(channels[i].in_voct, 0, 1, 0, 5);
			float out_voct = QuantizeUtils::closestVoltageInScale(
				in_voct, channels[i].rootNote, channels[i].scale);
			out_voct += octaveShift;

			patch.WriteCvOut(channels[i].out_channel, out_voct);
		}

		if (debug)
		{
			std::string ecStr = (editChannel == ChannelNum::CH_1) ? "CH_1" : "CH_2";
			// patch.PrintLine("channels[%s] get_patched: %s\n", ecStr.c_str(),
			//				channels[editChannel].gate_patched ? "True" : "False");
			// patch.PrintLine("channels[%s] rootNote: %d\n", ecStr.c_str(), rootNote);
			// patch.PrintLine("channels[%s] in_voct: %i\n", ecStr.c_str(), (int)channels[editChannel].in_voct*1000);
			// patch.PrintLine("channels[%s] out_voct: %i\n", ecStr.c_str(), (int)channels[editChannel].out_voct*1000);
			// patch.PrintLine("channels[%s] editChannel: %i, CH_1: %i", ecStr.c_str(), editChannel, CH_1);
			// patch.PrintLine("editChannel, == CH_1: %s", editChannel == CH_1 ? "true" : "false");
			// patch.PrintLine("channels[%s] eq? %s", ecStr.c_str(), channels[CH_1].in_voct == channels[0].in_voct ? "true" : "false");
			patch.PrintLine("#######################");

			patch.PrintLine("channels[%s] scale: %s", ecStr.c_str(), QuantizeUtils::scaleName(channels[CH_1].scale).c_str());

			patch.PrintLine("#######################");
			patch.Delay(200);
		}
	}
}
