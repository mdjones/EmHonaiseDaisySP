#include "daisy_patch_sm.h"
#include "daisysp.h"
#include "QuantizeUtils.cpp"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

DaisyPatchSM hw;

bool debug = false;

//inline int clampijw(int x, int minimum, int maximum) {
//	return std::clamp(x, minimum, maximum);
//}
inline float clampfjw(float x, float minimum, float maximum) {
	return fminf(fmaxf(x, minimum), maximum);
}
inline float rescalefjw(float x, float xMin, float xMax, float yMin, float yMax) {
	return yMin + (x - xMin) / (xMax - xMin) * (yMax - yMin);
}

bool AlmostEqualRelative(float A, float B,
                         float maxRelDiff = 1.0/12)
{
    // Calculate the difference.
    float diff = fabs(A - B);
    A = fabs(A);
    B = fabs(B);
    // Find the largest
    float largest = (B > A) ? B : A;

    if (diff <= largest * maxRelDiff)
        return true;
    return false;
}
void trig(){
		/** Set the gate high */
        dsy_gpio_write(&hw.gate_out_1, true);

        /** Wait 250 ms */
        hw.Delay(20);

        /** Set the gate low */
        dsy_gpio_write(&hw.gate_out_1, false);

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

	float current_volts = 0.0;

	if(debug){
		hw.StartLog(true);
  		hw.PrintLine("Start logging");
	}
	hw.StartAudio(AudioCallback);

	while(1) {
		float root_knob = hw.GetAdcValue(CV_1);
		float scale_knob = hw.GetAdcValue(CV_2);
		float octave_knob = hw.GetAdcValue(CV_3);
		float voct_cv = hw.GetAdcValue(CV_5);

		int rootNote = rescalefjw(root_knob, 0,1,0, QuantizeUtils::NUM_NOTES);
		int scale = rescalefjw(scale_knob, 0,1,0, QuantizeUtils::NUM_SCALES);
		int octaveShift = rescalefjw(octave_knob, 0,1,0,6);
		float in_volts = rescalefjw(voct_cv, 0,1,0, 5);
		//int octaveShift = params[OCTAVE_PARAM].getValue() + clampfjw(inputs[OCTAVE_INPUT].getVoltage(), -5, 5);

		float volts = QuantizeUtils::closestVoltageInScale(
			in_volts, rootNote, scale);
		volts += octaveShift;

		hw.WriteCvOut(CV_OUT_BOTH, volts);

		if ((!AlmostEqualRelative(volts,current_volts)) & (scale != QuantizeUtils::ScaleEnum::NONE)){
			trig();
			current_volts = volts;
		}

		if(debug){
			hw.PrintLine("#######################");	
			hw.PrintLine("root_knob: " FLT_FMT3, FLT_VAR3(root_knob));
			hw.PrintLine("rootNote: %d", rootNote);
			hw.PrintLine("scale_knob: " FLT_FMT3, FLT_VAR3(scale_knob));
			hw.PrintLine("scale:  %d", scale);
			hw.PrintLine("octave_knob: " FLT_FMT3, FLT_VAR3(octave_knob));
			hw.PrintLine("octaveShift: %d", octaveShift);
			hw.PrintLine("voct_cv: " FLT_FMT3, FLT_VAR3(voct_cv));
			hw.PrintLine("out_cv: " FLT_FMT3, FLT_VAR3(volts));
			hw.PrintLine("Note name: %s", QuantizeUtils::noteName(rootNote).c_str());
			hw.PrintLine("Scale name: %s", QuantizeUtils::scaleName(scale).c_str());


			hw.PrintLine("#######################");
			hw.Delay(200);
		}
	}
}


