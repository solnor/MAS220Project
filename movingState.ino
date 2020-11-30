

void movingState()
{ /* Runs while elevator is moving */
  // Setting currentFloor based on the current motor angle. Rounding to whichever floor is the closest.
  currentFloor = round(motor.getAngle()/degreesPerFloor);
  
  // If the motor is at the desired angle and the elevator is on the desired floor, switch to the stationary state
  if(checkIfArrived() && currentFloor == desiredFloor)
  {
    Serial.print("Arrived at floor ");Serial.println(currentFloor+1);
    arrivedAtFloor = true;
    stationary = true;
  }
}


bool checkIfArrived()
{ /* Returns either true or false based on whether current motor angle is within acceptable limits from the desiredAngle */
  if(motor.getAngle() >= desiredAngle - angleTolerance && motor.getAngle() <= desiredAngle + angleTolerance)
  {
    return true;
  }
  return false;
}


void elevatorIndicator()
{ /* Sets pin connected to LED corresponding to current floor to HIGH */
  if(movingUp)
  {
    if(currentFloor != 0)
    {
      digitalWrite(ledPins[currentFloor-1], LOW);
    }
    digitalWrite(ledPins[currentFloor], HIGH);
    
  }
  else
  {
    if(currentFloor != 4)
    {
      digitalWrite(ledPins[currentFloor+1], LOW);
    }
    digitalWrite(ledPins[currentFloor], HIGH);
  }
}


// ISR
void signal1Change()
{ /* ISR for changes on signal1 */
  motor.updateCounter(signal1);
}


void signal2Change()
{ /* ISR for changes on signal2 */
  motor.updateCounter(signal2);
}
