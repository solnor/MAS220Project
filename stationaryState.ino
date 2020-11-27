//Open/close door
const int OPEN = 1;
const int CLOSE = 0;

bool doorState = false; // 0 for closed, 1 for open. With this value set to 0, it is assumed the door starts off as closed

void stationaryState()
{
  
  if(analogRead(0) >= 1023/2)
  {
    stationary = false;
    moving = true;
  }
  //Serial.println("Stationary state");
  
  if(!doorState && arrivedAtFloor)
  {
    runStepper(OPEN);
  }
  
  if(doorState && arrivedAtFloor)
  {
    Queue *ptrs[] = {&upwardsQueue, &downwardsQueue};
    //Deletes element with value of currentFloor from both queues
    for (int i = 0; i < sizeof(ptrs) / sizeof(ptrs[0]); i++)
    {
      (*ptrs[i]).remove(currentFloor);
    }
    arrivedAtFloor = false;
    //Also clear external requests from that floor
  }
  if(doorState && !arrivedAtFloor)
  {
    runStepper(CLOSE);
  }

  desiredFloor = findNextDesiredFloor();
  
  if(desiredFloor == currentFloor)
  {
    arrivedAtFloor = true;
  }
  else if (desiredFloor != -1)
  {
    stationary = false;
    moving = true;
  }
  //If request in same floor open doors, if not on same floor, move elevator
  
  
  
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
  while(queueIterator < (*queuePointer).getSize())
  {
    if(multiplier * *((*queuePointer).getArrayPointer()+queueIterator) >= multiplier * currentFloor)
    {
      desiredFloorFound = true;
      break;
    }
    queueIterator++;
  }
  if(desiredFloorFound)
  {
    _desiredFloor = *((*queuePointer).getArrayPointer()+queueIterator);
  }
  else
  {
    if(currentFloor != 0)
    {
      movingUp = false;
    }
    queueIterator = 0;
  }
  return _desiredFloor;
}
