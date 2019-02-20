# openSip+Puff

__openSip+Puff__ aims to provide a cheap, simple, open, and modern alternative input method based on the breath of a user that can be easily mapped to a variety of common actions like mouse clicks and keypresses.

The ultimate goal of this project is to go beyond simple utilitarian functionality and provide a new modality for creative expression that is meaningful for people with or without limited mobility. For me this means musical expression and visual art applications, but you are welcome to take it in whatever direction you want!

In contrast to other DIY sip-and-puff solutions, this project aims to keep the number of parts and assembly steps to a minimum while keeping the focus entirely on the digitization of breath - nothing more, nothing less. The hope is that this device can be manufactured as a true "plug-and-play", standalone device such that the only remaining steps that users will need to complete on their own are to (1) pop the board into an enclosure, (2) attach a mouthpiece with tubing, and (3) plug in a USB cable. With the help of native USB HID functionality, this device should (theoretically) be as easy to set up as any keyboard or mouse - no special drivers needed!

In keeping the focus tight and the parts count low it is expected that this device will be possible to sell for under $100 (ideally around $60). Fewer external parts also means fewer points of failure, so the device should last a while with minimal need for maintenance.

## Features
### Hardware
* ATmega32u4 microcontroller running the [Arduino Leonardo bootloader](https://www.arduino.cc/en/Main/ArduinoBoardLeonardo), allowing for the device to work as a standard USB HID device (mouse, keyboard, MIDI, etc) without any special drivers.
* [MPXV7007GP](https://www.digikey.com/product-detail/en/nxp-usa-inc/MPXV7007GP/MPXV7007GP-ND/1168441) analog pressure sensor for detecting direction and strength of sips and puffs.
* USB mini-B port so it can be connected to any PC
* 4-pin I2C interface for future expansion (may be removed if a good use case isn't identified)

### Firmware
```
TODO:
=====
  1. Sips and puffs need to be associated with actions (keypresses, mouse clicks, MIDI notes, etc) using some 
     sort of persistent data structure that can be loaded from EEPROM when the device is powered on.
  2. Patterns of sips and puffs should also be supported at some point. For example, one puff for left mouse 
     click, two puffs for right mouse click.
  3. Output raw analog values as serial data over USB to be transformed into other interesting actions by custom 
     applications or something like Wekinator.
  4. Output analog data as MIDI CC changes so the device can be used for expressive musical applications.
```

### Software
```
TODO:
=====
To make it easy for non-technical users to reconfigure the device, some sort of easy-to-use desktop application 
should be built. 

Once the firmware is worked out, I'd like to experiment with using Electron and modern web technologies to build 
this interface. 
```

### Mechanical
```
TODO:
=====
Connecting a plain plastic tube directly the sensor is fine for prototyping, but is not hygienic for real-world 
users. Some sort of easy-to-replace filter(s) should be added to capture saliva and particulates before they 
reach the sensor. 

I don't know much about these sorts of filters, so if you have some experience or ideas please send them to me!
```

## Similar projects
When I started building this project there weren't any other viable DIY sip-and-puff interfaces out there, but in the past year so a few great projects have sprung up that are worth looking at!

* [LipSync](https://github.com/makersmakingchange/LipSync) - uses an Arduino Pro Micro board and the [MPXV7002DPT1](https://www.digikey.com/product-detail/en/nxp-usa-inc/MPXV7002DPT1/MPXV7002DPT1CT-ND/3524215) sensor, as well as an optional Bluetooth breakout board. Includes a custom mouth-operated joystick built using 3D-printed parts and FSRs, all housed in a 3D-printed enclosure. _Incredible_ documentation, and available as a kit or fully assembled through [Makers Making Change](https://www.makersmakingchange.com/project/lipsync/).
* [FLipMouse](https://github.com/asterics/FLipMouse) - uses a Teensy LC and the [MPXV7007GP](https://www.digikey.com/product-detail/en/nxp-usa-inc/MPXV7007GP/MPXV7007GP-ND/1168441) sensor. Includes a custom mouth-operated joystick that is nearly identical to LipSync, all housed in a laser-cut acrylic enclosure. Available as a kit through [Hackerspace Shop](https://hackerspaceshop.com/products/flipmouse-diy-kit).
* [The 'Sup](https://www.instructables.com/id/The-Sup-a-Mouse-for-Quadriplegics-Low-Cost-and-Ope/) - uses an Arduino Pro Micro and a [MPXV7002DP](https://www.ebay.com/itm/MD-PS002-MPXV7002DP-Transducer-Pressure-Sensor-Differential-Breakout-Board-/401602810835?oid=202106155765) sensor on a breakout board, as well as an off-the-shelf [two-axis joystick module](https://www.ebay.com/itm/Module-PS2-Lever-Hots-Two-axis-Sensor-Joystick-Game-Controller/302615277464?hash=item467546af98:g:eGQAAOSwKXdaadFS). Features a unique 3D-printed mouthpiece that couples the joystick and tubing, though no filters are used.

## License
This work is licensed under a [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License](http://creativecommons.org/licenses/by-nc-sa/4.0/).

Use this project to learn, tinker and share with friends. You are completely free to build this project for individuals  who need it. Small commercial runs for workshops or other educational uses are also completely cool.

The only reason that a fully-permissive open-source license is not used is to discourage, however naively, abuse through mass production and sale of this project by companies who are not as passionate about the quality and core mission of the project. If you're interested in producing and distributing this project to larger markets, please contact me and maybe we can collaborate!

[![Creative Commons license](https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png)](http://creativecommons.org/licenses/by-nc-sa/4.0/)

## Media

![Assembled prototype](media/assembled-prototype.jpg)
