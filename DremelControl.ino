/* DremelControl
 * 
 * Dremel Control program for the Arduino Nano
 *   Controls dremel speed and direction
 *   Uses a pedal to operate the dremel
 *   Option to invert the pedal for more relaxed operation
 *   Status display by RGB LEDs
 * 
 * Author: YeOldOne, 2023
 * 
*/ 

#include <stdlib.h>
#include <Arduino.h>  
#include "Globals.h"
#include "RotaryEncoder.h"
#include "ButtonProcessor.h"
#include "DremelController.h"
#include "MotorOutput.h"
#include "StatusLEDs.h"

#define PIN_ROTARY_ENCODER_DATA_D2 HW_INTERRUPT_PIN_1_D2
#define PIN_ROTARY_ENCODER_CLK_D7 NORMAL_DIGITAL_PIN_1_D7
#define PIN_BUTTON_D8 NORMAL_DIGITAL_PIN_2_D8
#define PIN_PEDAL_D9 NORMAL_DIGITAL_PIN_3_D9
#define PIN_MOTOR_PWM_D11 PWM_PIN_4_D11
#define PIN_MOTOR_IN1_A0 DIGITAL_ANALOG_PIN_1_D14_A0
#define PIN_MOTOR_IN2_A1 DIGITAL_ANALOG_PIN_2_D15_A1
#define PIN_LED_DATA DIGITAL_ANALOG_PIN_3_D16_A2

#define MIN_PWD 10
#define MAX_PWD 255

#define LED_MAX_BIGHTNESS 200

CRotaryEncoder *RotaryEncoder = NULL;
CButtonProcessor *ButtonProcessor = NULL;
CDremelController *DremelController = NULL;
CMotorOutput *MotorOutput = NULL;
CStatusLEDs *StatusLEDs = NULL;

void setup() 
{
  RotaryEncoder = new CRotaryEncoder(PIN_ROTARY_ENCODER_DATA_D2, PIN_ROTARY_ENCODER_CLK_D7, NO_PIN);

  ButtonProcessor = new CButtonProcessor(PIN_BUTTON_D8);

  pinMode(PIN_PEDAL_D9, INPUT); 
  digitalWrite(PIN_PEDAL_D9, INPUT_PULLUP);

  DremelController = new CDremelController();

  MotorOutput = new CMotorOutput(PIN_MOTOR_PWM_D11, PIN_MOTOR_IN1_A0, PIN_MOTOR_IN2_A1, MIN_PWD, MAX_PWD);

  StatusLEDs = new CStatusLEDs(PIN_LED_DATA, LED_MAX_BIGHTNESS);
}
  
void loop() 
{
  //RotaryEncoder
  RotaryEncoder->Run();

  //ButtonProcessor
  ButtonProcessor->Run();

  //DremelController
  if (ButtonProcessor->GetShortPressed()) 
    DremelController->ButtonClick();
    
  if (ButtonProcessor->GetLongPressed()) 
    DremelController->ButtonPress();

  DremelController->ChangeSpeed(RotaryEncoder->GetRotaryDelta());

  DremelController->SetPedal(digitalRead(PIN_PEDAL_D9) == LOW);
 
  DremelController->Run();

  //MotorOutput
  MotorOutput->SetSpeedPerc(DremelController->GetSpeedPerc());

  MotorOutput->SetEnabled(DremelController->GetStatus() == RUNNING);

  MotorOutput->Run();

  //StatusLEDs
  StatusLEDs->SetBlocked(DremelController->GetStatus() == BLOCKED);

  StatusLEDs->SetRunning(DremelController->GetStatus() == RUNNING);
  
  StatusLEDs->SetInverted(DremelController->GetInvertedPedal());

  StatusLEDs->SetSpeed(DremelController->GetSpeedPerc());

  StatusLEDs->Run(); 
}
