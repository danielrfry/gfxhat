#include "GFXHatTouch.h"

#include <Wire.h>

#define REG_MAIN 0x00
#define REG_INPUT 0x03
#define REG_SENSITIVITY 0x1f
#define REG_INPUT_ENABLE 0x21
#define REG_MULTI_TOUCH 0x2a
#define REG_LED_LINKING 0x72
#define REG_LED_OUTPUT 0x74
#define REG_LED_BEHAVIOUR_1 0x81
#define REG_LED_BEHAVIOUR_2 0x82
#define REG_LED_PULSE1_PERIOD 0x84
#define REG_LED_PULSE2_PERIOD 0x85
#define REG_LED_BREATHE_PERIOD 0x86
#define REG_LED_CONFIG 0x88
#define REG_LED_PULSE1_DUTY_CYCLE 0x90
#define REG_LED_PULSE2_DUTY_CYCLE 0x91
#define REG_LED_BREATHE_DUTY_CYCLE 0x92
#define REG_LED_DIRECT_DUTY_CYCLE 0x93
#define REG_LED_RAMP_RATES 0x94
#define REG_LED_OFF_DELAY 0x95

#define MAX(a,b) \
    ({ __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#define MIN(a,b) \
    ({ __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })

inline uint8_t getLEDForButton(uint8_t button) {
    return GFXHAT_NUM_BUTTONS - button - 1;
}

GFXHatTouch::GFXHatTouch() {
    this->ctlReg = 0;
    this->leds = 0;
    this->touchStatus = 0;
    this->prevTouchStatus = 0;
    this->ledModes = 0;
}

void GFXHatTouch::begin() {
}

void GFXHatTouch::end() {
}

void GFXHatTouch::updateTouchStatus() {
    this->prevTouchStatus = this->touchStatus;
    this->touchStatus = this->readReg(REG_INPUT);
    this->clearInt();
}

bool GFXHatTouch::getTouch(uint8_t button) {
    return (this->touchStatus & this->getButtonMask(button)) != 0;
}

bool GFXHatTouch::buttonPressed(uint8_t button) {
    uint8_t mask = this->getButtonMask(button);
    return (this->touchStatus & mask) && !(this->prevTouchStatus & mask);
}

bool GFXHatTouch::buttonReleased(uint8_t button) {
    uint8_t mask = this->getButtonMask(button);
    return !(this->touchStatus & mask) && (this->prevTouchStatus & mask);
}

uint8_t GFXHatTouch::getTouchStatus() {
    return this->touchStatus;
}

void GFXHatTouch::setLED(uint8_t button, bool on) {
    if (on)
        this->leds = (this->leds | this->getLEDMask(button)) & 0x3f;
    else
        this->leds &= ~this->getLEDMask(button);
}

void GFXHatTouch::updateLEDs() {
    this->writeReg(REG_LED_OUTPUT, this->leds & 0x3f);
    this->leds = leds;
}

void GFXHatTouch::setLEDs(uint8_t leds) {
    this->leds = leds & 0x3f;
}

void GFXHatTouch::setLEDMode(uint8_t button, uint8_t mode) {
    uint8_t led = getLEDForButton(button);
    this->ledModes &= ~(3 << (led * 2));
    this->ledModes |= ((mode & 0x03) << (led * 2));
}

void GFXHatTouch::updateLEDModes() {
    Wire.beginTransmission(GFXHAT_TOUCH_I2C_ADDR);
    Wire.write(REG_LED_BEHAVIOUR_1);
    Wire.write((const uint8_t *)&this->ledModes, 2);
    Wire.endTransmission();
}

void GFXHatTouch::setGain(uint8_t gain) {
    this->ctlReg = (gain & 0x03) << 6;
    this->writeReg(REG_MAIN, this->ctlReg);
}

void GFXHatTouch::setSensitivity(uint8_t sensitivity) {
    this->writeReg(REG_SENSITIVITY, ((sensitivity & 0x07) << 4) | 0x0f);
}

void GFXHatTouch::setInputsEnabled(uint8_t inputs) {
    this->writeReg(REG_INPUT_ENABLE, inputs & 0x3f);
}

void GFXHatTouch::setMaxTouches(uint8_t maxTouches) {
    maxTouches = MIN(4, maxTouches);
    if (maxTouches) {
        this->writeReg(REG_MULTI_TOUCH, 0x80 | (((maxTouches - 1) & 0x03) << 2));
    } else {
        this->writeReg(REG_MULTI_TOUCH, 0x00);
    }
}

void GFXHatTouch::setPulse1Period(uint16_t periodMS) {
    periodMS = MIN(4064, periodMS);
    this->writeReg(REG_LED_PULSE1_PERIOD, (uint8_t)(periodMS / 32));
}

void GFXHatTouch::setPulse2Period(uint16_t periodMS) {
    periodMS = MIN(4064, periodMS);
    this->writeReg(REG_LED_PULSE2_PERIOD, (uint8_t)(periodMS / 32));
}

void GFXHatTouch::setBreathePeriod(uint16_t periodMS) {
    periodMS = MIN(4064, periodMS);
    this->writeReg(REG_LED_BREATHE_PERIOD, (uint8_t)(periodMS / 32));
}

void GFXHatTouch::setPulseCounts(uint8_t pulse1Count, uint8_t pulse2Count) {
    pulse1Count = MAX(1, MIN(pulse1Count, 8));
    pulse2Count = MAX(1, MIN(pulse2Count, 8));
    this->writeReg(REG_LED_CONFIG, (pulse1Count - 1) | ((pulse2Count - 1) << 3));
}

void GFXHatTouch::setPulse1DutyCycle(uint8_t min, uint8_t max) {
    this->setLEDDutyCycle(REG_LED_PULSE1_DUTY_CYCLE, min, max);
}

void GFXHatTouch::setPulse2DutyCycle(uint8_t min, uint8_t max) {
    this->setLEDDutyCycle(REG_LED_PULSE2_DUTY_CYCLE, min, max);
}

void GFXHatTouch::setBreatheDutyCycle(uint8_t min, uint8_t max) {
    this->setLEDDutyCycle(REG_LED_BREATHE_DUTY_CYCLE, min, max);
}

void GFXHatTouch::setDirectDutyCycle(uint8_t min, uint8_t max) {
    this->setLEDDutyCycle(REG_LED_DIRECT_DUTY_CYCLE, min, max);
}

void GFXHatTouch::setDirectRampRate(uint16_t riseRateMS, uint16_t fallRateMS) {
    riseRateMS = MIN(riseRateMS / 250, 7);
    fallRateMS = MIN(fallRateMS / 250, 7);
    this->writeReg(REG_LED_RAMP_RATES, fallRateMS | (riseRateMS << 3));
}

void GFXHatTouch::setOffDelay(uint16_t breatheMS, uint16_t directMS) {
    breatheMS = MIN(breatheMS / 250, 7);
    directMS = MIN(directMS / 250, 13);
    this->writeReg(REG_LED_OFF_DELAY, directMS | (breatheMS << 4));
}

uint8_t GFXHatTouch::getButtonMask(uint8_t button) {
    return 1 << button;
}

uint8_t GFXHatTouch::getLEDMask(uint8_t led) {
    return 1 << getLEDForButton(led);
}

void GFXHatTouch::writeReg(uint8_t reg, uint8_t val) {
    Wire.beginTransmission(GFXHAT_TOUCH_I2C_ADDR);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
}

uint8_t GFXHatTouch::readReg(uint8_t reg) {
    Wire.beginTransmission(GFXHAT_TOUCH_I2C_ADDR);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(GFXHAT_TOUCH_I2C_ADDR, (int)reg);
    return Wire.read();
}

void GFXHatTouch::clearInt() {
    this->ctlReg &= 0xfe;
    this->writeReg(REG_MAIN, this->ctlReg);
}

void GFXHatTouch::setLEDDutyCycle(uint8_t reg, uint8_t minDuty, uint8_t maxDuty) {
    minDuty = MIN(minDuty, 0x0f);
    maxDuty = MIN(maxDuty, 0x0f);
    this->writeReg(reg, minDuty | (maxDuty << 4));
}
