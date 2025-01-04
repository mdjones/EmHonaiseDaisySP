# Notes for Hardware

The OLED needs a switching power supply (The Patch-SM already has one). I am
using the same OLED as the [Electrosmith Patch](https://electro-smith.com/products/patch)
with [schematic](https://daisy.nyc3.cdn.digitaloceanspaces.com/products/patch/ES_Daisy_Patch_Rev4.pdf)

## OLED Pin Layout

This is my OLED: [SSD1306 for 51 STM32 Arduino](https://www.amazon.com/dp/B01N1LZQA8?th=1)

Did a bunch of failing research, Discord, Forum, code reading. Finally decided
to search for path_sm and `OledDisplay<SSD130x4WireSpi128x64Driver>` on github
and found <https://github.com/electro-smith/libDaisy/blob/master/examples/OLED_SPI/OledSPI.cpp>

Final Pin set up is:

| OLED Pin Name | patch_sm pin | function |
| ------------- | ------------ | -------- |
| GND           | A7           | GND      |
| VCC           | A19          | +3V3     |
| D0            | D10          | SCLK     |
| D1            | D9           | MOSI     |
| RES           | D3           | RESET    |
| DC            | D2           | dc       |
| C5            | Not Wired    |          |
