/* MotorOutput
 * 
 * Controller class for motor output with the L298N motor driver
 *   Takes speed percentage as input in the range of -100% to 100%
 *   Outputs direction and speed signals for the L298N motor driver
 * 
 * L298N
 *   ENA = PWM input
 *   IN1=0 IN2 = 0  ==> Idle
 *   IN1=1 IN2 = 0  ==> Forward
 *   IN1=0 IN2 = 1  ==> Backward
 *   IN1=1 IN2 = 1  ==> Brake (shorts terminals)
 * 
 * Author: YeOldOne, 2023
 * 
*/ 
 
#ifndef MOTOROUTPUT_H
#define MOTOROUTPUT_H

#include <Arduino.h>  

typedef enum {
  MOTOR_IDLING,
  MOTOR_BRAKING,
  MOTOR_RUNNING
} motor_output_status_t ; 

#define MOTOR_BREAK_MILLIS 300

class CMotorOutput
{ 
  private: 
    uint8_t ENAPin;
    uint8_t IN1Pin;
    uint8_t IN2Pin;
    uint8_t MinPWM;
    uint8_t MaxPWM;
    unsigned long BrakeStart;
    bool Enabled;
    int Speed;
    int NewSpeed;
    motor_output_status_t Status;
    void SetMotorSpeed();
    void SetMotorDirection();
  protected:
  public: 
    CMotorOutput(uint8_t ENAPin, uint8_t IN1Pin, uint8_t IN2Pin, uint8_t MinPWM, uint8_t MaxPWM);
    void Run();
    void SetSpeedPerc(int Speed);
    void SetEnabled(bool Enabled);
};

#endif
