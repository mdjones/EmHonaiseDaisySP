# 2CQSandBox

## Author

Michael D. Jones

## Description

Sanbox for what will eventually become the 2CQ (2 Channel Quant) quantizer. A 
quantizer takes a V/OCT. input and quantizes it to the nearest note in a
selected scale.

This quantizer has two channels who's settings can be adjusted by toggeling the
channel switch. 

* Channel switch: Select which channel is selected for setting adjustment.
* Reset button: Resets channel 2 settings to channel 1. This is helpful, for 
example, if you want to make sure both chanels are starting with the same scale.
* Scale knob: Select the scale. Position 1 is chromatic.
* Root knob: Select the scales root note
* Octave knob: Select the scales octave
* Sparse knob: Select the sparcity of the scale
* V/OCT. IN: Channel V/OCT. input 
* Gate IN: Gate ins for each channel. 
* Gate OUT: Gate out for each channel. If there is now gate input a gate will
be emitted if the note changes. Otherwise a gate will be emitted whenever an
input gate is recieved.
* V/OCT. OUT: Quantized V/OCT

## Build

[Flashing](https://github.com/electro-smith/DaisyWiki/wiki/1.-Setting-Up-Your-Development-Environment#4-Run-the-Blink-Example)
the Daisy via USB.

```bash
make clean ; make ; make program-dfu
```
