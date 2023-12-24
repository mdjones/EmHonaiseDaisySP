#include "daisy_seed.h"
 
using namespace daisy;
 
DaisySeed hw;
 
int main(void) {
 
  // Initialize the Daisy Seed Hardware
  hw.Init();
 
  // Enable Logging, and set up the USB connection.
  hw.StartLog(true);
 
  // And Print Hello World!
  hw.PrintLine("Hello World. You all bitches!!!!!!!");
 
  while(1) {}
}