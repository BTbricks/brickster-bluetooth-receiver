# BricksTer &ndash; Bluetooth Receiver

<img src="https://644db4de3505c40a0444-327723bce298e3ff5813fb42baeefbaa.ssl.cf1.rackcdn.com/uploads/project/top_image/jNrizzs2/i.png" alt="BricksTer Bluetooth Receiver top side" width="200">&nbsp;&nbsp;&nbsp;<img src="https://644db4de3505c40a0444-327723bce298e3ff5813fb42baeefbaa.ssl.cf1.rackcdn.com/uploads/project/bottom_image/jNrizzs2/i.png" alt="BricksTer Bluetooth Receiver bottom side" width="200">

The BricksTer Bluetooth Receiver lets you wirelessly control your LEGO creations from a smart phone, tablet, or PC. It is compatible with the NXT programmable brick. This repository houses the Eagle CAD schematic and design files for the printed circuit board along with the firmware for the receiver.

## Getting Started

### Part List

Components can be ordered from any distributor (e.g. DigiKey, Mouser, etc).  
[Please see the Bill of Materials for the list of components.](https://github.com/BTbricks/brickster-bluetooth-receiver/blob/master/8884bt%20Remote%20Control%20Receiver/board/BOM.md)

The rev. 2 board can be purchased from OSH Park.  
[Click here to view the OSH Park project page.](https://oshpark.com/shared_projects/jNrizzs2)

### Programming the Board

Once your board is soldered, use the Arduino IDE to load the firmware onto the microcontroller.  
The IDE is available for free and can be downloaded from http://arduino.cc/main/software

#### Setting up the Arduino Environment

The microcontroller is an ATtiny84, which is not regularly supported by the Arduino IDE.  
[Please visit the Arduino support page to learn more about using Arduino with the ATtiny84.](http://playground.arduino.cc/Main/ArduinoOnOtherAtmelChips)

Download the [Attiny 44/45/84/85 files](https://github.com/damellis/attiny/archive/master.zip) and extract them to `{ARDUINO}/hardware/`, where `{ARDUINO}` represents the directory into which the Arduino IDE and supporting files have been installed. Once completed, your directory should look like this:

```
{ARDUINO}
└-hardware
  └-attiny
    ├-boards.txt
    └-variants
      ├-tiny8
      | └-pins_arduino.h
      └-tiny14
        └-pins_arduino.h
```

#### Burning the "Bootloader"

The ATtiny84 comes from the factory running on an internal 1 MHz clock. In order to run the BricksTer firmware, the ATtiny84 must be told to use its interal 8 MHz clock.

* Connect your board to your computer via the programmer
* Apply power to the power-in Power Functions wire
* Open the Arduino IDE
* Ensure you have selected the proper serial port (`Tools > Serial Port`) and can communicate with the microcontroller
* Select `Tools > Board > ATtiny84 (internal 8 Mhz clock)`
* Select `Tools > Burn Bootloader`
* Wait for the programmer to finish, the IDE should display a success message

#### Loading the Firmware

* Connect your board to your computer via the programmer
* Apply power to the power-in Power Functions wire
* Open the Arduino IDE
* Ensure you have selected the proper serial port (`Tools > Serial Port`) and can communicate with the microcontroller
* Select `Tools > Board > ATtiny84 (internal 8 Mhz clock)`
* Open the firmware file, [`brickster8884bt.ino`](https://github.com/BTbricks/brickster-bluetooth-receiver/blob/master/8884bt%20Remote%20Control%20Receiver/firmware/brickster8884bt.ino)
* Click the Upload button on the Arduino IDE (circle with a right-pointing arrow)
* Wait for the the IDE and programmer to finish, the IDE should display a success message

Note: Use of a Atmel AVRISPmk2 programmer is strongly advised. Other programmers have been known to work, but we can only provide support for the AVRISPmk2.

### File Formats

Design files are created in CadSoft Eagle PCB Design Software.  
A free version of the software can be downloaded from www.cadsoftusa.com

<img src="https://raw.githubusercontent.com/BTbricks/brickster-bluetooth-receiver/master/8884bt%20Remote%20Control%20Receiver/board/brickster-8884bt-sch.png" alt="BricksTer Bluetooth Receiver schematic" width="380">&nbsp;&nbsp;&nbsp;<img src="https://raw.githubusercontent.com/BTbricks/brickster-bluetooth-receiver/master/8884bt%20Remote%20Control%20Receiver/board/brickster-8884bt-brd.png" alt="BricksTer Bluetooth Receiver board design" width="300">

## Distribution License

Where possible, this project uses the MIT license. [Please see the license file for full details.](https://github.com/BTbricks/brickster-bluetooth-receiver/blob/master/LICENSE)

## Contributing

The project is open to all issues and pull requests.  
If you have any other questions or feedback, please [contact the project maintainers](https://github.com/orgs/BTbricks/members).
