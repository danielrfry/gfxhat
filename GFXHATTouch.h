#ifndef GFXHATTOUCH_H
#define GFXHATTOUCH_H

#include <stdint.h>

#define GFXHAT_NUM_BUTTONS  6

#define GFXHAT_BTN_UP       0
#define GFXHAT_BTN_DOWN     1
#define GFXHAT_BTN_BACK     2
#define GFXHAT_BTN_MINUS    3
#define GFXHAT_BTN_SELECT   4
#define GFXHAT_BTN_PLUS     5

#define GFXHAT_LED_MODE_DIRECT  0
#define GFXHAT_LED_MODE_PULSE1  1
#define GFXHAT_LED_MODE_PULSE2  2
#define GFXHAT_LED_MODE_BREATHE 3

#define GFXHAT_TOUCH_I2C_ADDR 0x2c

class GFXHATTouch {
    public:
        GFXHATTouch();

        void begin();
        void end();

        // TOUCH STATUS

        // Reads the touch status from the chip.
        void updateTouchStatus();
        // Gets the touch status of a single button.
        bool getTouch(uint8_t button);
        // Returns true if the button has been pressed since the previous update.
        bool buttonPressed(uint8_t button);
        // Returns true if the button has been released since the previous update.
        bool buttonReleased(uint8_t button);
        // Returns the touch status of all buttons as a bitmask.
        uint8_t getTouchStatus();

        // LED CONTROL

        // Sets the on/off status of a single LED.
        void setLED(uint8_t button, bool on);
        // Send the LED on/off status to the chip.
        void updateLEDs();
        // Directly sets the on/off status of all LEDs using a bitmask.
        void setLEDs(uint8_t leds);
        // Set the pulse/breathe/direct mode of an LED.
        void setLEDMode(uint8_t button, uint8_t mode);
        // Send the LED mode settings to the chip.
        void updateLEDModes();

        // TOUCH CONFIGURATION

        // Sets the gain used by the capacitive touch sensing circuitry (0-3).
        void setGain(uint8_t gain);
        // Sets the touch sensitivity (0-7).
        void setSensitivity(uint8_t sensitivity);
        // Set the buttons enabled for touch input as a bitmask.
        void setInputsEnabled(uint8_t inputs);
        // Set the maximum number of simultaneous button touches to accept (0-4).
        // If 0, no limit is enforced.
        void setMaxTouches(uint8_t maxTouches);

        // LED CONFIGURATION

        // Set the total duration of a pulse in milliseconds (4064-8160).
        void setPulse1Period(uint16_t periodMS);
        void setPulse2Period(uint16_t periodMS);
        // Set the total duration of the breathe animation in milliseconds (4064-8160).
        void setBreathePeriod(uint16_t periodMS);
        void setPulseCounts(uint8_t pulse1Count, uint8_t pulse2Count);
        // Set min/max duty cycle of each LED behaviour type (0-15).
        void setPulse1DutyCycle(uint8_t min, uint8_t max);
        void setPulse2DutyCycle(uint8_t min, uint8_t max);
        void setBreatheDutyCycle(uint8_t min, uint8_t max);
        void setDirectDutyCycle(uint8_t min, uint8_t max);
        // Set the time taken for LEDs in direct mode to fade in and out in milliseconds (0-1750).
        void setDirectRampRate(uint16_t riseRateMS, uint16_t fallRateMS);
        // Set the delay between setting an LED to off and the fade out animation beginning in milliseconds
        // (0-1750 for breathe mode, 0-3250 for direct mode).
        void setOffDelay(uint16_t breatheMS, uint16_t directMS);

        // Returns a bitmask with the bit representing a button set.
        uint8_t getButtonMask(uint8_t button);
        // Returns a bitmask with the bit representing an LED set.
        uint8_t getLEDMask(uint8_t led);
        void writeReg(uint8_t reg, uint8_t val);
    private:
        uint8_t ctlReg;
        uint8_t leds;
        uint8_t touchStatus;
        uint8_t prevTouchStatus;
        uint16_t ledModes;
        uint8_t readReg(uint8_t reg);
        void clearInt();
        void setLEDDutyCycle(uint8_t reg, uint8_t minDuty, uint8_t maxDuty);
};

#endif // GFXHATTOUCH_H
