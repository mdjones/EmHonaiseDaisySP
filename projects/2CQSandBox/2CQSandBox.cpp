#include "daisy_patch_sm.h"
#include "daisysp.h"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

DaisyPatchSM patch;

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
  	Switch button;
	button.Init(patch.B7);


	patch.SetAudioBlockSize(4); // number of samples handled per callback
	patch.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	patch.StartAudio(AudioCallback);
	while (1)
	{
		/** Debounce the switch */
        button.Debounce();

        /** Get the current button state */
        bool state = !button.Pressed();

        /** Set the onboard led to the current state */
        patch.SetLed(state);
	}
}
