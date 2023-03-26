/* StatusLEDs
 * 
 * Controller class for chained WS2812D RGB LED output
 *   Seven RGB LEDs
 *     The center LED is the inverted status
 *     
 *     The outside LEDs represent the direction and the speed
 *     - Blue slowest
 *     
 * 
 * WS2812D
 *   The Adafruit_NeoPixel library is used to controll the LED
 *   Each LED has a WS2812D chip with an input and an output pin.
 * 
 * Author: YeOldOne, 2023
 * 
*/ 
 
#ifndef STATUSLEDS_H
#define STATUSLEDS_H

#include <Arduino.h>  
#include "Adafruit_NeoPixel.h"

#define COLOR_TRUE_RED 0xFF0000
#define COLOR_TRUE_GREEN 0x00FF00
#define COLOR_TRUE_BLUE 0x0000FF
#define COLOR_LIGHT_GREEN 0xD9FF00
#define COLOR_MINT 0x03FFFB
#define COLOR_BLUE 0x2402FF
#define COLOR_YELLOW 0xF2FF00
#define COLOR_PURPLE 0xDE07FF

#define BLOCKED_COLOR COLOR_TRUE_RED
#define OPERATION_NORMAL_COLOR COLOR_LIGHT_GREEN
#define OPERATION_INVERTED_COLOR COLOR_MINT
#define SLOW_SPEED_COLOR COLOR_BLUE
#define MEDIUM_SPEED_COLOR COLOR_PURPLE
#define HIGH_SPEED_COLOR COLOR_YELLOW

#define BRIGHTNESS_RUNNING 255
#define BRIGHTNESS_IDLE 100

#define NR_STATUS_LEDS 7

class CStatusLEDs
{ 
  private: 
    Adafruit_NeoPixel *LEDs;
    bool Blocked;
    bool Running;
    bool Inverted;
    int Speed;
    bool Changed;
    unsigned long LastBlink;
    bool LastBlinkOn;
    void SetAllLEDs(uint32_t Color);
    uint32_t SetColorBrightness(uint32_t Color, uint8_t Brightness);
    uint32_t GradientColor(uint32_t StartColor, uint32_t EndColor, uint8_t Percentage);
  protected:
  public: 
    CStatusLEDs(uint8_t DataPin, uint8_t MaxBrightness);
    void Run();
    void SetBlocked(bool Blocked);
    void SetRunning(bool Running);
    void SetInverted(bool Inverted);
    void SetSpeed(int Speed);
};

#endif
