#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;

DaisySeed hw;
daisysp::Oscillator osc;

#define S10 daisy::seed::D9 // SWITCH : ORDERED / AS PLAYED

void AudioCallback(AudioHandle::InputBuffer in, 
                AudioHandle::OutputBuffer out, 
                size_t size) 
{
    for (size_t i = 0; i < size; i++)
    {
        // The oscillator's Process function synthesizes, and
        // returns the next sample.
        float sine_signal = osc.Process();
        out[0][i] = sine_signal;
        out[1][i] = sine_signal;
    }
}

int main(void) {

  // Initialize the Daisy Seed Hardware
  hw.Init();

    // We initialize the oscillator with the sample rate of the hardware
    // this ensures that the frequency of the Oscillator will be accurate.
    osc.Init(hw.AudioSampleRate());
    hw.StartAudio(AudioCallback);

  // Create a GPIO object
  GPIO my_switch;

  // Initialize the GPIO object
  my_switch.Init(S10, GPIO::Mode::INPUT, GPIO::Pull::NOPULL);

  // Enable Logging, and set up the USB connection.
  hw.StartLog(true);

  // And Print Hello World!
  hw.PrintLine("Hello World. You all bitches!!!!!!!");

  bool init_switch_state = my_switch.Read();
  while (1) {
    // And let's store the state of the button in a variable called
    // "button_state"
    bool switch_state = my_switch.Read();
    hw.SetLed(switch_state);
	if (init_switch_state != switch_state){
		hw.PrintLine("Switch S10 (D9) state: %d\n", static_cast<int>(switch_state));
		init_switch_state = switch_state;
	}
  }
}