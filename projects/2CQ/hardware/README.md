# Notes for Hardware

The OLED needs a switching power supply (The Patch-SM already has one). I am
using the same OLED as the [Electrosmith Patch](https://electro-smith.com/products/patch)
with [schematic](https://daisy.nyc3.cdn.digitaloceanspaces.com/products/patch/ES_Daisy_Patch_Rev4.pdf)

## OLED Pin Layout

This is my OLED: [SSD1306 for 51 STM32 Arduino](https://www.amazon.com/dp/B01N1LZQA8?th=1)

```txt
HiLetgo 0.96" inch SPI Serial 128 * 64
12864 Characters OLED LCD Display 
SSD1306 for 51 STM32 Arduino Font Color White 
```

Did a bunch of failing research, Discord, Forum, code reading. Finally decided
to search for path_sm and `OledDisplay<SSD130x4WireSpi128x64Driver>` on github
and found <https://github.com/electro-smith/libDaisy/blob/master/examples/OLED_SPI/OledSPI.cpp>

Final Pin set up is:

| OLED Pin Name | patch_sm pin | function |
| ------------- | ------------ | -------- |
| GND           | A7           | GND      |
| VCC           | A10          | +3V3     |
| D0            | D10          | SCLK     |
| D1            | D9           | MOSI     |
| RES           | D3           | RESET    |
| DC            | D2           | dc       |
| C5            | Not Wired    |          |

Correction: Based on <https://forum.electro-smith.com/t/connecting-an-oled-to-the-patch-init/7501/2>

> note! I am using +3V3 instead of +5V5 as above.

| OLED Pin Name | patch_sm pin | function | P1-M12 patch.Init() breakout | Wire color |
| ------------- | ------------ | -------- | ---------------------------- | ---------- |
| GND           | A4           | GND      | 12                           | black      |
| VCC           | A10          | +3V3     | 1                            | white      |
| D0            | D10          | SCLK     | 7                            | grey       |
| D1            | D9           | MOSI     | 5                            | purple     |
| RES           | A3           | RESET    | 10                           | blue       |
| DC            | A2           | dc       | 9                            | green      |
| C5            | Not Wired    |          | Not wired                    | Not wired  |


## 3 Way Switch

<https://forum.electro-smith.com/t/adding-a-3-way-switch-to-seed/2418/4>

## Panels

<https://forum.electro-smith.com/t/frequently-asked-questions-specifically-about-patch-init/2630/5>

## Using the Audio outs as CV outs

<https://forum.electro-smith.com/t/audio-outputs-negate-the-value-i-send-from-firmware/3566/8>

## Knobs

These are probably the knobs. 

<https://www.thonk.co.uk/shop/make-noise-mutable-style-knobs/>

## Jacks

Thonkin jacks <https://www.thonk.co.uk/wp-content/uploads/2014/02/Thonkiconn_Jack_Datasheet.pdf>

1: The long leg in front connects to the brushing/sleave?
2: Normally connects to 3. I thought this would be useful but seems useless here.
3: Normally connects to 2. Connects to cable plug when norm is broken.

2 and 3 are normally connected
If you plug something in 3 will output the voltage

I am not sure this is correct but it works.

1: Connect to gate_patched pin. This only works if the cable is also plugged
into a grounded source.
2: Connect to ground. Don't think this is doing anything.
3: Connect to gate in 

## Trouble shooting

Problem: CV values hoover around 1. Make sure the 12V power supply is on
