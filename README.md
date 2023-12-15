# EmHonaise DaisySP

## Purpose

A place to support DaisySP development

## Setup

How to setup your enviornment for DaisySP dev.

Electrosmith has a
[guide](https://github.com/electro-smith/DaisyWiki/wiki/1.-Setting-Up-Your-Development-Environment)
directed for VS Code users. Below is a more general guide.

1. Install the toolchain tools

The tool chain are build 
[utilities](https://github.com/electro-smith/DaisyWiki/wiki/Understanding-the-Toolchain) 
needed for DaisySP dev. You are likely to have a few of them already. 
Electrosmith also provides utilities to install them for you.

> **_NOTE:_**  The libDaisy and DaisySP will be built and deployed within their
> cloned repo directories. The default target is `make all`. There is no formal
> instalation target. You could copy the build dir to another location but they 
> work fine in their repo directories.

> **_NOTE:_**  The [DaisyExamples](https://github.com/electro-smith/DaisyExamples) 
> have relative LIBDAISY_DIR and DAISYSP_DIR already set in their Makefiles.
> You could make them point to your intalation by using [Conditional Variable 
> Assignment](https://www.gnu.org/software/make/manual/make.html#index-conditional-variable-assignment)

2. Get libDaisy

<https://github.com/electro-smith/libDaisy> is the Hardware Abstraction Library
for the Daisy Audio Platform 

```bash
git clone https://github.com/electro-smith/libDaisy.git
cd libDaisy
make all
```

3. Get DaisySP

<https://github.com/electro-smith/DaisySP> A Powerful, Open Source DSP Library
in C++. This provides common DSP classes.

```bash
git clone https://github.com/electro-smith/DaisySP.git
cd DaisySP
make all
```

4. Add libDaisy and DaisySP to your enviornment

```bash
export LIBDAISY_DIR=${HOME}/Builds/libDaisy/
export DAISYSP_DIR=${HOME}/Builds/DaisySP/
```

5. Test Blink example

```bash
cd  seed_examples/Blink
make clean ; make ; make program-dfu
```

>  **_NOTE:_**   You may get an `Error during download get_status` but it doesn;t seem to
> be a problem

6. Create your own module

```bash
./helper.py create ~/Builds/seed/MySeedProj --board seed
```

