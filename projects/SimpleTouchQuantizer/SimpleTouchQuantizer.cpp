#include "daisy_seed.h"
#include "daisysp.h"
#include "theory.h"

using namespace daisy;
using namespace daisysp;
using namespace ev_theory;

DaisySeed hw;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{

	hw->ProcessAnalogControls();
    hw->ProcessDigitalControls();

	// Read v/oct from CTRL 4
    float ctrl = hw->GetKnobValue((DaisySeed::Ctrl)3);
    uint8_t i = static_cast<uint8_t>(std::round(ctrl*60.f) + inTune);
    debugStr = std::to_string(i);

    i = quantizeNoteToRange(i);

	quantizeDacValToRange(3);
	for (size_t i = 0; i < size; i++)
	{
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
	while(1) {}
}
