# Firmware

This directory contains the microcontroller code for the environmental sensor. The firmware is divided into two separate sketches: a utility for configuring the Real-Time Clock (RTC) and the main sensor program.

## Directory Structure

### 1. RTC Adjustment Utility (`/rtc`)
The `rtc.ino` sketch is a standalone utility used strictly for programming the current date and time into the RTC board. 
* Please refer to the `README.md` located inside the `/rtc` folder for specific instructions on how to flash and set the clock before running the main sensor code.

### 2. Main Sensor Program (`/sensor`)
The `sensor.ino` sketch contains the primary operating logic for the device. 
* **Initialization:** Checks that all hardware components and sensors are present and responding.
* **Main Loop:** Queries the connected components at set intervals and writes the logged data to output files on the local SD card.

## Required Libraries

This project relies on several open-source libraries. You will need to install these via your Arduino Library Manager (or your preferred IDE) before compiling the sketches. No external libraries are bundled within this repository.

Search for and install the following libraries (we recommend installing any missing dependencies if the IDE prompts you to do so):

* **RTClib** (by Adafruit) - *Required by both `rtc.ino` and `sensor.ino`*
* **Adafruit GFX Library** 
* **Adafruit ILI9341**
* **Adafruit FT6206 Library** 
* **Adafruit AS7341** 
* **Adafruit TouchScreen**
* **SdFat - Adafruit Fork** *(Note: Ensure you use the Adafruit fork of the SdFat library for compatibility)*
* **Sensirion I2C SEN5X**

## SD Card Preparation

Before running the main sensor firmware, we recommend preparing your SD card:
* For optimal compatibility and performance with the `SdFat` library, we highly recommend formatting your SD card to **ExFAT** (though it will generally work with standard FAT formats as well).
* Insert the card into your computer and use your operating system's default disk utility to format it.
* Once formatted, insert the SD card into the slot on your hardware before powering on the sensor. The `sensor.ino` script will automatically create the required log files on the card at startup.

## Setup and Flashing

1. Install the required libraries listed above.
2. First, open and flash `/rtc/rtc.ino` to set the correct time on the hardware. (See `/rtc/README.md` for details).
3. Once the RTC is set, open `/sensor/sensor.ino`, compile, and flash it to the microcontroller to begin normal data collection.