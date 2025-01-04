# Notes for Hardware

The OLED needs a switching power supply (The Patch-SM already has one). I am using the same OLED as the
[Electrosmith Patch](https://electro-smith.com/products/patch) with [schematic](https://daisy.nyc3.cdn.digitaloceanspaces.com/products/patch/ES_Daisy_Patch_Rev4.pdf)

![image](./OLED_SwitchingPWRSupply.png)

## OLED Pin Layout

This is my OLED: [SSD1306 for 51 STM32 Arduino](https://www.amazon.com/dp/B01N1LZQA8?th=1)

This might be correct.
<https://forum.electro-smith.com/t/daisyduino-oled-for-patchsm/2536/2>

```txt
I initialized the Oled with the following:

// the magic incantation
U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI oled(U8G2_R0,
/* clock=/ PIN_PATCH_SM_D10,
/ data=/ PIN_PATCH_SM_D8,
/ cs=/ PIN_PATCH_SM_D1,
/ dc=/ PIN_PATCH_SM_D9,
/ reset=*/ PIN_PATCH_SM_D3);
I wired it up as follows:

Display Pin Daisy SM Pin
VCC… A10
GND… A7
SCK … D10
SDA … D9
RES …D3
DC …D8
CS …D1
```

Pins

Note: There seems to be a typo on the Patch SM Adapter <https://github.com/abluenautilus/Patch_SM_Breadboard_Adapter>

Hi @BlueNautilus. I have wired up your Adaptor. And seems to work great so far. I was wondering about one thing. Your board has D9 -> ADC_12, D8->ADC_11. But <https://cdn.shopify.com/s/files/1/0744/9640/9892/files/patch-sm-pinout.pdf?v=1723735216> has D9->ADC_11, and D8->ADC_12. Is that a mistake in your adaptor or was there a change in the Patch_SM?

| Name | Function | Patch | Patch function |
|-- | --| -- | -- |
| GND |ground| A7 | GND |
|VCC| +3V3 input| A10 | +3V3 OUT |
|D0| CLK/SCK |D10 | SPI_SCK (Serial Clock) |
| D1| Data/SDA | D9 | ADC_11 / SPI_MOSI |
| RES |  | D3 | SDMMC_D2 / UART_TX |
| DC | | D2 | ADC_12 / SPI_MISO |
| CS | | D1   | SPI_CS |

Or this
<https://forum.electro-smith.com/t/oleddisplay-init-with-daisy-submodule-help/2417/2>

This has <https://forum.arduino.cc/t/help-getting-started-with-a-i2c-oled/243309/6>

```txt
OLED -> Arduino

GND -> GND
VCC -> 3.3 or 5V
D0 (SCL) -> D13
D1(MOSI) -> D11
RST (RESET) -> D12
DC (DATA) -> D9
```

<https://forum.electro-smith.com/t/oled-and-sd-card-reader/3796/2>

Patch SM pins → OLED pins
SPI_CS (D1) → CS
SPI_SCK (D10) → D0
SPI_MISO (D8) → DC *
SPI_MOSI (D9) → D1
Any digital GPIO → Res (I used A3)
