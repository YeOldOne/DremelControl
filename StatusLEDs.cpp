#include "StatusLEDs.h"

CStatusLEDs::CStatusLEDs(uint8_t DataPin, uint8_t MaxBrightness)
{
  LEDs = new Adafruit_NeoPixel(7, DataPin, NEO_RGB + NEO_KHZ800);

  Blocked = false;;
  Speed = 0;
  Changed = false;
  LastBlink = 0;
  LastBlinkOn = false;

  LEDs->begin();
  LEDs->clear();

  LEDs->setBrightness(MaxBrightness);

  //Feed off value to all LEDs
  //This makes sure all LED input and output pins are flushed
  SetAllLEDs(0x000000);
}

void CStatusLEDs::SetAllLEDs(uint32_t Color)
{
  for (int LEDNr = 0; LEDNr < NR_STATUS_LEDS; LEDNr++)
    LEDs->setPixelColor(LEDNr, Color);

  LEDs->show();
}

uint32_t CStatusLEDs::SetColorBrightness(uint32_t Color, uint8_t Brightness)
{
  //Break Color into the RGB channels
  uint8_t R = (uint8_t)(Color >> 16);
  uint8_t G = (uint8_t)(Color >> 8);
  uint8_t B = (uint8_t)Color;

  //Brightness adjust the channels
  R = (uint8_t)((R * Brightness) >> 8);
  G = (uint8_t)((G * Brightness) >> 8);
  B = (uint8_t)((B * Brightness) >> 8);

  //Combine RGB channels to the color code
  return (((uint32_t)R << 16) & 0x00FF0000) | (((uint32_t)G << 8) & 0x0000FF00) | ((uint32_t)B & 0x000000FF);
}

uint32_t CStatusLEDs::GradientColor(uint32_t StartColor, uint32_t EndColor, uint8_t Percentage)
{
  //Break Color into the RGB channels
  uint8_t StartR = (uint8_t)(StartColor >> 16);
  uint8_t StartG = (uint8_t)(StartColor >> 8);
  uint8_t StartB = (uint8_t)StartColor;

  uint8_t EndR = (uint8_t)(EndColor >> 16);
  uint8_t EndG = (uint8_t)(EndColor >> 8);
  uint8_t EndB = (uint8_t)EndColor;

  //Calculate color spectrum range
  int DeltaR = (int)EndR - (int)StartR;
  int DeltaG = (int)EndG - (int)StartG;
  int DeltaB = (int)EndB - (int)StartB;

  //Apply percentage
  int IntermediateR = round((float)(DeltaR * Percentage) / 100.0);
  int IntermediateG = round((float)(DeltaG * Percentage) / 100.0);
  int IntermediateB = round((float)(DeltaB * Percentage) / 100.0);

  //Offset relative to start color
  uint8_t NewR = (uint8_t)((int)StartR + IntermediateR);
  uint8_t NewG = (uint8_t)((int)StartG + IntermediateG);
  uint8_t NewB = (uint8_t)((int)StartB + IntermediateB);

  //Combine new RGB channels to the color code
  return (((uint32_t)NewR << 16) & 0x00FF0000) | (((uint32_t)NewG << 8) & 0x0000FF00) | ((uint32_t)NewB & 0x000000FF);
}  

void CStatusLEDs::SetBlocked(bool Blocked)
{
  if (Blocked != this->Blocked)
  {
    this->Blocked = Blocked;  
  
    Changed = true;
  }
}

void CStatusLEDs::SetSpeed(int Speed)
{
  if (Speed != this->Speed)
  {
    this->Speed = Speed;  
  
    Changed = true;
  }
}

void CStatusLEDs::SetRunning(bool Running)
{
  if (Running != this->Running)
  {
    this->Running = Running;  
  
    Changed = true;
  }
}

void CStatusLEDs::SetInverted(bool Inverted)
{
  if (Inverted != this->Inverted)
  {
    this->Inverted = Inverted;  
  
    Changed = true;

    LastBlink = 0;

    LastBlinkOn = false;
  }
}

void CStatusLEDs::Run()
{
  if (Changed)
  {
    Changed = false;

    if (Blocked)
    {
      if (Speed < 0) 
      {
        LEDs->setPixelColor(0, SetColorBrightness(BLOCKED_COLOR, 255));  
        LEDs->setPixelColor(1, SetColorBrightness(BLOCKED_COLOR, 125));  
        LEDs->setPixelColor(2, SetColorBrightness(BLOCKED_COLOR, 70));  
      }
      else
      {
        LEDs->setPixelColor(0, 0);  
        LEDs->setPixelColor(1, 0);  
        LEDs->setPixelColor(2, 0);  
      }  

      if (Speed > 0) 
      {
        LEDs->setPixelColor(4, SetColorBrightness(BLOCKED_COLOR, 70));  
        LEDs->setPixelColor(5, SetColorBrightness(BLOCKED_COLOR, 125));  
        LEDs->setPixelColor(6, SetColorBrightness(BLOCKED_COLOR, 255));  
      }
      else
      {
        LEDs->setPixelColor(4, 0);  
        LEDs->setPixelColor(5, 0);  
        LEDs->setPixelColor(6, 0);  
      }

      LEDs->show();
    }  
    else
    {
      uint8_t OperationBrightness = BRIGHTNESS_RUNNING;

      if (!Running)
        OperationBrightness = BRIGHTNESS_IDLE;

      int AbsSpeed = Speed;

      if (AbsSpeed < 0)
        AbsSpeed *= -1;

      uint32_t SpeedColor = 0;
      if (AbsSpeed < 50)
        SpeedColor = GradientColor(SLOW_SPEED_COLOR, MEDIUM_SPEED_COLOR, AbsSpeed * 2);
      else
        SpeedColor = GradientColor(MEDIUM_SPEED_COLOR, HIGH_SPEED_COLOR, (AbsSpeed - 50) * 2);
      SpeedColor = SetColorBrightness(SpeedColor, OperationBrightness);

      if (Speed < 0) 
      {
        if  (AbsSpeed >= 75)        
          LEDs->setPixelColor(0, SpeedColor);
        else
          LEDs->setPixelColor(0, 0);

        if  (AbsSpeed >= 50)        
          LEDs->setPixelColor(1, SpeedColor);
        else
          LEDs->setPixelColor(1, 0);

        LEDs->setPixelColor(2, SpeedColor);
      }
      else
      {
        LEDs->setPixelColor(0, 0x000000);
        LEDs->setPixelColor(1, 0x000000);
        LEDs->setPixelColor(2, 0x000000);
      }

      if (!Inverted)
        LEDs->setPixelColor(3, SetColorBrightness(COLOR_TRUE_GREEN, OperationBrightness));

      if (Speed > 0) 
      {
        LEDs->setPixelColor(4, SpeedColor);

        if  (AbsSpeed >= 50)        
          LEDs->setPixelColor(5, SpeedColor);
        else
          LEDs->setPixelColor(5, 0);

        if  (AbsSpeed >= 75)        
          LEDs->setPixelColor(6, SpeedColor);
        else
          LEDs->setPixelColor(6, 0);
      }
      else
      {
        LEDs->setPixelColor(4, 0x000000);
        LEDs->setPixelColor(5, 0x000000);
        LEDs->setPixelColor(6, 0x000000);
      }

      LEDs->show();
    }
  }


if (Inverted)
  if (millis() - LastBlink >= 300)
  {
    LastBlinkOn = !LastBlinkOn;

    uint8_t OperationBrightness = BRIGHTNESS_RUNNING;

    if ((!Running) || (Blocked))
      OperationBrightness = BRIGHTNESS_IDLE;

    if (LastBlinkOn) 
      LEDs->setPixelColor(3, SetColorBrightness(COLOR_TRUE_RED, OperationBrightness));
    else
      LEDs->setPixelColor(3, SetColorBrightness(COLOR_TRUE_GREEN, OperationBrightness));

    LEDs->show();

    LastBlink = millis();
  }
}
