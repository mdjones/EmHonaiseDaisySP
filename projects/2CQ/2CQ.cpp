#include "daisy_patch_sm.h"
#include "daisysp.h"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

DaisyPatchSM patch;

Switch gate1PatchedSwitch;
Switch gate2PatchedSwitch;

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

	// Create a GPIO object
	gate1PatchedSwitch.Init(patch.B7);
	gate2PatchedSwitch.Init(patch.B8);

	while (1)
	{
		/** Debounce the switch */
		gate1PatchedSwitch.Debounce();
		patch.SetLed(!gate1PatchedSwitch.Pressed());

		gate2PatchedSwitch.Debounce();
		patch.SetLed(!gate2PatchedSwitch.Pressed());
	}
}
