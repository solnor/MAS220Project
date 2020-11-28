int currentFloor = 0; //Which floor the elevator is currently at
int elevDirection; //-1: down, 0: stationary, 1: up
int buttonState[] = {0, 0, 0, 0, 0, 0};
int buttonInput[] = {0, 0, 0, 0, 0, 0};
int floorState[] = {0, 0, 0, 0};
int potValue;
int n = 0;
int arrayCompensation = 24; //Subtracts from the i-counter used to set outputs


void readButtonsSetState() {
  for (int i = 29; i > 23; i--) {
    buttonInput[i - arrayCompensation] = digitalRead(i);
    /*Serial.print(buttonInput[i]);
      Serial.print(" ");*/
  }
  //These buttons now need to be linked to each floor to put them in required states
  //Floor 0: button 7 for "up"-request
  //Floor 1: button 6 & 5 for "up/down"-request
  //Floor 2: button 4 & 3 for "up/down"-request
  //Floor 3: button 2 for "down"-request
  //Buttons 7-2 maps to pins 29-24;

  //For all floors: Floorstates will be set to 0 once the floor is visited

  //Floor 0:
  if (buttonInput[29 - arrayCompensation] == 1) { // "up" button for floor 0
    buttonState[0] = 2;
  }
  floorState[0] = buttonState[0];

  //Floor 1: This floor has two inputs, so some additional code has to be written
  if (buttonInput[28 - arrayCompensation] == 1) { // The "down"-button, button 6 on the unit
    buttonState[1] = 1;
  }
  if (buttonInput[27 - arrayCompensation] == 1) { // The "up"-button, button 5 on the unit
    buttonState[2] = 1;
  }
  floorState[1] = (buttonState[2]) * 2 + buttonState[1];

  //Floor 2: Duplication of code above (could/should be done in for-loop?)
  if (buttonInput[26 - arrayCompensation] == 1) { // The "down"-button, button 4 on the unit
    buttonState[3] = 1;
  }
  if (buttonInput[25 - arrayCompensation] == 1) { // The "up"-button, button 3 on the unit
    buttonState[4] = 1;
  }
  floorState[2] = (buttonState[4]) * 2 + buttonState[3];

  //Floor 3, mostly same as for floor 0
  if (buttonInput[24 - arrayCompensation] == 1) {
    buttonState[5] = 1; // The "down"-button on floor 3
  }
  floorState[3] = buttonState[5];
}

void clearFloorState() {
  //Floor 0. Consider throwing this into some for loops later
  if ((floorState[0] != 0) && currentFloor == 0) { //Floor 0 has requests, and the elevator has arrived at that floor: req's are cleared
    buttonState[0] = 0;
    floorState[0] = 0;
  }
  //Floor 1
  if ((floorState[1] != 0) && currentFloor == 1) {
    buttonState[1] = 0;
    buttonState[2] = 0;
    floorState[1] = 0;
  }
  //Floor 2
  if ((floorState[2] != 0) && currentFloor == 2) {
    buttonState[3] = 0;
    buttonState[4] = 0;
    floorState[2] = 0;
  }
  //Floor 3
  if ((floorState[3] != 0) && currentFloor == 3) {
    buttonState[5] = 0;
    floorState[3] = 0;
  }

}

void requestHandler()
{
  for(int i = 0; i<sizeof(floorState)/sizeof(floorState[0]); i++)
  {
    if(floorState[i] != 0)
    {
      if(floorState[i] == 1)
      {
        if(upwardsQueue.getHighestValue() < i)
        {
          upwardsQueue.insert(i);
        }
        else
        {
          downwardsQueue.insert(i);
        }
        buttonState[1] = 0;
        buttonState[2] = 0;
        floorState[i] = 0;
        //Serial.println(floorState[i]);
      }
      else if(floorState[i] == 2)
      {
        if(downwardsQueue.getHighestValue() < i)
        {
          downwardsQueue.insert(i);
        }
        else
        {
          upwardsQueue.insert(i);
        }
      }
      else if(floorState[i] == 3)
      {
        downwardsQueue.insert(i);
        upwardsQueue.insert(i);
      }
      downwardsQueue.sort();
      upwardsQueue.sort();
      downwardsQueue.flip();
    }
  }
}
