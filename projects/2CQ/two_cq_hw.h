
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
// #include "QuantizeUtils.cpp"

using namespace daisy;
using namespace patch_sm;

namespace two_cq
{

    // Hardware Definitions
    constexpr int ROOT_ADC_IN = patch_sm::CV_1;    // C5
    constexpr int SCALE_ADC_IN = patch_sm::CV_2;   // C4
    constexpr int OCATAVE_ADC_IN = patch_sm::CV_3; // C3
    constexpr int MASK_ADC_IN = patch_sm::CV_4;    // C2

    constexpr static Pin CH_RESET = DaisyPatchSM::B7;
    constexpr static Pin CH_SELECT = DaisyPatchSM::B8;

    constexpr int CH1_IN_VOCT = patch_sm::CV_5; // C6
    constexpr int CH2_IN_VOCT = patch_sm::CV_6; // C7

    constexpr int CH1_OUT_VOCT = patch_sm::CV_OUT_1; // C10
    constexpr int CH2_OUT_VOCT = patch_sm::CV_OUT_2; // C1

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

        void Init();

        int GetRootNote();
        int GetScale();
        int GetOctaveShift();
        int GetScaleMask();

        using Display = OledDisplay<SSD130x4WireSpi128x64Driver>;
        Display display;

    private:
        DaisyPatchSM &patch;
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
        ~Channel() {};

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
        int mask;
    };

} // namespace two_cq
