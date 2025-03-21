# Notes for Hardware

The OLED needs a switching power supply (The Patch-SM already has one). I am
using the same OLED as the [Electrosmith Patch](https://electro-smith.com/products/patch)
with [schematic](https://daisy.nyc3.cdn.digitaloceanspaces.com/products/patch/ES_Daisy_Patch_Rev4.pdf)

## OLED Pin Layout

This is my OLED: [SSD1306 for 51 STM32 Arduino](https://www.amazon.com/dp/B01N1LZQA8?th=1) [or](http://hiletgo.com/ProductDetail/2169510.html)

KiCad Ref[https://kicad.github.io/footprints/Display.html]

[PCB](https://github.com/adafruit/Adafruit-128x64-Monochrome-OLED-PCB)

[Footprint](https://kicadrookie.blogspot.com/2022/01/096-inch-128x64-spi-display-module_4.html) - look for the link at near the bottom of follow the instructions. I could not find the datasheet diagram the designer keeps keeps showing. 

Put the 128x64_0.96_Disp_Module.kicad_mod file in side a .pretty folder

```txt
HiLetgo 0.96" inch SPI Serial 128 * 64
12864 Characters OLED LCD Display 
SSD1306 for 51 STM32 Arduino Font Color White 
```

Did a bunch of failing research, Discord, Forum, code reading. Finally decided
to search for path_sm and `OledDisplay<SSD130x4WireSpi128x64Driver>` on github
and found <https://github.com/electro-smith/libDaisy/blob/master/examples/OLED_SPI/OledSPI.cpp>

[PINS](https://electronics.stackexchange.com/questions/581256/wiring-a-ssd1306-oled-with-spi)

Final Pin set up is:

| OLED Pin Name | patch_sm pin | function |
| ------------- | ------------ | -------- |
| GND           | A7           | GND      |
| VCC           | A10          | +3V3     |
| D0            | D10          | SCLK     |
| D1            | D9           | MOSI     |
| RES           | D3           | RESET    |
| DC            | D2           | dc       |
| CS            | Not Wired    |          |

Correction: Based on <https://forum.electro-smith.com/t/connecting-an-oled-to-the-patch-init/7501/2>

> note! I am using +3V3 instead of +5V5 as above.

TODO: I feel like A3 and A2 should be swapped 

| OLED Pin Name | patch_sm pin | function    | P1-M12 patch.Init() breakout | Wire color |
| ------------- | ------------ | ----------- | ---------------------------- | ---------- |
| GND           | A4           | GND         | 12                           | black      |
| VCC           | A10          | +3V3 (+5V)  | 1                            | white      |
| D0            | D10          | SCLK        | 7                            | grey       |
| D1            | D9           | MOSI  (SDA) | 5                            | purple     |
| RES           | A3           | RESET       | 10                           | blue       |
| DC            | A2           | dc          | 9                            | green      |
| CS            | Not Wired    | Chip Select | Not wired*                   | Not wired  |

In the Daisy Patch OLED_CS is wired to 8*2 (GPIO8(SPI1_NSS)) 


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

## TODOs

TODO: Consider using project libs for sharing <https://forum.kicad.info/t/creating-a-project-using-global-libraries-and-then-exporting-to-local-libraries/47132/3>
TODO: Consider using SD_D2 for Gate 1 in patch switch
TODO: Consider swapping SIG_UART_TX and SIG_UART_RX
TODO: Test channel 2
TODO: Test all switches
TODO: Can I/should I change the patch labels to be more informative for this
project?
TODO: Put no connect flags

## KCad

<https://github.com/labtroll/KiCad-DesignRules>

Excellent video series. 

<https://youtu.be/vLnu21fS22s?si=ND8ia9d4luT5V4JJ>


## Pin optimization

Some of the pins I chose where so I could test them out on the patch.Init() 
break out board. But they are awkwardly named. But it will make prototyping easier

TODO: Why am I using SIG_SPI_NSS for Gate 1 patched? I could use SD_D2? I used it because I could check on the patch init? 

<https://github.com/sethhillbrand/kicad_templates>

Imported PCB settings from <https://github.com/sethhillbrand/kicad_templates/tree/master/JLCPCB_1-2Layer>

## Edge cuts

patch.Init() is 111.0 mm x 48.5 mm


## Models

[Thonkiconn](https://grabcad.com/library/wqp-pj398sm-1)

I didn't keep track of all the models but I pulled some from 
<https://grabcad.com/> and some from <https://www.digikey.com>.


These connectors might be able to be used for connecting the display

<https://www.mouser.com/ProductDetail/Preci-dip/801-87-050-10-001101?qs=FtMuP6KVi2S5kFH%252BuHX1%252Bw%3D%3D>
These standoffs should work
<https://www.mouser.com/ProductDetail/Wurth-Elektronik/971070244?qs=%252BEew9%252B0nqrAahtXL2o5lAw%3D%3D>


## EU Format

3U = 128.5 mm
