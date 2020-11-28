// Open/close door - used as arguments to stepperMotor function
const int OPEN = 1
        , CLOSE = 0;

// Timers used to close doors and return to 1st floor (floor 0)
unsigned long initialTime = 0
            , doorTimer = 0;

bool reset = false; // Bool to check whether elevator returns to 1st floor upon request or because of reset


void stationaryState()
{ /* Runs while stationary */
  
  // If the elevator has just reached the desired floor, but hasn't opened the doors:
  if(!doorOpen && arrivedAtFloor)
  {
    // Doesn't open doors if the elevator has automatically moved to the first floor, but deletes floor from queue.
    if(reset)
    {
      downwardsQueue.remove(currentFloor);
      downwardsQueue.sort();
      downwardsQueue.flip();
      arrivedAtFloor = false;
    }
    else
    {
      // Starting timer and open doors.
      initialTime = millis();
      runStepper(OPEN);
    }
  }
  
  if(doorOpen && arrivedAtFloor)
  {
    // Starting doorTimer
    doorTimer = millis();
    Queue *ptrs[] = {&upwardsQueue, &downwardsQueue};
    // Deleting element with value of currentFloor from both queues
    for (int i = 0; i < sizeof(ptrs) / sizeof(ptrs[0]); i++)
    {
      (*ptrs[i]).remove(currentFloor);
      (*ptrs[i]).sort();
    }
    downwardsQueue.flip();

    clearFloorState();
    
    arrivedAtFloor = false;
  }
  
  if(doorOpen && !arrivedAtFloor)
  {
    if(millis() - doorTimer>1000)
    {
      runStepper(CLOSE);
    }
  }
  // Finding next floor to go to after door is closed and switching states
  if(!doorOpen && !arrivedAtFloor)
  {
    if(desiredFloor == currentFloor)
    {
      arrivedAtFloor = true;
    }
    else if (desiredFloor > -1)
    {
      reset = false;
      stationary = false;
      moving = true;
    }
  }
  // If no requests has been made within 10 seconds, the elevator will return to floor zero
  if(millis() - initialTime > 10000 && currentFloor != 0)
  {
    downwardsQueue.insert(0);
    reset = true;
  }
}


int findNextDesiredFloor()
{ /* Returns either the next queue element to go to, or -1 if no fitting element is found */
  int8_t multiplier = 1;
  Queue *queuePointer = 0;

  // Setting direction
  setDirection();

  if(movingUp)
  {
    queuePointer = &upwardsQueue;
    multiplier = 1;
  }
  else
  {
    queuePointer = &downwardsQueue;
    multiplier = -1;
  }
  for(int i = 0; i<(*queuePointer).getSize(); i++)
  {
    // If the pointer points to the upwardsQueue, this checks whether the queue element at the current index is larger than the current floor
    // If the pointer points to the downwardsQueue, the additive inverse of both the current queue element and the current floor 
    // is used to check whether the element is smaller than the current floor
    // The function returns the first queue elements that meets the requirement
    if(multiplier * *((*queuePointer).getArrayPointer()+i) >= multiplier * currentFloor)
    {
      return *((*queuePointer).getArrayPointer()+i);;
    }
  }
  // If no queue element is found, -1 is returned
  return -1;
}


void setDirection()
{ /* Sets the direction for the elevator to go in */
  if(movingUp)
  {
    // If the highest value in upwardsQueue is less than zero, no requests has been made in that direction
    // If the downwardsQueue contains an element above or equal to zero, there are one or more requests in that direcion
    // If the elevator is not in the bottom floor, the elevator switch directions (move down)
    if(upwardsQueue.getHighestValue() < 0 && downwardsQueue.getHighestValue() >= 0 && currentFloor != bottomFloor)
    {
      movingUp = false;
    }
    // If both the upwardsQueue and the downwardsQueue have requests and the elevator is above the highest requested floor in the upwards direction
    // the elevator should switch directions (move down) so that the upwards requests can be dealt with the next time the elevator goes upwards again
    else if(upwardsQueue.getHighestValue() >= 0 && downwardsQueue.getHighestValue() >= 0 && currentFloor > upwardsQueue.getHighestValue())
    {
      movingUp = false;
    }
    // If none of these conditions are met, leave the movingUp boolean as is (true)
  }
  else if(!movingUp)
  {
    // If the upwardsQueue has requests while the downwardsQueue do not and the elevator is not on the top floor, the elevator should switch directions (move up)
    if(upwardsQueue.getHighestValue() >= 0 && downwardsQueue.getHighestValue() < 0 && currentFloor != topFloor)
    {
      movingUp = true;
    }
    // If both the upwardsQueue and the downwardsQueue have requests and the elevator is beneath the lowest request in the downwardsQueue, the elevator should switch directions (move up)
    else if(upwardsQueue.getHighestValue() >= 0 && downwardsQueue.getHighestValue() >= 0 && currentFloor < downwardsQueue.getHighestValue())
    {
      movingUp = true;
    }
  }
}
