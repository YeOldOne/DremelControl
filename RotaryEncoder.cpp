#include "RotaryEncoder.h"

long GetDelta(uint8_t DataPin, uint8_t ClkPin, bool *LastState)
{
  bool DataState = digitalRead(DataPin) == LOW;
  bool TempLastState = *LastState;
  *LastState = DataState;
  if ((!TempLastState) && (DataState))
  {
    if (digitalRead(ClkPin) == HIGH)
      return -1;
    else
      return 1;
  }
  return 0;
}

void ProcessHWInterrupt(uint8_t DataPin, uint8_t ClkPin, volatile long *Delta, volatile bool *LastState)
{
  bool HWLastState = *LastState;
  *Delta += GetDelta(DataPin, ClkPin, &HWLastState);
  *LastState = HWLastState;
}

volatile long DeltaInterrupt0 = 0;
volatile uint8_t ClkPinInterrupt0 = NO_PIN;
volatile bool LastStateInterrupt0 = false;
void HWInterrupt0()
{
  ProcessHWInterrupt(2, ClkPinInterrupt0, &DeltaInterrupt0, &LastStateInterrupt0);
}

volatile long DeltaInterrupt1 = 0;
volatile uint8_t ClkPinInterrupt1 = NO_PIN;
volatile bool LastStateInterrupt1 = false;
void HWInterrupt1()
{
  ProcessHWInterrupt(3, ClkPinInterrupt1, &DeltaInterrupt1, &LastStateInterrupt1);
}

CRotaryEncoder::CRotaryEncoder(uint8_t DataPin, uint8_t ClkPin, uint8_t SwPin, bool SoftPullUpDataPin, bool SoftPullUpClkPin, bool SoftPullUpSwPin)
{
  this->DataPin = DataPin;
  this->ClkPin = ClkPin;
  this->SwPin = SwPin;

  pinMode(DataPin, INPUT); 
  pinMode(ClkPin, INPUT); 
  if (SwPin != NO_PIN)
    pinMode(SwPin, INPUT); 

  if (SoftPullUpDataPin)
    digitalWrite(DataPin, INPUT_PULLUP);
  if (SoftPullUpClkPin)
    digitalWrite(ClkPin, INPUT_PULLUP);
  if ((SwPin != NO_PIN) && (SoftPullUpSwPin))
    digitalWrite(SwPin, INPUT_PULLUP);

  UsesHardwareInterrupt = false;
  HardwareInterruptDelta = NULL;
  if (DataPin == 2)
  {
    attachInterrupt(0, HWInterrupt0, CHANGE);
    UsesHardwareInterrupt = true;
    HardwareInterruptDelta = &DeltaInterrupt0;
    ClkPinInterrupt0 = ClkPin;
  }
  if (DataPin == 3)
  {
    attachInterrupt(1, HWInterrupt1, CHANGE);
    UsesHardwareInterrupt = true;
    HardwareInterruptDelta = &DeltaInterrupt1;
    ClkPinInterrupt1 = ClkPin;
  }
}

void CRotaryEncoder::Run()
{
  if (!UsesHardwareInterrupt)
    Delta += GetDelta(DataPin, ClkPin, &LastState);

  if (SwPin != NO_PIN)
  {
    bool CurrentButtonState = (digitalRead(SwPin) == LOW);
    if (CurrentButtonState != ButtonState)
    {
      ButtonState = CurrentButtonState;
      if (ButtonState)
        ButtonDelta++;
    }
  }
}

long CRotaryEncoder::GetRotaryDelta()
{
  if (UsesHardwareInterrupt)
  {
    noInterrupts();    
    long Result = *HardwareInterruptDelta; 
    *HardwareInterruptDelta = 0;
    interrupts();
    return Result;
  }
  else
  {
    long Result = Delta; 
    Delta = 0;
    return Result;
  }  
}

long CRotaryEncoder::GetButtonDelta()
{
  if (SwPin != NO_PIN)
  {
    long Result = ButtonDelta; 
    ButtonDelta = 0;
    return Result;
  }
  return 0;
}


