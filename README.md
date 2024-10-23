# pico_firmware_template

This repo contains a simple firmware template (folder structure, basic functionality, ...) for deploying projects. The used pins and code of WS2812b are specific to the KB2040. You find the pinout of the KB2040 [here](https://learn.adafruit.com/adafruit-kb2040/pinouts).

## Functions
- Timer interrupt
- GPIO interrupt via Button
- LED control of WS2812b
- Filter structures (IIR-Lowpass, IIR-Allpass)
- Reading a Lookup-Table (LUT)
- Basic functions for using a USB FIFO buffer and sending data hex to/from device from/to host
- Basic handler and configuration function for SPI, USB and I2C communication
- Scan and identify sensors on I2C bus (print via USB/UART)

## Sensor Libraries
- SHT21
- Bosch BMI280 (not implemented)
