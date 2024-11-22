# InitQuant

## Author

Michael D. Jones

## Description

Implementation of JW-Modules <https://github.com/jeremywen/JW-Modules/>
Quantizer for the [patch.Init()](https://electro-smith.com/products/patch-init).

All knobws are scaled counter clockwise to clockwise
CV_1: Root note knob. Select root note chromatically from C to B
CV_2: Scale kbob: See: QuantizerUtils.ScaleEnum
CV_3: Octave knob
CV_5: v/oct CV to quantitize
CV_OUT_1: The quantized voltage
CV_OUT_2: LED blinks to indicate whent the knobs have changed the root note,
scale, or octave
B5: 20 ms trigger when a note changes

## Build

```bash
make clean ; make ; make program-dfu
```
