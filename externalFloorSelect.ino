uint8_t buttonState[] = {0, 0, 0, 0, 0, 0}
      , buttonInput[] = {0, 0, 0, 0, 0, 0}
      , floorState[] = {0, 0, 0, 0};

uint8_t arrayCompensation = 24; // Subtracts from the i-counter used to set outputs


void externalFloorSelection() { /* Checks for external floor requests */
  for (int i = 29; i > 23; i--) {
    buttonInput[i - arrayCompensation] = digitalRead(i);
  }
  // These buttons now need to be linked to each floor to put them in required states
  // Floor 0: button 7 for "up"-request
  // Floor 1: button 6 & 5 for "up/down"-request
  // Floor 2: button 4 & 3 for "up/down"-request
  // Floor 3: button 2 for "down"-request
  // Buttons 7-2 maps to pins 29-24;

  // For all floors: Floorstates will be set to 0 once the floor is visited and the elevator doors are open

  // Floor 0:
  if (buttonInput[29 - arrayCompensation] == 1) { // "up" button for floor 0
    buttonState[0] = 2;
  }
  floorState[0] = buttonState[0];

  // Floor 1: This floor has two inputs, so some additional code has to be written
  if (buttonInput[28 - arrayCompensation] == 1) { // The "down"-button, button 6 on the unit
    buttonState[1] = 1;
  }
  if (buttonInput[27 - arrayCompensation] == 1) { // The "up"-button, button 5 on the unit
    buttonState[2] = 1;
  }
  floorState[1] = (buttonState[2]) * 2 + buttonState[1];

  // Floor 2: Duplication of code above 
  if (buttonInput[26 - arrayCompensation] == 1) { // The "down"-button, button 4 on the unit
    buttonState[3] = 1;
  }
  if (buttonInput[25 - arrayCompensation] == 1) { // The "up"-button, button 3 on the unit
    buttonState[4] = 1;
  }
  floorState[2] = (buttonState[4]) * 2 + buttonState[3];

  // Floor 3, mostly same as for floor 0
  if (buttonInput[24 - arrayCompensation] == 1) {
    buttonState[5] = 1; // The "down"-button on floor 3
  }
  floorState[3] = buttonState[5];

  
  externalRequestHandler();
}


void clearFloorState() { /*Clears floor states*/
  // Floor 0
  if ((floorState[0] != 0) && currentFloor == 0) { // Floor 0 has requests, and the elevator has arrived at that floor: requests are cleared
    buttonState[0] = 0;
    floorState[0] = 0;
  }
  // Floor 1
  if ((floorState[1] != 0) && currentFloor == 1) {
    buttonState[1] = 0;
    buttonState[2] = 0;
    floorState[1] = 0;
  }
  // Floor 2
  if ((floorState[2] != 0) && currentFloor == 2) {
    buttonState[3] = 0;
    buttonState[4] = 0;
    floorState[2] = 0;
  }
  // Floor 3
  if ((floorState[3] != 0) && currentFloor == 3) {
    buttonState[5] = 0;
    floorState[3] = 0;
  }

}


void externalRequestHandler()
{ /* Evalutates the external requests and puts them in the proper queue */
  for(int floor = 0; floor<sizeof(floorState)/sizeof(floorState[0]); floor++)
  {
    if(floorState[floor] != 0)
    {
      // If request for elevator to go downwards:
      if(floorState[floor] == 1)
      {
        // If the requested floor is above the largest value in the upwards queue, the elevator has to ascend to get to it
        if(upwardsQueue.getHighestValue() < floor && currentFloor < floor /*SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS*/)
        {
          upwardsQueue.insert(floor);
        }
        else
        {
          downwardsQueue.insert(floor);
        }
      }
      // If request for elevator to go upwards:
      else if(floorState[floor] == 2)
      {
        // If the requested floor is below the lowest value in the downwards queue, the elevator has to descend to get to it
        if(downwardsQueue.getLowestValue() > floor && currentFloor > floor)
        {
          downwardsQueue.insert(floor);
        }
        else
        {
          upwardsQueue.insert(floor);
        }
      }
      // If there are requests for elevator to go both upwards and downwards:
      else if(floorState[floor] == 3)
      {
        downwardsQueue.insert(floor);
        upwardsQueue.insert(floor);
      }
      // Sorting the queues, and flipping the downwardsQueue
      downwardsQueue.sort();
      upwardsQueue.sort();
      downwardsQueue.flip();
    }
  }
}
