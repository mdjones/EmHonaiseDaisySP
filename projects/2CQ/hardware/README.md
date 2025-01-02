# Notes for Hardware

The OLED needs a switching power supply (The Patch-SM already has one). I am using the same OLED as the
[Electrosmith Patch](https://electro-smith.com/products/patch) with [schematic](https://daisy.nyc3.cdn.digitaloceanspaces.com/products/patch/ES_Daisy_Patch_Rev4.pdf)

![image](./OLED_SwitchingPWRSupply.png)

## OLED Pin Layout

This is my OLED: [SSD1306 for 51 STM32 Arduino](https://www.amazon.com/dp/B01N1LZQA8?th=1)

<https://forum.electro-smith.com/t/daisyduino-oled-for-patchsm/2536/2>

```
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

| Name | Function | Patch | Patch function |
|-- | --| -- | -- |
| GND |ground| A7 | GND |
|VCC| +3V3 input| A10 | +3V3 OUT |
|D0| CLK/SCK |D10 | SPI_SCK (Serial Clock) |
| D1| Data/SDA | D9 | ADC_11 / SPI_MISO |
| RES |  | D3 | SDMMC_D2 / UART_TX |
| DC | | D8 | ADC_12 / SPI_MISO |
| CS | | D1   | SPI_CS |
