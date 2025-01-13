# 2CQ

## Author

Michael D. Jones

## Description

The 2CQ (2 Channel Quant) quantizer produces a new quantized V/OCT source per
channel whenever a trigger is emitted. A trigger is emitted when either a trig
in is patched and a trigger or gate is sent or the trig in is not patched and
the calculated quantized voltage changes.

This quantizer has two channels who's settings can be adjusted by toggeling the
channel switch.

* Channel switch: Select which channel is selected for setting adjustment. A 
setting will be changed only if a knob has been adjusted or the All button
has been triggered.
* Reset button: Change all current channgle settings to match the knob values.
* Scale knob: Select the scale. Position 1 is chromatic.
* Root knob: Select the scales root note.
* Octave knob: Select the octave.
* Mask knob: Select the scale mask.
* V/OCT. IN: Channel V/OCT. input. Voltage outside of the 0-5 V will be ignored.
* Gate IN: Gate ins for each channel.
* Gate OUT: Gate out for each channel. If there is no gate input a gate will
be emitted if the note changes. Otherwise a gate will be emitted whenever an
input gate is recieved.
* V/OCT. OUT: Quantized V/OCT

## Build

[Flashing](https://github.com/electro-smith/DaisyWiki/wiki/1.-Setting-Up-Your-Development-Environment#4-Run-the-Blink-Example)
the Daisy via USB.

```bash
make clean ; make ; make program-dfu
```
or if you have the ST-link V3 mini hooked up

```bash
make clean ; make ; make program
```
