
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

using namespace daisy;
using namespace patch_sm;

namespace two_cq
{

    // Hardware Definitions
    constexpr int ROOT_ADC_IN = patch_sm::CV_1;    // C5
    constexpr int SCALE_ADC_IN = patch_sm::CV_2;   // C4
    constexpr int OCATAVE_ADC_IN = patch_sm::CV_3; // C3
    constexpr int SPARSE_ADC_IN = patch_sm::CV_4; // C2

    constexpr static Pin CH_RESET = DaisyPatchSM::B7;
    constexpr static Pin CH_SELECT = DaisyPatchSM::B8;

    constexpr int CH1_IN_VOCT = patch_sm::CV_5;
    constexpr int CH2_IN_VOCT = patch_sm::CV_6;
    // patch.Init() uses CV_OUT_2 for an LED
    // So I am doing a hack and sending these to
    // OUT_L[i] and OUT_R[i] too. Will not need this
    // For a pure patch_sm implementation. Maybe use
    // for random quantized voltage.
    constexpr int CH1_OUT_VOCT = patch_sm::CV_OUT_1;
    constexpr int CH2_OUT_VOCT = patch_sm::CV_OUT_2;

    // GATE_IN_1: B10
    // GATE_IN_2: B9
    // GATE_OUT_1: B5
    // GATE_OUT_2: B6
    constexpr static Pin CH1_GATE_PATCHED = DaisyPatchSM::B1;
    constexpr static Pin CH2_GATE_PATCHED = DaisyPatchSM::B2;

    constexpr static Pin OLED_DC = DaisyPatchSM::A2;
    constexpr static Pin OLED_RST = DaisyPatchSM::A3;
    constexpr static Pin OLED_SCLK = DaisyPatchSM::D10;
    constexpr static Pin OLED_MOSI = DaisyPatchSM::D9;

    class TwoCQ
    {
    public:
        TwoCQ(DaisyPatchSM &patch) : patch(patch) {};
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

        int GetRootNote();
        int GetScale();
        int GetOctaveShift();
        int GetScaleSparcity();

        using Display = OledDisplay<SSD130x4WireSpi128x64Driver>;
        Display display;

    private:
        DaisyPatchSM &patch;
    };

    int TwoCQ::GetRootNote()
    {
        float adc = patch.GetAdcValue(ROOT_ADC_IN);
        int rootNote = QuantizeUtils::rescalefjw(adc, 0, 1, 0, QuantizeUtils::NUM_NOTES);
        return rootNote;
    }

    int TwoCQ::GetScale()
    {
        float adc = patch.GetAdcValue(SCALE_ADC_IN);
        int scale = QuantizeUtils::rescalefjw(adc, 0, 1, 0, QuantizeUtils::NUM_SCALES);
        return scale;
    }

    int TwoCQ::GetOctaveShift()
    {
        float adc = patch.GetAdcValue(OCATAVE_ADC_IN);
        int octaveShift = QuantizeUtils::rescalefjw(adc, 0, 1, 0, 5);
        return octaveShift;
    }

    int TwoCQ::GetScaleSparcity()
    {
        float adc = patch.GetAdcValue(OCATAVE_ADC_IN);
        int scaleSparcity = QuantizeUtils::rescalefjw(adc, 0, 1, 0, 4);
        return scaleSparcity;
    }

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
        return false;
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
