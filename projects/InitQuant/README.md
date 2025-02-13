# InitQuant

## Author

Michael D. Jones

## Description

Implementation of JW-Modules <https://github.com/jeremywen/JW-Modules/>
Quantizer for the [patch.Init()](https://electro-smith.com/products/patch-init).

All knobs are scaled counter clockwise to clockwise

* CV_1: Root note knob. Select root note chromatically from C to B
* CV_2: Scale knob: See: QuantizerUtils.ScaleEnum
* CV_3: Octave knob
* CV_5: v/oct CV to quantitize
* CV_OUT_1: The quantized voltage
* CV_OUT_2: LED blinks to indicate whent the knobs have changed the root note,
scale, or octave
* B5: Emits a 20 ms trigger when a note changes

## Build

If you have already used projects in this repo before just do.

To load with dfu-util (i.e. manual reset)

 ```bash
 make clean ; make ; make program-dfu
 ```

To load with OCD with a debugger probe

```bash
make clean ; make ; make program
```


To start fresh do

```bash
git clone git@github.com:mdjones/EmHonaiseDaisySP.git
cd EmHonaiseDaisySP/
make
cd ./projects/InitQuant/
make clean ; make ; make program-dfu
```
