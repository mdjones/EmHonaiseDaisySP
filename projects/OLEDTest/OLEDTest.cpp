//Configured based on
//https://github.com/electro-smith/libDaisy/blob/master/examples/OLED_SPI/OledSPI.cpp

#include <stdio.h>
#include <string.h>
#include "dev/oled_ssd130x.h"
#include "daisy_patch_sm.h"

using namespace daisy;
using namespace daisy::patch_sm;

DaisyPatchSM hw;

using MyDisplay = OledDisplay<SSD130x4WireSpi128x64Driver>;
MyDisplay display;

void AudioCallback(AudioHandle::InputBuffer in,
                   AudioHandle::OutputBuffer out,
                   size_t size)
{
    //
}

int main(void)
{

	bool led_state;
	led_state = true;

	uint8_t message_idx;
	hw.Init();
	hw.StartAudio(AudioCallback);

	/** Configure the Display */
	MyDisplay::Config display_config;

    SpiHandle::Config& spi_conf = display_config.driver_config.transport_config.spi_config;

    spi_conf.mode = SpiHandle::Config::Mode::MASTER;             // we're in charge
    spi_conf.periph = SpiHandle::Config::Peripheral::SPI_2;      // Use the SPI_2 Peripheral
    spi_conf.direction = SpiHandle::Config::Direction::ONE_LINE; // TWO_LINES_TX_ONLY;

    spi_conf.datasize = 8;
    spi_conf.clock_polarity = SpiHandle::Config::ClockPolarity::LOW;
    spi_conf.clock_phase = SpiHandle::Config::ClockPhase::ONE_EDGE;
    // spi_conf.nss = SpiHandle::Config::NSS::HARD_OUTPUT;
    spi_conf.baud_prescaler = SpiHandle::Config::BaudPrescaler::PS_128;

    // Pins to use. These must be available on the selected peripheral
    spi_conf.pin_config.sclk = DaisyPatchSM::D10; // Use pin D10 as SCLK
    spi_conf.pin_config.miso = Pin();             // We won't need this
    spi_conf.pin_config.mosi = DaisyPatchSM::D9;  // Use D9 as MOSI
    spi_conf.pin_config.nss = Pin();              // DaisyPatchSM::D1;   // use D1 as NSS

    // data will flow from master to slave over just the MOSI line

    // The master will output on the NSS line
    spi_conf.nss = SpiHandle::Config::NSS::SOFT;

    display_config.driver_config.transport_config.pin_config.dc
        = DaisyPatchSM::D2;
    display_config.driver_config.transport_config.pin_config.reset
        = DaisyPatchSM::D3;
    display.Init(display_config);


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
