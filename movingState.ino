  // Start engine, make LEDs light up, etc.

int duty = 127;
const int countsPerRev = 64;
float currentAngle = 0.0;
float previousAngle = 0.0;
float desiredAngle = 0.0; // desired amount of rotation in degrees on the load axle

int desiredFloor = 0;

int p=0;
bool arrivedAtFloor = false;

void movingState(){
//  if(millis() % 500 == 0)
//  {
//    Serial.println(desiredFloor);
//    //Serial.println(round(motor.getAngle()/360.0));
//  }
  if(desiredFloor >= 0)
  {
    desiredAngle = float(desiredFloor) * 360.0;
    motor.setDutyCycle(PID(desiredAngle));
  }
  ///////////////////////////////
  //Temporary:
  currentFloor = round(motor.getAngle()/360.0);
  //NOTE: Hardcoded
  arrivedAtFloor = checkIfArrived();
  if(arrivedAtFloor && currentFloor == desiredFloor)
  {
      motor.setSpeed(0);
      Serial.print("Arrived at floor ");Serial.println(currentFloor+1);
      p++;
      //Remove later
      queueIterator++;
      //
      stationary = true;
      moving = false;
  }
}

bool checkIfArrived()
{
  if(motor.getAngle() >= desiredAngle - 2.0 && motor.getAngle() <= desiredAngle + 2.0)
  {
    return true;
  }
  return false;
}

//ISR
void signal1Change()
{
  motor.updateCounter(signal1);
}
void signal2Change()
{
  motor.updateCounter(signal2);
}
