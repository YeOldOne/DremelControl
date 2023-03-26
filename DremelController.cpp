#include "DremelController.h"

String DremelControllerStatusText(dremel_control_status_t Status)
{
  switch (Status)
  {
    case BLOCKED:
      return "BLOCKED";
    case IDLE:
      return "IDLE";
    case RUNNING:
      return "RUNNING";
    default:
      return "";
  }
}

CDremelController::CDremelController()
{
  SpeedPerc = 50;

  Status = IDLE;

  PedalDown = false;

  InvertedPedal = false;
}

void CDremelController::Run()
{
}

void CDremelController::ButtonClick()
{
  if (Status == RUNNING)
    Status = BLOCKED;
}

void CDremelController::ButtonPress()
{
  if (Status == RUNNING)
    Status = BLOCKED;
  else
  {
    InvertedPedal = !InvertedPedal;

    if ((!InvertedPedal) && (!PedalDown) && (Status == BLOCKED))
      Status = IDLE;
  }
}

void CDremelController::ChangeSpeed(int Delta)
{
  if (!Delta)
    return;

  SpeedPerc += (Delta * DREMEL_CONTROL_STEP_SIZE);

  if (SpeedPerc > 100)
    SpeedPerc = 100;

  if (SpeedPerc < -100)
    SpeedPerc = -100;

  if (SpeedPerc == 0)
  {
    if (Delta > 0) 
      SpeedPerc = DREMEL_CONTROL_STEP_SIZE;
    else
      SpeedPerc = -DREMEL_CONTROL_STEP_SIZE;
  }
}

void CDremelController::SetPedal(bool Down)
{
  if (PedalDown == Down)
    return;

  PedalDown = Down;

  if (Status == BLOCKED)  
  {
    //Unblock on pedal up when not inverted
    if ((!PedalDown) && (!InvertedPedal))
      Status = IDLE;

    //Unblock on pedal down when inverted
    if ((PedalDown) && (InvertedPedal))
      Status = IDLE;
  }
  else if (Status == IDLE)
  {
    //Set running if pedal is down when not inverted 
    if ((!InvertedPedal) && (PedalDown))
      Status = RUNNING;
      
    //Set running if pedal is up when inverted 
    if ((InvertedPedal) && (!PedalDown))
      Status = RUNNING;
  }
  else if (Status == RUNNING)
    Status = IDLE;
}

dremel_control_status_t CDremelController::GetStatus()
{
  return Status;  
}

int CDremelController::GetSpeedPerc()
{  
  return SpeedPerc;
}

bool CDremelController::GetInvertedPedal()
{
  return InvertedPedal;
}
