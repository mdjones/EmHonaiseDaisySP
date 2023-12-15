#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;

ReverbSc verb;

void AudioCallback(AudioHandle::InputBuffer in, 
					AudioHandle::OutputBuffer out, 
					size_t size)
{
	float dry, send, wetl, wetr, intensity, wet_ratio; // Effects Vars

	intensity = 0.7f;
	wet_ratio = 0.9999f;

	for (size_t i = 0; i < size; i++)
	{
		// Create Reverb Send
		dry  = in[0][i];
		send = dry * intensity;
		verb.Process(send, send, &wetl, &wetr);
		// Output
		out[0][i] = dry*(1-wet_ratio) + wetl*wet_ratio;
		out[1][i] = dry*(1-wet_ratio) + wetr*wet_ratio;
	}
}

int main(void)
{
    /** Initialize the hardware */
    hw.Init();

    /** Start Processing the audio */
    hw.StartAudio(AudioCallback);

    verb.Init(hw.AudioSampleRate());

	verb.SetFeedback(.8f);
	verb.SetLpFreq(10000.0f);
	
	hw.SetLed(true);
	while(1) {}
}
