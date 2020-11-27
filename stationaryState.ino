

void stationaryState()
{
  desiredFloor = findNextDesiredFloor();
  if(analogRead(0) >= 1023/2)
  {
    stationary = false;
    moving = true;
  }
  //Serial.println("Stationary state");
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
  while(iterator < (*queuePointer).getSize())
  {
    if(multiplier * *((*queuePointer).getArrayPointer()+iterator) >= multiplier * currentFloor)
    {
      desiredFloorFound = true;
      break;
    }
    iterator++;
  }
  if(desiredFloorFound)
  {
    _desiredFloor = *((*queuePointer).getArrayPointer()+iterator);
  }
  else
  {
    if(currentFloor != 0)
    {
      movingUp = false;
    }
    iterator = 0;
  }
  return _desiredFloor;
}
