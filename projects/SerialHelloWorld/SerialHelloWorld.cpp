#include "daisy_seed.h"

using namespace daisy;

DaisySeed hw;

#define S10 daisy::seed::D9 // SWITCH : ORDERED / AS PLAYED

int main(void) {

  // Initialize the Daisy Seed Hardware
  hw.Init();

  // Create a GPIO object
  GPIO my_switch;

  // Initialize the GPIO object
  my_switch.Init(S10, GPIO::Mode::INPUT, GPIO::Pull::PULLUP);

  // Enable Logging, and set up the USB connection.
  hw.StartLog(true);

  // And Print Hello World!
  hw.PrintLine("Hello World. You all bitches!!!!!!!");

  while (1) {
    // And let's store the state of the button in a variable called
    // "button_state"
    bool switch_state = my_switch.Read();
    hw.SetLed(switch_state);
  }
}