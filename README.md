# pico_runtime_template
This repo contains a simple firmware template (folder structure, basic functionality, ...) for deploying projects. The used pins and code of WS2812b are specific to the KB2040. You find the pinout of the KB2040 [here](https://learn.adafruit.com/adafruit-kb2040/pinouts).
You can use this template for building your new projects. If you have generated the template, you can move the necessary files from the library into the firmware and adapt it and also the Python API.

Enjoy. If you have questions and suggestions, please create an issue or contact the authors.
Further sensors, devices are used from [here](https://github.com/AErbsloeh/pico_runtime_library).

## Template of a Firmware
This template provides a basic structure with timer IRQ, USB handling, and state machine. The sensors will be automatic downloaded and included.

## Template of a Python API
This Python API is usable for communicating with Pico MCU using the firmware template. 

