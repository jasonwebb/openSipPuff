# openSip+Puff

__openSip+Puff__ aims to provide a cheap, simple, open and modern alternative input method based on the breath of a user that can be easily mapped to a variety of common actions like mouse clicks and keypresses.

The ultimate goal of this project is to go beyond simple utilitarian functionality and provide a new modality for creative expression that is meaningful for people with or without limited mobility. For me this means musical expression and visual art applications, but you are welcome to take it in whatever direction you want!

## Features
### Hardware
The core functionality of this project is built upon the capabilities of the [ATmega32U4 microcontroller](https://github.com/jasonwebb/openSipPuff/blob/master/datasheets/ATMega32u4.pdf) running the [Arduino Leonardo bootloader](https://www.arduino.cc/en/Main/ArduinoBoardLeonardo) paired with an analog pressure sensor.

Currently this project uses the [MPX12GP](https://github.com/jasonwebb/openSipPuff/blob/master/datasheets/MPX12.pdf) pressure sensor from Freescale, which requires an instrumentation amplifier in order to interface with the microcontroller. However, this sensor has proven to be hard to integrate into a manufacturable design, so I will be experimenting with another sensor very soon.

### Firmware
Thanks to the [Arduino Leonardo bootloader](https://www.arduino.cc/en/Main/ArduinoBoardLeonardo), __openSip+Puff__ can mimic generic USB devices such as keyboards, mice, game controllers, or musical instruments allowing data from the pressure sensor to be associated with real actions on the user's computer.

The current firmware uses a rather crude data structure stored in EEPROM to do this association, but I am hoping to come up with a more elegant solution over time. If you know of a good way to do this, please consider detailing your idea in an Issue or a fork!

### Software
Currently whenever an end user wants to change how pressure data events are associated with keyboard/mouse/etc behaviors they need to dive into the firmware code, make alterations and upload a fresh sketch. For developers this is pretty trivial, but I ultimately want this process to be easier for non-developers.

Therefore my current plan is to experiment with [Electron](http://electron.atom.io/) to build a modern, cross-platform desktop application using web technologies to create a simple, intuitive interface that allows for easy reconfiguration of the board over a transparent serial connection.

## License
This work is licensed under a [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License](http://creativecommons.org/licenses/by-nc-sa/4.0/).

Use this project to learn, tinker and share with friends. You are completely free to build this project for individuals  who need it. Small commercial runs for workshops or other educational uses are also completely cool.

The only reason that a fully-permissive open-source license is not used is to discourage, however naively, abuse through mass production and sale of this project by companies who are not as passionate about the quality and core mission of the project. If you're interested in producing and distributing this project to larger markets, please contact me and maybe we can collaborate!

[![Creative Commons license](https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png)](http://creativecommons.org/licenses/by-nc-sa/4.0/)

## Photos

![Assembled prototype](images/assembled-prototype.jpg)
