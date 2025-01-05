
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
#include "QuantizeUtils.cpp"

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

    class Channel
    {

    private:
        DaisyPatchSM &patch;
        Switch gatePatchedSwitch_;
        int in_voct_accessor_;
        int out_voct_accessor_;
        float quant_voct_;
        int channelNum_;
        float out_voct_;
        GateIn gate_in_;
        dsy_gpio gate_out_;

    public:
        Channel(DaisyPatchSM &patch) : patch(patch) {};
        ~Channel() {}

        void Init(int channelNum,
                  GateIn gate_in,
                  dsy_gpio gate_out,
                  Pin gate_patched_pin,
                  int in_voct_accessor,
                  int out_voct_accessor);

        void quantize();
        void trig();
        bool gate_patched();
        GateIn GetGateIn();
        bool quant_voct_changed();
        void set_quant2voct();
        int GetChannelNum();
        float GetVoctOut();

        int rootNote;
        int scale;
        int octaveShift;
    };

    void Channel::Init(
        int channelNum,
        GateIn gate_in,
        dsy_gpio gate_out,
        Pin gate_patched_pin,
        int in_voct_accessor,
        int out_voct_accessor)
    {
        channelNum_ = channelNum;
        gate_in_ = gate_in;
        gate_out_ = gate_out;
        gatePatchedSwitch_.Init(gate_patched_pin);
        in_voct_accessor_ = in_voct_accessor;
        out_voct_accessor_ = out_voct_accessor;

        rootNote = 0;
        scale = 0;
        octaveShift = 0;

        quantize();
        set_quant2voct();
    }

    int Channel::GetChannelNum()
    {
        return channelNum_;
    }

    void Channel::trig()
    {
        /** Set the gate high */
        dsy_gpio_write(&gate_out_, true);

        /** Wait 20 ms */
        patch.Delay(20);

        /** Set the gate low */
        dsy_gpio_write(&gate_out_, false);
    }

    GateIn Channel::GetGateIn()
    {
        return gate_in_;
    }

    float Channel::GetVoctOut()
    {
        return out_voct_;
    }

    bool Channel::gate_patched()
    {
        gatePatchedSwitch_.Debounce();
        // TODO: Revert to !gatePatchedSwitch_.Pressed() when I have the thonk wired
        // up
        // return gatePatchedSwitch_.Pressed();
        return true;
    }

    /** Quantize any voltage from 0-5 */
    void Channel::quantize()
    {
        float in_adc = patch.GetAdcValue(in_voct_accessor_);
        float in_voct = QuantizeUtils::rescalefjw(in_adc, 0, 1, 0, 5);
        quant_voct_ = QuantizeUtils::closestVoltageInScale(
            in_voct,
            rootNote,
            scale);

        quant_voct_ += octaveShift;
    }

    bool Channel::quant_voct_changed()
    {
        bool voct_changed = !QuantizeUtils::AlmostEqualRelative(out_voct_, quant_voct_);
        return voct_changed;
    }

    void Channel::set_quant2voct()
    {
        out_voct_ = quant_voct_;
        patch.WriteCvOut(out_voct_accessor_, out_voct_);
    }

} // namespace two_cq_hw
