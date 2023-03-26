/* DremelController
 * 
 * Controller class for the dremel
 *   Inceases and decreases speed between -100% and 100%
 *   Pedal controlled running
 *   Option to invert pedal down for more relaxed operation
 *   Blocks to prevent unexpected running without pedal control
 * 
 * Operation
 *   ButtonClick and ButtonPress while running puts the contoller immediately in blocked mode
 *   ButtonPress while not running inverts the operation of the pedal, if this would set the controller into running mode the controller goes into blocked mode instead
 *     (This ensures running mode is only activated by operating the pedal)
 *   Blocked mode can be cancelled by operating the pedal
 * 
 * Author: YeOldOne, 2023
 * 
*/ 
 
#ifndef DREMELCONTROLLER_H
#define DREMELCONTROLLER_H

#include <Arduino.h>  

typedef enum {
  BLOCKED,
  IDLE,
  RUNNING
} dremel_control_status_t; 

#define DREMEL_CONTROL_STEP_SIZE 5

String DremelControllerStatusText(dremel_control_status_t Status);

class CDremelController
{ 
  private: 
    int SpeedPerc;
    dremel_control_status_t Status;
    bool PedalDown;
    bool InvertedPedal;
  protected:
  public: 
    CDremelController();
    void Run();
    void ButtonClick();
    void ButtonPress();
    void ChangeSpeed(int Delta);
    void SetPedal(bool Down);
    dremel_control_status_t GetStatus();
    int GetSpeedPerc();
    bool GetInvertedPedal();
};

#endif
