# Environmental Sensor: Hardware and Firmware

> **🚧 Work in Progress:** This repository is currently under active development. Files and documentation are being added incrementally.

This repository is home to the hardware designs, wiring instructions, and firmware source code for the environmental sensor project developed as part of the AI-READI initiative. 

## Publication & Project Context 

To support the AI-READI initiative's goal of making AI-ready data generation tools available to everyone, we are sharing the hardware designs, 3D enclosure (case) design files, and firmware mentioned in our upcoming paper. These resources are here for you to use, modify, and build upon. We invite you to download the code and design files and assemble your own sensor.

* 📄 **Paper:** *[Link to published paper will be added here]*
* 📊 **Supplement:** *[Link to supplementary materials will be added here]*

**Note on Scope:** This repository is strictly focused on the technical implementation of the sensor's physical build and microcontroller programming. For overarching project details, data analysis methodologies, and general information, please visit [aireadi.org](https://aireadi.org). We intentionally do not duplicate that information here. 

**No Research Approval Implied:** The AIREADI project was approved by an institutional review board to collect data from the homes of human participants. This approval, which is referenced in the [AIREADI Healthsheet](https://docs.aireadi.org/docs/3/dataset/healthsheet#collection-process), does not extend approval for any other entity to use this device in research with human participants without obtaining the permissions relevant to their proposed research.

## Repository Structure

### 1. `/firmware`
Contains the microcontroller code for the ESP32-S2.
* **`/sensor`:** The main program that initializes components, runs the primary data collection loop, and writes outputs to the SD card.
* **`/rtc`:** A standalone utility used strictly for setting the correct time on the precision RTC board.

### 2. `/hardware`
Contains the physical specifications, BOMs, and 3D files for the sensor build. *(The bill of materials files are available now; others will be uploaded soon).*
* **BOMs:** `BOM_electronics.md` (required electronics) and `BOM_assembly.md` (optional case hardware) list the necessary components.
* **`/wiring`:** Diagrams, schematics, and photos for connecting the electronics. *We highly recommend starting here to test your components before printing the case.*
* **`/enclosure`:** 3D printable files (STLs) for the 6-piece external case and internal PLA component trays, along with physical assembly notes.
* **`/board_models`:** 3D reference models (digital twins) of the electronic boards for those looking to design custom CAD enclosures.

## Getting Started Workflow

If you are building a sensor from scratch, we recommend the following sequence:

1. **Source Components:** Gather the required parts listed in `/hardware/BOM_electronics.md`.
2. **Wire and Test:** Follow the guides in `/hardware/wiring` to connect the electronics on your desk. 
3. **Flash Firmware:** Navigate to the `/firmware` directory to install the required libraries, set the RTC clock, and flash the main sensor code to verify the hardware is communicating properly.
4. **Assemble the Case:** Once the bare electronics are functioning, gather the hardware from `/hardware/BOM_assembly.md` and follow the instructions in `/hardware/enclosure` to print and assemble the physical unit.

---
*For general project information, please visit [aireadi.org](https://aireadi.org).*

## License and Citation

The hardware designs, firmware, and documentation in this repository are licensed under the [MIT License](LICENSE), with the following exceptions located in the `/hardware/board_models` directory:
* **`light.stl` and `metro.stl`:** Licensed under MIT License Copyright (c) 2016 Adafruit Industries.
* **`tft.stl`:** Licensed under Creative Commons Attribution-ShareAlike 3.0 Adafruit Industries.

*(Please see the `README.md` or license files within the `/hardware/board_models` directory for the full text of these specific licenses).*

If you use these hardware designs, firmware, or documentation in your research, please cite our paper:


**🚧 Our paper is making its way through the publishing process...we will update this when we have final information.**


> **[Authors]. (2026). [Title of the AI-READI Environmental Sensor Paper]. [Journal/Conference Name]. [DOI/Link]**

**BibTeX:**
```bibtex
@article{aireadi_sensor_2026,
  title={Title of the AI-READI Environmental Sensor Paper},
  author={Lastname, Firstname and Lastname, Firstname},
  journal={Journal Name},
  year={2026},
  publisher={Publisher}
}
