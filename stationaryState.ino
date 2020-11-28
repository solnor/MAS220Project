//Open/close door
const int OPEN = 1;
const int CLOSE = 0;
//This only needs to be an int since it will be reset before it reaches 2^32
unsigned int initialTime = 0;
unsigned int doorTimer = 0;
//bool doorState = false; // 0 for closed, 1 for open. With this value set to 0, it is assumed the door starts off as closed
bool reset = false;

void stationaryState()
{
  if(!doorState && arrivedAtFloor && !reset)
  {
    initialTime = millis();
    runStepper(OPEN);
  }
  
  if(doorState && arrivedAtFloor)
  {
    doorTimer = millis();
    Queue *ptrs[] = {&upwardsQueue, &downwardsQueue};
    //Deletes element with value of currentFloor from both queues
    for (int i = 0; i < sizeof(ptrs) / sizeof(ptrs[0]); i++)
    {
      (*ptrs[i]).remove(currentFloor);
      (*ptrs[i]).sort();
    }
    downwardsQueue.flip();
    arrivedAtFloor = false;
    clearFloorState();
  }
  if(doorState && !arrivedAtFloor)
  {
    if(millis() - doorTimer>1000)
    {
      runStepper(CLOSE);
    }
  }

  if(!doorState && !arrivedAtFloor)
  {
    //desiredFloor = findNextDesiredFloor();
    if(desiredFloor == currentFloor)
    {
      arrivedAtFloor = true;
    }
    else if (desiredFloor > -1)
    {
      stationary = false;
      moving = true;
    }
  }
  if(millis() - initialTime > 10000 && currentFloor != 0)
  {
    downwardsQueue.insert(0);
    reset = true;
  }
}
// Enable door movement, start timer

  //if desiredDoorPosition != currentDoorPosition:
    //move stepper motor
  //if desiredDoorPosition == currentDoorPosition && !doorsOpen:
    //doorsOpen = true
    //Starting timer = startTime
    //Clear queue element - from both queueueueues
  //if doorsOpen:
    //if currentTime - startTime > 3000 ms || floorSelected:
      //if not closed:
        //move stepper motor
      //if desiredDoorPosition == currentDoorPosition && doorsOpen:
        //doorsOpen = false;
    //else:
      //checkForFloorSelection - bool func?
  //if !checkForFloorSelection && currentTime - startTime > 10000 ms:
    //if doorsOpen:
      //close doors
    //else:
      //move to first floor
      
  
//Loop through queue and check whether current element values are the same as the last time it was checked. If they aren't the same, reset "startTime"

    //CHECK IF DC MOTOR MOVES WHEN TURNED OFF -> TURN ON MOTOR AND GO BACK TO DESIRED FLOOR
// Enable door movement, start timer


int findNextDesiredFloor()
{
  int _desiredFloor = -1;
  bool desiredFloorFound = false;
  int multiplier = 1;
  Queue *queuePointer = 0;

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
    if(multiplier * *((*queuePointer).getArrayPointer()+i) >= multiplier * currentFloor)
    {
      return *((*queuePointer).getArrayPointer()+i);;
    }
  }
  return -1;
}

void setDirection()
{
  if(movingUp)
  {
    
    if(upwardsQueue.getHighestValue() < 0 && downwardsQueue.getHighestValue() >= 0 && currentFloor != 0)
    {
      movingUp = false;
    }
    else if(upwardsQueue.getHighestValue() >= 0 && downwardsQueue.getHighestValue() >= 0 && currentFloor > upwardsQueue.getHighestValue())
    {
      movingUp = false;
    }
  }
  else if(!movingUp)
  {
    if(upwardsQueue.getHighestValue() >= 0 && downwardsQueue.getHighestValue() < 0 && currentFloor != 4)
    {
      movingUp = true;
    }
    else if(upwardsQueue.getHighestValue() >= 0 && downwardsQueue.getHighestValue() >= 0 && currentFloor < downwardsQueue.getHighestValue())
    {
      movingUp = true;
    }
  }
}
