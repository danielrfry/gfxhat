# GFX HAT

This is an Arduino driver for the [Pimoroni GFX HAT](https://shop.pimoroni.com/products/gfx-hat).

:warning: **WARNING:** The GFX HAT was designed for use with the Raspberry Pi which uses 3.3V logic. Many Arduino and
compatible boards use 5V logic which may damage the GFX HAT. Ensure your microcontroller uses 3.3V logic before
connecting the GFX HAT. This library has been tested with [Teensy](https://www.pjrc.com/teensy/) 3.2 but other 3.3V
microcontrollers with SPI, I2C and 3 available digital I/O pins should work.

## Installation

1. Install the [Adafruit_GFX](https://github.com/adafruit/Adafruit-GFX-Library) and
   [Pimoroni SN3218](https://github.com/pimoroni/pimoroni_arduino_sn3218) libraries.
2. Download the GFX HAT library [here](https://github.com/danielrfry/gfxhat/archive/master.zip).
3. In the Arduino IDE, install using **Sketch** → **Include Library** → **Add .ZIP Library...** For more details, see
   the Arduino documentation [here](https://www.arduino.cc/en/guide/libraries).

Examples are provided demonstrating basic usage of the LCD, touch buttons and backlight. They are available from the
**File** → **Examples** menu under **Examples from Custom Libraries**.

## Wiring

[GFX HAT pinout](https://pinout.xyz/pinout/gfx_hat)

Both 5V and 3.3V power are required. Connect SDA, SCL, MOSI and SCLK to their corresponding pins on your
microcontroller board. The LCD requires three more digital I/O pins. The examples assume they are connected as follows:

GFX HAT pin     |Arduino pin
----------------|-----------
LCD Chip Select |0
LCD Reset       |1
LCD Data/Command|2
