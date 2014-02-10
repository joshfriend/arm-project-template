# ARM Project Template

This template was developed for the [Stellaris/Tivia Launchpad](http://www.ti.com/ww/en/launchpad/stellaris_head.html?DCMP=stellaris-launchpad&HQS=stellaris-launchpad-b) boards from TI, but should work with other targets and kits.

## Requirements

### Compiler

This template requires an ARM cross compiler toolchain, which can be obtained several ways:

* [arm-eabi-toolchain](https://github.com/jsnyder/arm-eabi-toolchain)  by James Snyder
* [summon-arm-toolchain](https://github.com/esden/summon-arm-toolchain)  by Piotr Esden-Tempski
* [Sourcery Codebench Lite](http://www.mentor.com/embedded-software/sourcery-tools/sourcery-codebench/editions/lite-edition/request?id=e023fac2-e611-476b-a702-90eabb2aeca8&downloadlite=scblite2012&fmpath=/embedded-software/sourcery-tools/sourcery-codebench/editions/lite-edition/form) from Mentor Graphics

### Code Libraries

Download [StellarisWare](http://www.ti.com/tool/sw-ek-lm4f120xl) or [Tiviaware](http://www.ti.com/tool/sw-tm4c) depending on which version of the TI Launchpad you have.

Unpack the source code and place it in a good location on your computer. I use `/Developer/stellarisware` and `/Developer/tiviaware`.

