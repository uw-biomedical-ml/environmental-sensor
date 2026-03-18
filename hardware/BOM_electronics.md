# Electronics Bill of Materials

| Component | Model / Description | Source / Link | Qty | Function & Notes |
| :--- | :--- | :--- | :---: | :--- |
| **MCU** | ESP32-S2 | [Adafruit 4775](https://www.adafruit.com/product/4775) | 1 | Main microcontroller processing sensor data. Contains the barrel jack power port. |
| **TFT Display** | 2.8" TFT Touch screen | [Adafruit 1947](https://www.adafruit.com/product/1947) | 1 | Minimal user interface (tap to wake display). Pairs directly with the MCU board. Contains the SD card slot.|
| **Particle Sensor** | Sensirion SEN55 | [Sensirion SEN55](https://sensirion.com/products/catalog/SEN55) | 1 | Measures particulate matter, VOCs, NOx, temperature and relative humidity. |
| **Spectral Sensor** | AS7341 10-Channel Light / Color Sensor | [Adafruit 4698](https://www.adafruit.com/product/4698) | 1 | Measures visible and near-infrared (NIR) light spectra. |
| **Adapter Board** | 3V to 5V level shifter for SEN55 | [Adafruit 5964](https://www.adafruit.com/product/5964) | 1 | Safely steps up logic voltage for the SEN55. |
| **Real Time Clock** | Precision RTC - Adafruit DS3231 | [Adafruit 3013](https://www.adafruit.com/product/3013) | 1 | Maintains system time (+/- 1 min/year, 2-5yr battery life). Reports internal temperature.|
| **Cable** | STEMMA QT / Qwiic JST SH 4-Pin - 50mm | [Adafruit 4399](https://www.adafruit.com/product/4399) | 3 | Connects I2C components. |
| **Cable** | 6-pin SEN55 cable | Custom (Similar: [Adafruit 5754](https://www.adafruit.com/product/5754)) | 1 | Connects SEN55 to the level shifter. |
| **Battery** | CR1220 | Local hardware/electronics store | 1 | Retains RTC time when unplugged. Without it, the clock resets on every startup. |
| **SD card** | Sandisk 32GB Micro SDHC Ultra Class 10 | Local electronics retailer | 1 | Stores output data files. (Recommend reformat to ExFAT before use.)|
| **Wall plug** | 12V barrel jack; center positive | Local electronics retailer | 1 | Power supply. Plugs into the barrel jack port on the MCU board. (A 6V adapter works, but 12V was used for all project data validation). |