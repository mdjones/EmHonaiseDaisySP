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

| OLED Pin Name | patch_sm pin | function | P1-M12 patch.Init() breakout |
| ------------- | ------------ | -------- | ---------------------------- |
| GND           | A4           | GND      | 12                           |
| VCC           | A10          | +3V3     | 1                            |
| D0            | D10          | SCLK     | 7                            |
| D1            | D9           | MOSI     | 5                            |
| RES           | A3           | RESET    | 10                           |
| DC            | A2           | dc       | 9                            |
| C5            | Not Wired    |          | Not wired                    |


## 3 Way Switch

<https://forum.electro-smith.com/t/adding-a-3-way-switch-to-seed/2418/4>

## Panels

<https://forum.electro-smith.com/t/frequently-asked-questions-specifically-about-patch-init/2630/5>


## Trouble shooting

Problem: CV values hoover around 1. Make sure the 12V power supply is on
