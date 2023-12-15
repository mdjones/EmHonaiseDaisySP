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
        send = dry * 0.8f;
        verb.Process(send, send, &wetl, &wetr);

		// Output
		out[0][i] = dry + wetl;
		out[1][i] = dry + wetr;
       }
}

int main(void)
{
    /** Initialize the hardware */
	float samplerate;
    hw.Init();

    /** Start Processing the audio */
    hw.StartAudio(AudioCallback);

	samplerate = hw.AudioSampleRate();
    verb.Init(samplerate);
    verb.SetFeedback(0.99f);
    verb.SetLpFreq(2000.0f);
	
	hw.SetLed(true);


	while(1) {}
}
