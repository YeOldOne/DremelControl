/* ButtonProcessor
 * 
 * Controller class for buttons
 *   Removes mechanical contact bounces
 *   Distinguished between short and long presses
 * 
 * Author: YeOldOne, 2023
 * 
*/ 
 
#ifndef BUTTONPROCESSOR_H
#define BUTTONPROCESSOR_H

#include <Arduino.h>  

class CButtonProcessor
{ 
  private: 
    uint8_t Pin;
    bool LastState;
    unsigned long LastChange;
    bool ShortPressed;
    bool LongPressed;
  protected:
  public: 
    CButtonProcessor(uint8_t Pin, bool SoftPullUpSwPin = true);
    void Run();
    int ShortPressMinTimeMs;
    int LongPressMinTimeMs;
    bool GetShortPressed();
    bool GetLongPressed();
};

#endif
