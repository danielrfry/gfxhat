#include "GFXHATBacklight.h"

#include <sn3218.h>

void GFXHATBacklight::begin() {
    sn3218.begin();
    sn3218.enable_leds(SN3218_CH_ALL);
    this->set(0, 0, 0);
}

void GFXHATBacklight::end() {
}

void GFXHATBacklight::set(uint8_t zone, uint8_t r, uint8_t g, uint8_t b) {
    uint8_t led;
    switch (zone) {
        case 0: led = 6; break;
        case 1: led = 3; break;
        case 2: led = 0; break;
        case 3: led = 15; break;
        case 4: led = 12; break;
        case 5: led = 9; break;
        default: return;
    }
    sn3218.set(led, b);
    sn3218.set(led + 1, g);
    sn3218.set(led + 2, r);
}

void GFXHATBacklight::set(uint8_t r, uint8_t g, uint8_t b) {
    for (uint8_t zone = 0; zone < 6; zone++) {
        // Only turn on half of the LEDs to save power.
        if (zone % 2) {
            this->set(zone, r, g, b);
        } else {
            this->set(zone, 0, 0, 0);
        }
    }
}

void GFXHATBacklight::update() {
    sn3218.update();
}
