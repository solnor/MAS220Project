  // Start engine, make LEDs light up, etc.

int duty = 127;
const int countsPerRev = 64;
float currentAngle = 0.0;
float previousAngle = 0.0;

int p=0;
bool arrivedAtFloor = false;

void movingState(){
  currentFloor = round(motor.getAngle()/360.0);
  ///////////////////////////////
  //Temporary:
  arrivedAtFloor = checkIfArrived();
  if(arrivedAtFloor && currentFloor == desiredFloor)
  {
      Serial.print("Arrived at floor ");Serial.println(currentFloor+1);
      p++;

      stationary = true;
      moving = false;
  }
  ///////////////////////////////
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
