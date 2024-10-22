# pico_firmware_template

This repo contains a simple firmware template (folder structure, basic functionality, ...) for deploying projects. The used pins and code of WS2812b are specific to the KB2040.

## Functions
- Timer interrupt
- GPIO interrupt via Button
- LED control of WS2812b
- Filter structures (IIR-Lowpass, IIR-Allpass)
- Reading a Lookup-Table (LUT)
- Basic functions for using a USB FIFO buffer and sending data hex to/from device from/to host
- Sensor libraries

## Libraries
- SHT21 (not tested)
- Bosch BMI280 (not implemented)
