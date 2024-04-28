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

	hw.StartLog(true);
  	hw.PrintLine("Config complete !!!");

	while(1) {
		// To Add CV control as well see: https://github.com/jeremywen/JW-Modules/blob/master/src/Quantizer.cpp#L50

		int rootNote = hw.adc.GetFloat(CV_1); //This is here so I can add CV control later
		int scale = hw.adc.GetFloat(CV_2); //This is here so I can add CV control later
		int octaveShift = hw.adc.GetFloat(CV_3); //This is here so I can add CV control later

		float in_volts = hw.adc.GetFloat(CV_4);
		hw.PrintLine("in_volts: %f", in_volts);
		float volts = QuantizeUtils::closestVoltageInScale(hw.adc.GetFloat(CV_4), rootNote+0.67, scale);
		float out_volts = volts + octaveShift;
		hw.WriteCvOut(CV_OUT_1, out_volts);
		hw.PrintLine("out_volts: %f", out_volts);
	}
}
