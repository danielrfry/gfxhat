// Pimoroni GFX Hat LCD driver for Arduino.
// Based on Pimoroni's Python library:
// https://github.com/pimoroni/gfx-hat/

#include <Arduino.h>
#include <SPI.h>

#include "GFXHATLCD.h"

#define ST7567_PAGESIZE 128

#define ST7567_DISPOFF 0xae         // 0xae: Display OFF (sleep mode)  
#define ST7567_DISPON 0xaf          // 0xaf: Display ON in normal mode  

#define ST7567_SETSTARTLINE 0x40    // 0x40-7f: Set display start line  
#define ST7567_STARTLINE_MASK 0x3f

#define ST7567_REG_RATIO 0x20

#define ST7567_SETPAGESTART 0xb0    // 0xb0-b7: Set page start address  
#define ST7567_PAGESTART_MASK 0x07

#define ST7567_SETCOLL 0x00         // 0x00-0x0f: Set lower column address  
#define ST7567_COLL_MASK 0x0f
#define ST7567_SETCOLH 0x10         // 0x10-0x1f: Set higher column address  
#define ST7567_COLH_MASK 0x0f

#define ST7567_SEG_DIR_NORMAL 0xa0  // 0xa0: Column address 0 is mapped to SEG0  
#define ST7567_SEG_DIR_REV 0xa1     // 0xa1: Column address 128 is mapped to SEG0  

#define ST7567_DISPNORMAL 0xa6      // 0xa6: Normal display  
#define ST7567_DISPINVERSE 0xa7     // 0xa7: Inverse display  

#define ST7567_DISPRAM 0xa4         // 0xa4: Resume to RAM content display  
#define ST7567_DISPENTIRE 0xa5      // 0xa5: Entire display ON  

#define ST7567_BIAS_1_9 0xa2        // 0xa2: Select BIAS setting 1/9  
#define ST7567_BIAS_1_7 0xa3        // 0xa3: Select BIAS setting 1/7  

#define ST7567_ENTER_RMWMODE 0xe0   // 0xe0: Enter the Read Modify Write mode  
#define ST7567_EXIT_RMWMODE 0xee    // 0xee: Leave the Read Modify Write mode  
#define ST7567_EXIT_SOFTRST 0xe2    // 0xe2: Software RESET  

#define ST7567_SETCOMNORMAL 0xc0    // 0xc0: Set COM output direction, normal mode  
#define ST7567_SETCOMREVERSE 0xc8   // 0xc8: Set COM output direction, reverse mode  

#define ST7567_POWERCTRL_VF 0x29    // 0x29: Control built-in power circuit  
#define ST7567_POWERCTRL_VR 0x2a    // 0x2a: Control built-in power circuit  
#define ST7567_POWERCTRL_VB 0x2c    // 0x2c: Control built-in power circuit  
#define ST7567_POWERCTRL 0x2f       // 0x2c: Control built-in power circuit  

#define ST7567_REG_RES_RR0 0x21     // 0x21: Regulation Resistor ratio  
#define ST7567_REG_RES_RR1 0x22     // 0x22: Regulation Resistor ratio  
#define ST7567_REG_RES_RR2 0x24     // 0x24: Regulation Resistor ratio  

#define ST7567_SETCONTRAST 0x81     // 0x81: Set contrast control  

#define ST7567_SETBOOSTER 0xf8      // Set booster level  
#define ST7567_SETBOOSTER4X 0x00    // Set booster level  
#define ST7567_SETBOOSTER5X 0x01    // Set booster level  

#define ST7567_NOP 0xe3             // 0xe3: NOP Command for no operation  

GFXHATLCD::GFXHATLCD(uint8_t pinRst, uint8_t pinDC, uint8_t pinCS) : Adafruit_GFX(GFXHAT_LCD_WIDTH, GFXHAT_LCD_HEIGHT) {
    this->pinRst = pinRst;
    this->pinDC = pinDC;
    this->pinCS = pinCS;
}

void GFXHATLCD::begin() {
    pinMode(pinCS, OUTPUT);
    digitalWrite(pinCS, HIGH);
    pinMode(pinRst, OUTPUT);
    digitalWrite(pinRst, HIGH);
    pinMode(pinDC, OUTPUT);
    digitalWrite(pinDC, HIGH);

    this->reset();
    this->init();
}

void GFXHATLCD::end() {
}

void GFXHATLCD::reset() {
    digitalWrite(this->pinRst, LOW);
    delay(10);
    digitalWrite(this->pinRst, HIGH);
    delay(100);
}

void GFXHATLCD::init() {
    uint8_t cmds[] = {
        ST7567_BIAS_1_7,
        ST7567_SEG_DIR_NORMAL,
        ST7567_SETCOMREVERSE,
        ST7567_DISPNORMAL,
        ST7567_SETSTARTLINE | 0,
        ST7567_POWERCTRL,
        ST7567_REG_RATIO | 3
    };
    this->write(true, &cmds[0], 7);
    this->fillScreen(0);
    this->show();
    this->command(ST7567_DISPON);
    this->setContrast(44);
}

void GFXHATLCD::setContrast(uint8_t contrast) {
    uint8_t buffer[] = { ST7567_SETCONTRAST, contrast };
    this->write(true, &buffer[0], 2);
}

void GFXHATLCD::show() {
    this->command(ST7567_ENTER_RMWMODE);
    for (uint8_t page = 0; page < 8; page++) {
        uint8_t pageData[ST7567_PAGESIZE];
        uint16_t offset = page * ST7567_PAGESIZE;
        uint8_t cmds[] = { (uint8_t)(ST7567_SETPAGESTART | page), ST7567_SETCOLL, ST7567_SETCOLH };
        this->write(true, &cmds[0], 3);
        memcpy(&pageData[0], this->buffer + offset, ST7567_PAGESIZE);
        this->write(false, &pageData[0], ST7567_PAGESIZE);
    }
    this->command(ST7567_EXIT_RMWMODE);
}

void GFXHATLCD::write(bool command, uint8_t *buffer, int size) {
    digitalWrite(this->pinDC, command ? LOW : HIGH);
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    digitalWrite(this->pinCS, LOW);
    SPI.transfer(buffer, size);
    digitalWrite(this->pinCS, HIGH);
    SPI.endTransaction();
}

void GFXHATLCD::command(uint8_t command) {
    this->write(true, &command, 1);
}

void GFXHATLCD::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if (x >= GFXHAT_LCD_WIDTH || x < 0 || y >= GFXHAT_LCD_HEIGHT || y < 0)
        return;
    
    uint16_t offset = ((y / 8) * WIDTH) + x;
    uint8_t bit = y % 8;
    this->buffer[offset] &= ~(1 << bit);
    this->buffer[offset] |= (color & 1) << bit;
}

void GFXHATLCD::fillScreen(uint16_t color) {
    memset(this->buffer, color ? 0xff : 0, GFXHAT_LCD_WIDTH * GFXHAT_LCD_HEIGHT / 8);
}