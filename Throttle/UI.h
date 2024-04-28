#pragma once

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>




static const unsigned char PROGMEM batteryEmpty[] =
{
  B00011111, B11111111, B11111111,
  B00010000, B00000000, B00000001,
  B01110000, B00000000, B00000001,
  B01110000, B00000000, B00000001,
  B01110000, B00000000, B00000001,
  B00010000, B00000000, B00000001,
  B00011111, B11111111, B11111111,
  B00000000, B00000000, B00000000,
};
static const unsigned char PROGMEM batteryOne[] =
{
  B00011111, B11111111, B11111111,
  B00010000, B00000000, B00000001,
  B01110000, B00000000, B01111101,
  B01110000, B00000000, B01111101,
  B01110000, B00000000, B01111101,
  B00010000, B00000000, B00000001,
  B00011111, B11111111, B11111111,
  B00000000, B00000000, B00000000,
};
static const unsigned char PROGMEM batteryTwo[] =
{
  B00011111, B11111111, B11111111,
  B00010000, B00000000, B00000001,
  B01110000, B00011111, B01111101,
  B01110000, B00011111, B01111101,
  B01110000, B00011111, B01111101,
  B00010000, B00000000, B00000001,
  B00011111, B11111111, B11111111,
  B00000000, B00000000, B00000000,
};
static const unsigned char PROGMEM batteryFull[] =
{
  B00011111, B11111111, B11111111,
  B00010000, B00000000, B00000001,
  B01110111, B11011111, B01111101,
  B01110111, B11011111, B01111101,
  B01110111, B11011111, B01111101,
  B00010000, B00000000, B00000001,
  B00011111, B11111111, B11111111,
  B00000000, B00000000, B00000000,
};


class UserInterface {
  private:
    static const int SCREEN_WIDTH   = 128;
    static const int SCREEN_HEIGHT  = 32;
    static const int OLED_RESET     = -1;
    static const int SCREEN_ADDRESS = 0x3C;
    const unsigned char *batteryIcons[4] = {batteryEmpty, batteryOne, batteryTwo, batteryFull};

  public:
    static const int width = 21;
    Adafruit_SSD1306 display;
    enum BatteryState {
      BATTERY_EMPTY = 0,
      BATTERY_ONE   = 1,
      BATTERY_TWO   = 2,
      BATTERY_THREE = 3,
      BATTERY_FULL = BATTERY_THREE,
    };
    
    UserInterface(): display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {};
    //TODO - power save mode
/*
    void powerOn() {
      display.ssd1306_command(SSD1306_DISPLAYON);
    }
    
    void powerOff() {
      display.ssd1306_command(SSD1306_DISPLAYOFF);
    }
  */  
    void showBattery(BatteryState state) {
      display.drawBitmap(104, 0,  batteryIcons[state], 24, 8, 1);      
    }

    void startScreen() {
      display.clearDisplay();
      display.setCursor(0, 0);
      textColor(false);
    }

    void textColor(bool selected) {
      if (selected)
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      else
        display.setTextColor(SSD1306_WHITE);
    }
    
    void setup() {
      if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println("SSD1306 allocation failed");
        for(;;);
      }
      display.setTextSize(1);
    }
};
