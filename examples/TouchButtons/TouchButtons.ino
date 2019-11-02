#include <Wire.h>
#include <GFXHat.h>

GFXHatTouch touch;

void setup() {
  Serial.begin(115200);

  // The touch sensor uses I2C to communicate - enable it first.
  Wire.begin();

  // Initialize the touch sensor and LED driver.
  touch.begin();

  // Enable multi-touch, allowing up to 3 buttons to be pressed at a time.
  touch.setMaxTouches(3);
}

void loop() {
  // Read the status of all the touch inputs.
  touch.updateTouchStatus();

  for (uint8_t btn = 0; btn < GFXHAT_NUM_BUTTONS; btn++) {
    // Check whether the button has been pressed or released.
    if (touch.buttonPressed(btn)) {
      printButton(btn);
      Serial.println(" pressed");
      // Turn on the corresponding LED.
      touch.setLED(btn, true);
    } else if (touch.buttonReleased(btn)) {
      printButton(btn);
      Serial.println(" released");
      // Turn off the corresponding LED.
      touch.setLED(btn, false);
    }
  }

  // Send the new LED on/off status to the chip.
  touch.updateLEDs();
}

void printButton(uint8_t btn) {
  switch (btn) {
    case GFXHAT_BTN_UP:
      Serial.print("Up");
      break;
    case GFXHAT_BTN_DOWN:
      Serial.print("Down");
      break;
    case GFXHAT_BTN_BACK:
      Serial.print("Back");
      break;
    case GFXHAT_BTN_MINUS:
      Serial.print("-");
      break;
    case GFXHAT_BTN_SELECT:
      Serial.print("Select");
      break;
    case GFXHAT_BTN_PLUS:
      Serial.print("+");
      break;
  }
}
