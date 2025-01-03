

#include <stdio.h>
#include <string.h>
#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"
#include "daisy_patch_sm.h"

using namespace daisy;
using namespace patch_sm;
using MyOledDisplay = OledDisplay<SSD130x4WireSpi128x64Driver>;

DaisyPatchSM hw;
MyOledDisplay display;

int main(void)
{

	bool led_state;
	led_state = true;

	uint8_t message_idx;
	hw.Init();

	/** Configure the Display */
	MyOledDisplay::Config disp_cfg;

	disp_cfg.driver_config.transport_config.pin_config.reset = DaisyPatchSM::D3;
	// OLED PIN DC
	disp_cfg.driver_config.transport_config.pin_config.dc = DaisyPatchSM::D8;

	/** And Initialize */
	display.Init(disp_cfg);

	message_idx = 0;
	char strbuff[128];
	while (1)
	{
		System::Delay(500);
		switch (message_idx)
		{
		case 0:
			sprintf(strbuff, "Testing. . .");
			break;
		case 1:
			sprintf(strbuff, "Daisy. . .");
			break;
		case 2:
			sprintf(strbuff, "1. . .");
			break;
		case 3:
			sprintf(strbuff, "2. . .");
			break;
		case 4:
			sprintf(strbuff, "3. . .");
			break;
		default:
			break;
		}
		message_idx = (message_idx + 1) % 5;
		display.Fill(false);
		display.SetCursor(0, 0);
		display.WriteString(strbuff, Font_11x18, true);
		display.Update();

		// Set the onboard LED
		hw.SetLed(led_state);

		// Toggle the LED state for the next time around.
		led_state = !led_state;
	}
}
