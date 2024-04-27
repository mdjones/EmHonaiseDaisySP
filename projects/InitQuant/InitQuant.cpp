#include "daisy_patch_sm.h"
#include "daisysp.h"
#include "QuantizeUtils.cpp"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

DaisyPatchSM hw;

enum AdcChannel {
   rootKnob = 0,
   scaleKnob,
   octaveKnob,
   voltageInput,
   voltageOutput,
   NUM_ADC_CHANNELS
};


void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	//hw.ProcessAllControls();
	for (size_t i = 0; i < size; i++)
	{
		OUT_L[i] = IN_L[i];
		OUT_R[i] = IN_R[i];
	}
}

int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAudio(AudioCallback);

	AdcChannelConfig adc_config[NUM_ADC_CHANNELS];
	hw.adc.Init(adc_config, NUM_ADC_CHANNELS);

	while(1) {
		// To Add CV control as well see: https://github.com/jeremywen/JW-Modules/blob/master/src/Quantizer.cpp#L50

		int rootNote = hw.adc.GetFloat(rootKnob); //This is here so I can add CV control later
		int scale = hw.adc.GetFloat(scaleKnob); //This is here so I can add CV control later
		int octaveShift = hw.adc.GetFloat(octaveKnob); //This is here so I can add CV control later

		float volts = QuantizeUtils::closestVoltageInScale(hw.adc.GetFloat(voltageInput), rootNote, scale);
		//hw.adc.setVoltage(volts + octaveShift);
	}
}
