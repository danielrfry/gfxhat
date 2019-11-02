#include <SPI.h>
#include <GFXHat.h>


// The Arduino pin connected to the LCD Chip Select input of the GFX Hat.
#define PIN_LCD_CS 0
// The Arduino pin connected to the LCD Reset input of the GFX Hat.
#define PIN_LCD_RST 1
// The Arduino pin connected to the LCD Data/Command input of the GFX Hat.
#define PIN_LCD_DC 2


GFXHatLCD lcd(PIN_LCD_RST, PIN_LCD_DC, PIN_LCD_CS);


void setup() {
  // The LCD uses SPI to communicate - enable it first.
  SPI.begin();

  // Initialize the LCD.
  lcd.begin();

  // Draw some text.
  lcd.setTextColor(1);
  lcd.setCursor(0, 0);
  lcd.println("Pimoroni GFX Hat");
  lcd.drawLine(0, 10, 128, 10, 1);

  // Draw some shapes.
  lcd.drawRect(0, 21, 32, 32, 1);
  lcd.drawCircle(64, 37, 16, 1);
  lcd.drawTriangle(96, 53, 112, 21, 128, 53, 1);

  // Send the image data to the LCD.
  lcd.show();
}


void loop() {
}
