#include "daisy_patch_sm.h"
#include "daisysp.h"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

DaisyPatchSM patch;

Switch gate1PatchedSwitch;
Switch gate2PatchedSwitch;
Switch button, ch_toggle;

bool debug = true;

enum ChannelName {
	CH_1,
	CH_2,
	NUM_CHANNELS
};

struct Channel {
	int rootNote;
	int scale;
	int octaveShift;
	float in_volts;
	float out_volts;
	bool trig;
};

Channel channels[NUM_CHANNELS] = {};


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

	ch_toggle.Init(patch.B8);

	if (debug)
	{
		bool wait_for_pc = true;
		patch.StartLog(wait_for_pc);
		patch.PrintLine("Start logging");
	}

	gate1PatchedSwitch.Init(patch.B7);
	gate2PatchedSwitch.Init(patch.B8);

	for (size_t i = 0; i < NUM_CHANNELS; i++) {
		channels[i] = {};
	}

	while (1)
	{

		ch_toggle.Debounce();
		bool ch_toggle_pressed = ch_toggle.Pressed();
		ChannelName editChannel = ch_toggle_pressed ? ChannelName::CH_1 : ChannelName::CH_2;


		gate1PatchedSwitch.Debounce();
		patch.SetLed(!gate1PatchedSwitch.Pressed());

		gate2PatchedSwitch.Debounce();
		patch.SetLed(!gate2PatchedSwitch.Pressed());

		float root_knob = patch.GetAdcValue(CV_1);
		float scale_knob = patch.GetAdcValue(CV_2);
		float octave_knob = patch.GetAdcValue(CV_3);
		float voct_1 = patch.GetAdcValue(CV_5);
		float voct_2 = patch.GetAdcValue(CV_6);

		if(debug){
			patch.PrintLine("Edit Channel: %s",
                         editChannel == ChannelName::CH_1 ? "CH_1" : "CH_2");
		}
	}
}
