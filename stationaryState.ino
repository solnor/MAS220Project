
void idleState()
{
  Serial.println("Idle State");
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
      
  


    //CHECK IF DC MOTOR MOVES WHEN TURNED OFF -> TURN ON MOTOR AND GO BACK TO DESIRED FLOOR
// Enable door movement, start timer
