#include "daisy_patch_sm.h"
#include "daisysp.h"
#include "QuantizeUtils.cpp"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

DaisyPatchSM hw;

//inline int clampijw(int x, int minimum, int maximum) {
//	return std::clamp(x, minimum, maximum);
//}
inline float clampfjw(float x, float minimum, float maximum) {
	return fminf(fmaxf(x, minimum), maximum);
}
inline float rescalefjw(float x, float xMin, float xMax, float yMin, float yMax) {
	return yMin + (x - xMin) / (xMax - xMin) * (yMax - yMin);
}

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

		int rootNote = rescalefjw(root_knob, 0,1,0, QuantizeUtils::NUM_NOTES);
		int scale = rescalefjw(scale_knob, 0,1,0, QuantizeUtils::NUM_SCALES);
		//int octaveShift = params[OCTAVE_PARAM].getValue() + clampfjw(inputs[OCTAVE_INPUT].getVoltage(), -5, 5);

		float volts = QuantizeUtils::closestVoltageInScale(
			voct_cv, rootNote, scale);

		hw.PrintLine("#######################");	
		hw.PrintLine("root_knob: " FLT_FMT3, FLT_VAR3(root_knob));
		hw.PrintLine("rootNote: %d", rootNote);
		hw.PrintLine("scale_knob: " FLT_FMT3, FLT_VAR3(scale_knob));
		hw.PrintLine("scale:  %d", scale);
		hw.PrintLine("octave_knob: " FLT_FMT3, FLT_VAR3(octave_knob));
		hw.PrintLine("voct_cv: " FLT_FMT3, FLT_VAR3(voct_cv));
		hw.PrintLine("out_cv: " FLT_FMT3, FLT_VAR3(volts));
		hw.PrintLine("Note name: %s", QuantizeUtils::noteName(rootNote).c_str());
		hw.PrintLine("Scale name: %s", QuantizeUtils::scaleName(scale).c_str());

		hw.PrintLine("#######################");
		hw.Delay(2000);
		hw.WriteCvOut(CV_OUT_BOTH, volts);
	}
}


