/* RotaryEncoder
 * 
 * Controller class for rotary encoders
 *   Not optimized for high precision
 *   Interrupts only optimized for Arduino Nano
 * 
 * Author: Unknown
 * 
*/ 
 
#ifndef ROTARYENCODER_H
#define ROTARYENCODER_H

#include <Arduino.h>  
#include "Globals.h"

class CRotaryEncoder
{ 
  private: 
    uint8_t DataPin;
    uint8_t ClkPin;
    uint8_t SwPin;
    bool UsesHardwareInterrupt;
    volatile long *HardwareInterruptDelta;
    long Delta;
    bool LastState;
    bool ButtonState;
    long ButtonDelta;
  protected:
  public: 
    CRotaryEncoder(uint8_t DataPin, uint8_t ClkPin, uint8_t SwPin, bool SoftPullUpDataPin = false, bool SoftPullUpClkPin = false, bool SoftPullUpSwPin = true);
    void Run();
    long GetRotaryDelta();
    long GetButtonDelta();
};

#endif
