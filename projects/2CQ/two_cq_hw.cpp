
/**
 * Daisy Patch SM based 2 Channel Quantizer
 * Display is HiLetgo 0.96" inch SPI Serial 128 * 64
 * 12864 Characters OLED LCD Display 
 * SSD1306 for 51 STM32 Arduino Font Color White 
 * 
 * Configured based on
 * https://github.com/electro-smith/libDaisy/blob/master/examples/OLED_SPI/OledSPI.cpp
 *  
 */
#pragma once

#include <stdio.h>
#include <string.h>
#include "dev/oled_ssd130x.h"
#include "daisy_patch_sm.h"

using namespace daisy;
using namespace daisy::patch_sm;

namespace two_cq
{

    class TwoCQ
    {
    public:
        TwoCQ() {}
        ~TwoCQ() {}

        void Init()
        {
            Display::Config display_config;

            SpiHandle::Config &spi_conf = display_config.driver_config.transport_config.spi_config;

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

            // data will flow from master
            // The master will output on the NSS line
            spi_conf.nss = SpiHandle::Config::NSS::SOFT;

            display_config.driver_config.transport_config.pin_config.dc = DaisyPatchSM::D2;
            display_config.driver_config.transport_config.pin_config.reset = DaisyPatchSM::D3;
            display.Init(display_config);
        }

        using Display = OledDisplay<SSD130x4WireSpi128x64Driver>;
        Display display;

    private:
    };

} // namespace two_cq_hw
