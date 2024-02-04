# EmHonaise DaisySP

## Purpose

A place to support DaisySP development

## Setup

How to setup your enviornment for DaisySP dev.

See [DaisyWiki](https://github.com/electro-smith/DaisyWiki/wiki/1.-Setting-Up-Your-Development-Environment) for setting up your development
enviornment.

* Build the DaisySP submodules

```bash
git submodule update --init --recursive
cd libdaisy/lib/DaisySP/
make
cd ../libDaisy/
make
```

* Install the toolchain tools

The tool chain are build 
[utilities](https://github.com/electro-smith/DaisyWiki/wiki/Understanding-the-Toolchain) 
needed for DaisySP dev. You are likely to have a few of them already. 
Electrosmith also provides utilities to install them for you.

* Create your module

Boards: patch_sm,seed

```bash
./helper.py create ./projects/MyDSPProj --board patch_sm
```

## Build

For each example directory do the following after 
[Flashing](https://github.com/electro-smith/DaisyWiki/wiki/1.-Setting-Up-Your-Development-Environment#4-Run-the-Blink-Example)
the Daisy via USB.

```bash
make clean ; make ; make program-dfu
```
