#include "daisy_patch_sm.h"
#include "daisysp.h"
#include "QuantizeUtils.cpp"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

DaisyPatchSM hw;


void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	hw.ProcessAllControls();

	for (size_t i = 0; i < size; i++)
	{
		OUT_L[i] = IN_L[i];
		OUT_R[i] = IN_R[i];
	}
}

int main(void)
{
	hw.Init();

	hw.StartLog(true);
  	hw.PrintLine("Start logging");
	hw.StartAudio(AudioCallback);

	while(1) {
		float root_knob = hw.GetAdcValue(CV_1);
		float scale_knob = hw.GetAdcValue(CV_2);
		float octave_knob = hw.GetAdcValue(CV_3);
		float voct_cv = hw.GetAdcValue(CV_5);

		float volts = QuantizeUtils::closestVoltageInScale(
			hw.adc.GetFloat(CV_5), root_knob+0.67, scale_knob);

		hw.PrintLine("#######################");	
		hw.PrintLine("root_knob: " FLT_FMT3, FLT_VAR3(root_knob));
		hw.PrintLine("scale_knob: " FLT_FMT3, FLT_VAR3(scale_knob));
		hw.PrintLine("octave_knob: " FLT_FMT3, FLT_VAR3(octave_knob));
		hw.PrintLine("voct_cv: " FLT_FMT3, FLT_VAR3(voct_cv));
		hw.PrintLine("out_cv: " FLT_FMT3, FLT_VAR3(volts));
		hw.PrintLine("#######################");
		hw.Delay(2000);
	}
}
