#include <Wire.h>
#include <GFXHAT.h>

GFXHATBacklight light;

void setup() {
  // The backlight driver uses I2C to communicate - enable it first.
  Wire.begin();

  // Initialise the backlight driver.
  light.begin();
}

void loop() {
  // Cycle through some backlight colours

  // Red-Yellow
  for (uint8_t i = 0; i < 255; i++) {
    light.set(255, i, 0);
    light.update();
    delay(10);
  }
  // Yellow-Green
  for (uint8_t i = 0; i < 255; i++) {
    light.set(255 - i, 255, 0);
    light.update();
    delay(5);
  }
  // Green-Cyan
  for (uint8_t i = 0; i < 255; i++) {
    light.set(0, 255, i);
    light.update();
    delay(5);
  }
  // Cyan-Blue
  for (uint8_t i = 0; i < 255; i++) {
    light.set(0, 255 - i, 255);
    light.update();
    delay(5);
  }
  // Blue-Magenta
  for (uint8_t i = 0; i < 255; i++) {
    light.set(i, 0, 255);
    light.update();
    delay(5);
  }
  // Magenta-Red
  for (uint8_t i = 0; i < 255; i++) {
    light.set(255, 0, 255 - i);
    light.update();
    delay(5);
  }
}
