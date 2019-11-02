#ifndef GFXHATBACKLIGHT_H
#define GFXHATBACKLIGHT_H

#include <stdint.h>

class GFXHATBacklight {
    public:
        void begin();
        void end();
        // Set the colour of an individual backlight zone.
        // Zones are numbered 0-5 from left to right.
        void set(uint8_t zone, uint8_t r, uint8_t g, uint8_t b);
        // Set the colour of all backlight zones at once.
        void set(uint8_t r, uint8_t g, uint8_t b);
        // Send the updated backlight settings to the chip.
        void update();
};

#endif // GFXHATBACKLIGHT_H