#include "ButtonProcessor.h"

CButtonProcessor::CButtonProcessor(uint8_t Pin, bool SoftPullUpSwPin = true)
{
  this->Pin = Pin;
  
  pinMode(Pin, INPUT); 
  
  if (SoftPullUpSwPin)
    digitalWrite(Pin, INPUT_PULLUP);

  ShortPressMinTimeMs = 50;
  LongPressMinTimeMs = 300;
  ShortPressed = false;
  LongPressed = false;
  LastState = (digitalRead(Pin) == LOW);
  LastChange = millis();
}

void CButtonProcessor::Run()
{
  bool CurrentState = (digitalRead(Pin) == LOW);

  if (CurrentState != LastState)
  {
    //Debounces on button down, button up doesn't need to be checked for it's equal to the default previous state
    if ((CurrentState) && (millis() - LastChange < ShortPressMinTimeMs)) 
      return;

    LastState = CurrentState;

    //Register presses on button up
    if (!CurrentState)
    {
      if (millis() - LastChange >= LongPressMinTimeMs) 
        LongPressed = true;
      else
        ShortPressed = true;
    }

    LastChange = millis();
  }
}

bool CButtonProcessor::GetShortPressed()
{
  bool State = ShortPressed;

  ShortPressed = false;

  return State;
}

bool CButtonProcessor::GetLongPressed()
{
  bool State = LongPressed;

  LongPressed = false;

  return State;
}
