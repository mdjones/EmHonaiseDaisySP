# Template

## Author

<!-- Insert Your Name Here -->

## Description

<!-- Describe your example here -->

## Build

[Flashing](https://github.com/electro-smith/DaisyWiki/wiki/1.-Setting-Up-Your-Development-Environment#4-Run-the-Blink-Example)
the Daisy via USB.

To load with dfu-util (i.e. manual reset)
```bash
make clean ; make ; make program-dfu
```

To load with OCD and a debugger probe

```bash
make clean ; make ; make program
```
