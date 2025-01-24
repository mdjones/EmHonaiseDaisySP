
/**
 * Daisy Patch SM based 2 Channel Quantizer
 * Display is HiLetgo 0.96" inch SPI Serial 128 * 64
 * 12864 Characters OLED LCD Display
 * SSD1306 for 51 STM32 Arduino Font Color White
 *
 * Configured based on
 * https://github.com/electro-smith/libDaisy/blob/master/examples/OLED_SPI/OledSPI.cpp
 * and  https://discord.com/channels/1037767234803740694/1039589722831474830/1203742317346037800
 *
 */
#pragma once

#include "dev/oled_ssd130x.h"
#include "daisy_patch_sm.h"
#include "QuantizeUtils.cpp"
#include "two_cq_hw.h"

using namespace daisy;
using namespace patch_sm;

namespace two_cq
{

    void TwoCQ::Init()
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
        spi_conf.pin_config.sclk = OLED_SCLK; // Use pin D10 as SCLK
        spi_conf.pin_config.miso = Pin();     // We won't need this
        spi_conf.pin_config.mosi = OLED_MOSI; // Use D9 as MOSI
        spi_conf.pin_config.nss = Pin();      // DaisyPatchSM::D1;   // use D1 as NSS

        // data will flow from master
        // The master will output on the NSS line
        spi_conf.nss = SpiHandle::Config::NSS::SOFT;

        display_config.driver_config.transport_config.pin_config.dc = OLED_DC;
        display_config.driver_config.transport_config.pin_config.reset = OLED_RST;
        display.Init(display_config);
    }

    int TwoCQ::GetRootNote()
    {
        float adc = patch.GetAdcValue(ROOT_ADC_IN);
        int rootNote = QuantizeUtils::rescalefjw(
            adc, 0, 1, 0, QuantizeUtils::NUM_NOTES);
        return rootNote;
    }

    int TwoCQ::GetScale()
    {
        float adc = patch.GetAdcValue(SCALE_ADC_IN);
        int scale = QuantizeUtils::rescalefjw(
            adc, 0, 1, 0, QuantizeUtils::NUM_SCALES);
        return scale;
    }

    int TwoCQ::GetOctaveShift()
    {
        float adc = patch.GetAdcValue(OCATAVE_ADC_IN);
        int octaveShift = QuantizeUtils::rescalefjw(
            adc, 0, 1, 0, QuantizeUtils::NUM_OCTAVES);
        return octaveShift;
    }

    int TwoCQ::GetScaleMask()
    {
        float adc = patch.GetAdcValue(MASK_ADC_IN);
        int scaleMask = QuantizeUtils::rescalefjw(
            adc, 0, 1, 0, QuantizeUtils::NUM_MASKS);
        return scaleMask;
    }

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
        gatePatchedSwitch_.Init(gate_patched_pin, patch.AudioCallbackRate());
        in_voct_accessor_ = in_voct_accessor;
        out_voct_accessor_ = out_voct_accessor;

        rootNote = 0;
        scale = 0;
        octaveShift = 0;
        mask = 0;

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
        //gatePatchedSwitch_.Debounce();
        //This should be not true when the gate is patched
        //return gatePatchedSwitch_.Pressed();

        return channelNum_==1;
    }

    /** Quantize any voltage from 0-5 */
    void Channel::quantize()
    {
        float in_adc = patch.GetAdcValue(in_voct_accessor_);
        float in_voct = QuantizeUtils::rescalefjw(in_adc, 0, 1, 0, 5);
        quant_voct_ = QuantizeUtils::closestVoltageInScale(
            in_voct,
            rootNote,
            scale,
            mask,
            patch);

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
