# pico_runtime_template
This repo contains a simple firmware template (folder structure, basic functionality, ...) for deploying projects. The used pins and code of WS2812b are specific to the KB2040. You find the pinout of the KB2040 [here](https://learn.adafruit.com/adafruit-kb2040/pinouts).
You can use this template for building your new projects. If you have generated the template, you can move the necessary files from the library into the firmware and adapt it and also the Python API.

Enjoy. If you have questions and suggestions, please create an issue or contact the authors.

## Hardware Abstraction Layer (hal)
- SPI handler
- I2C handler
- USB handler
- Timer interrupt
- LED control
- GPIO interrupt via Button
- Basic functions for using a USB FIFO buffer and sending data hex to/from device from/to host

## Template of a Python API
This Python API is usable for communicating with Pico MCU using the firmware template. 

## Sensor Libraries (loading via library)
- LED control of WS2812b (using PIO)
- Environmeont Sensor: SHT21
- Accelerator and Gyroscope: BMI270
- Light: VEML7700
- DAC: LTC2668, AD57x4
- ADC: ADS8881
- Digital Potentiometer: AD526x, AD528x, AD5141, AD5142
- Digital Multiplexer: ADG1408, ADGS1208
- PIO examples: WS2812, Blink LED
- Distance Estimuation: VL6180x
- Further examples can be found in this [repo](https://github.com/analogdevicesinc/no-OS/tree/main)
