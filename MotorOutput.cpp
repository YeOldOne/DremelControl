#include "MotorOutput.h"

CMotorOutput::CMotorOutput(uint8_t ENAPin, uint8_t IN1Pin, uint8_t IN2Pin, uint8_t MinPWM, uint8_t MaxPWM)
{
  this->ENAPin = ENAPin;
  this->IN1Pin = IN1Pin;
  this->IN2Pin = IN2Pin;
  this->MinPWM = MinPWM;
  this->MaxPWM = MaxPWM;

  Enabled = false;

  Speed = 0;
  NewSpeed = 0;

  Status = MOTOR_IDLING;

  pinMode(ENAPin, OUTPUT); 
  pinMode(IN1Pin, OUTPUT); 
  pinMode(IN2Pin, OUTPUT); 

  analogWrite(ENAPin, 0);
  digitalWrite(IN1Pin, LOW);
  digitalWrite(IN2Pin, LOW);
}

void CMotorOutput::SetSpeedPerc(int Speed)
{
  NewSpeed = Speed;
}

void CMotorOutput::SetEnabled(bool Enabled)
{
  this->Enabled = Enabled;
}

void CMotorOutput::SetMotorSpeed()
{
  int AbsSpeed = Speed;

  if (AbsSpeed < 0)
    AbsSpeed *= -1;  

  float Value = MaxPWM - MinPWM;
  Value = Value * (AbsSpeed / 100.0);
  Value += MinPWM;

  analogWrite(ENAPin, round(Value));
}

void  CMotorOutput::SetMotorDirection()
{
  switch (Status)
  {
    case MOTOR_IDLING:
      digitalWrite(IN1Pin, LOW);
      digitalWrite(IN2Pin, LOW);
      break;
    case MOTOR_BRAKING:
      digitalWrite(IN1Pin, HIGH);
      digitalWrite(IN2Pin, HIGH);
      break;
    case MOTOR_RUNNING:  
      if (Speed >= 0)
      {
        digitalWrite(IN1Pin, HIGH);
        digitalWrite(IN2Pin, LOW);
      }
      else
      {
        digitalWrite(IN1Pin, LOW);
        digitalWrite(IN2Pin, HIGH);
      }
      break;
  }
}

void CMotorOutput::Run()
{
  switch (Status)
  {
    case MOTOR_IDLING:
      if (Enabled)
      {
        Speed = NewSpeed;
      
        Status = MOTOR_RUNNING;

        SetMotorSpeed();

        SetMotorDirection();
      }
      break;
    case MOTOR_BRAKING:
      if (millis() - BrakeStart >= MOTOR_BREAK_MILLIS)
      {
        if (!Enabled)
        {
          Status = MOTOR_IDLING;

          SetMotorDirection();
        }
        else
        {
          Speed = NewSpeed;

          Status = MOTOR_RUNNING;

          SetMotorSpeed();

          SetMotorDirection();
        }
      }
      break;
    case MOTOR_RUNNING:  
      if (!Enabled)
      {
        Status = MOTOR_BRAKING;

        BrakeStart = millis();

        SetMotorDirection();
      }
      else if (NewSpeed != Speed)
      {
        bool ChangingDirection = ((NewSpeed >= 0) && (Speed < 0)) || ((NewSpeed < 0) && (Speed >= 0));

        if (!ChangingDirection)  
        {
          Speed = NewSpeed;
        
          SetMotorSpeed();
        }
        else
        {
          Status = MOTOR_BRAKING;

          BrakeStart = millis();

          SetMotorDirection();
        }
      }
      break;
  }
}
