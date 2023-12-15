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
	float dry, send, wetl, wetr; // Effects Vars

	for (size_t i = 0; i < size; i++)
	{
		// Create Reverb Send
        dry  = in[0][i];
        send = dry * 0.6f;
        verb.Process(send, send, &wetl, &wetr);

		// Output
		//out[0][i] = dry + wetl;
		//out[1][i] = dry + wetr;

		out[0][i] = in[0][i];
		out[1][i] = in[1][i];
	}
}

int main(void)
{

	
	hw.Init();
	
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAudio(AudioCallback);

	float samplerate;
    samplerate = hw.AudioSampleRate();
	verb.Init(samplerate);
    verb.SetFeedback(0.85f);
    verb.SetLpFreq(2000.0f);

	hw.SetLed(true);


	while(1) {}
}
