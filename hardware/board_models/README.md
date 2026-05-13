# Reference Board Models

> **🚧 Work in Progress:** This repository is currently under active development. Files and documentation are being added incrementally.

This directory contains 3D reference models (digital twins) of the physical electronic components used in the environmental sensor. 

**Note:** These files are provided strictly as dimensional references for use in CAD software to help you design custom enclosures or modify our existing case. They are *not* intended to be 3D printed.

## Reference STL Files

* **`metro.stl`:** Reference model for the main MCU board. 
* **`tft.stl`:** Reference model for the 2.8" TFT Touchscreen. 
* **`light.stl`:** Reference model for the AS7341 10-Channel Spectral Sensor.
* **`sen55.stl`:** Reference model (derivative) for the Sensirion SEN55 particle sensor. A STEP file can be found on the Sensirion website for the [SEN55]( https://sensirion.com/products/catalog/SEN55 ). Direct link: [SEN5x STEP file](https://sensirion.com/resource/cad/sen5x).

## Licensing Exceptions

While the majority of the hardware designs, firmware, and documentation in this repository are openly licensed under the MIT License, some of the 3D reference models in this directory are subject to different terms:

* **`light.stl` and `metro.stl`:** Licensed under MIT License Copyright (c) 2016 Adafruit Industries. Please see the `LICENSE-light-metro.txt` file in this directory for full terms.
* **`tft.stl`:** Licensed under Creative Commons Attribution-ShareAlike 3.0 Adafruit Industries. Please see the `LICENSE-tft.stl` file in this directory for full terms.
  
* * **`sen55.stl`:** This derivative STL part is included by permission from Sensirion Inc. Global Technical Support. Please see Sensirion's publicly available SEN55 STEP file for definitive information on the SEN55 component.

Please ensure you review the relevant license files if you plan to redistribute or modify these specific CAD models.
