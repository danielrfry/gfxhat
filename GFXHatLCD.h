#ifndef GFXHATLCD_H
#define GFXHATLCD_H

#include <stdint.h>
#include <Adafruit_GFX.h>

#define GFXHAT_LCD_WIDTH     128
#define GFXHAT_LCD_HEIGHT    64

class GFXHatLCD : public Adafruit_GFX {
    public:
        GFXHatLCD(uint8_t pinRst, uint8_t pinDC, uint8_t pinCS);
        void begin();
        void end();
        void setContrast(uint8_t contrast);
        void show();
        void drawPixel(int16_t x, int16_t y, uint16_t color);
        void fillScreen(uint16_t color);
    private:
        uint8_t buffer[(GFXHAT_LCD_WIDTH * GFXHAT_LCD_HEIGHT) / 8];
        uint8_t pinRst;
        uint8_t pinDC;
        uint8_t pinCS;
        void reset();
        void init();
        void write(bool command, uint8_t *buffer, int size);
        void command(uint8_t command);
};

#endif // GFXHATLCD_H