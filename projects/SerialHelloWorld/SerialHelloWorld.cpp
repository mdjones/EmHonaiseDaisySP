#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;
using namespace seed;

DaisySeed hw;
Oscillator sineOsc;
Oscillator triOsc;

#define S10 daisy::seed::D9     // SWITCH : ORDERED / AS PLAYED
#define S31 daisy::seed::A1.pin // FADER : ORDERED / AS PLAYED

// Create a GPIO object
GPIO my_switch;
GPIO knob1;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out,
                   size_t size) {

  for (size_t i = 0; i < size; i++) {
    // The oscillator's Process function synthesizes, and
    // returns the next sample.
    float sine_signal = sineOsc.Process();
    float tri_signal = triOsc.Process();
    float nobFade = hw.adc.GetFloat(0);

    if (my_switch.Read()) {
      float signal =
          sine_signal * (1.0f / nobFade) + (sine_signal * tri_signal) * nobFade;
      out[0][i] = signal;
      out[1][i] = signal;
    } else {
      out[0][i] = sine_signal;
      out[1][i] = sine_signal;
    }
  }
}

int main(void) {

  // Initialize the Daisy Seed Hardware
  hw.Init();

  // We initialize the oscillator with the sample rate of the hardware
  // this ensures that the frequency of the Oscillator will be accurate.
  sineOsc.Init(hw.AudioSampleRate());
  sineOsc.SetWaveform(Oscillator::WAVE_SIN);
  triOsc.Init(hw.AudioSampleRate());
  triOsc.SetWaveform(Oscillator::WAVE_POLYBLEP_TRI);

  hw.StartAudio(AudioCallback);

  // Initialize the GPIO object
  my_switch.Init(S10, GPIO::Mode::INPUT, GPIO::Pull::NOPULL);

  AdcChannelConfig adcConfig;
  // Figure out how to map this to the touch IDs
  // Look at the pin diagram
  adcConfig.InitSingle(A0);

  // Initialize the adc with the config we just made
  hw.adc.Init(&adcConfig, 1);
  // Start reading values
  hw.adc.Start();

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

    if (init_switch_state != switch_state) {
      hw.PrintLine("Switch S10 (D9) state: %d\n",
                   static_cast<int>(switch_state));
      init_switch_state = switch_state;
      hw.PrintLine("Fader1 state: [%d]\n", hw.adc.Get(0));
      hw.PrintLine("Fader1 state: [%0.2f]\n", hw.adc.GetFloat(0));
    }
  }
}