# RTC Time Adjustment

This utility is used strictly to program the current date and time onto the Real-Time Clock (RTC) board. 

You only need to use this script when:
* The very first battery is placed into the slot on the RTC board.
* The battery has been removed and replaced.
* The time has drifted away from the current or target time.

## Equipment required
* A computer or laptop running the Arduino IDE. The instructions below were developed in IDE version 2.3.6 on macOS Version 15.5 (Sequoia). Install the IDE and load the required libraries (refer to the main [Firmware README](../README.md) for the complete list of dependencies).
* An MCU board (e.g. ESP32-S2) that will connect to the computer or laptop
* The real time clock RTC component to be programmed
* One 4-pin cable between the MCU and RTC
* One USB cable between the MCU and the computer / laptop

> **⚠️ WARNING for Existing Sensors:** Running this script will completely overwrite the firmware of the connected MCU. If you need to reset the RTC on an already-built sensor and want to preserve its existing firmware (e.g., to avoid breaking older library dependencies), **disconnect the RTC from the sensor's MCU** and use a separate, spare MCU to perform this programming.

> **💡 Tip for Batch Building:** If you are assembling multiple sensors and have spare hardware, it is highly convenient to reserve one MCU board exclusively for RTC programming. This avoids having to repeatedly flash the RTC adjustment script and then the main sensor firmware onto every individual sensor's ESP32.

## General instructions
* Connect the MCU board to the real time clock RTC component with the 4-pin cable
* Load the rtc.ino into the IDE
* Connect the MCU to the computer via USB-C and then select the MCU in the IDE
  * **Note:** The 12V power adapter is not required for this step; the USB-C connection provides sufficient power for programming the RTC.
* Compile and upload the *.ino file by pressing the arrow button at the top left of the IDE
* Manually open the Serial Monitor for messages and interactions by pressing the magnifying glass icon at the top right of the IDE

## Instructions for using the rtc.ino script
A few handy terms:

* compile_time: the time at which the *.ino script was compiled by the IDE
* laptop_time: the time showing on the computer doing the programming
* rtc_time: the time reported by the programmed RTC
* target_time: the target time for the RTC, e.g. local time, UTC time, or some other time

This version of the programming script will initially set the clock on the RTC to match the time at which the programming script was compiled (compile_time). The script will begin reporting the current RTC time (rtc_time) every 2 seconds in the output log of the Serial Monitor once it has been successfully programmed. In some cases, the rtc_time may differ from the laptop_time, the local time, or UTC time.

If the rtc_time is not sufficiently close to the target_time, then the dialog box of the Serial Monitor window can be used to send commands to the RTC to make adjustments by skipping forward or backward. Remember to press "Return" after entering the command. Any unknown commands entered will cause the list of available commands to be shown on screen.

### Available adjustment commands:

* f - adds  5 seconds to the rtc_time
* m - adds  1 minute  to the rtc_time
* n - adds 10 minutes to the rtc_time (i.e. a nap)
* h - adds  1 hour    to the rtc_time (60 minutes)
* b - subtracts 1 hour from the rtc_time (-60 minutes)

Date roll-overs (both forward and backward) are correctly handled by the rtc.adjust() function used in the script; only the time increment commands shown above are needed.

To set UTC time, an online site such as
* https://www.timeanddate.com/worldclock/timezone/utc

can be used as a reference while deciding on adjustments.

Once the rtc_time is sufficiently close to the target_time, the device can be powered off and disconnected from the MCU. Programming is complete.
