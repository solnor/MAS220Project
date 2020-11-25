//encoder variables
int counter = 0;
int aState;
int aLastState;
int encoderState = 0;
int Q = 1;

void floorSelection() {
  int desiredFloor = -1;
  
  aState = digitalRead(outputA);
  //Code picks up that the encoder is rotated and which way it rotates 
  if (aState != aLastState) {

    if (digitalRead(outputB) != aState) {
      counter ++;
    } else {
      counter --;
    }
    //Steps between each output, for a more accurate read off
    if (counter == 10) {
      Serial.println("1st floor");
      encoderState = 0;
    }
    else if (counter == 20) {
      Serial.println("2nd floor");
      encoderState = 1;
    }
    else if (counter == 30) {
      Serial.println("3rd floor");
      encoderState = 2;
    }
    else if (counter == 40) {
      Serial.println("4th floor");
      encoderState = 3;
    }
    if (counter >= 45) {
      counter = 41;
    }
    else if (counter <= 5) {
      counter = 9;
    }
  }
  aLastState = aState;

  //Enables the button to choose what floor you want to go to
  if (digitalRead(button) == 1 && Q == 1) {
    desiredFloor = encoderState;
    Serial.print("you chose the ");

    if (desiredFloor == 0) {
      Serial.println("1st floor");
      Q = 0;
    }
    else if (desiredFloor == 1) {
      Serial.println("2nd floor");
      Q = 0;
    }
    else if (desiredFloor == 2) {
      Serial.println("3rd floor");
      Q = 0;
    }
    else {
      Serial.println("4th floor");
      Q = 0;
    }
    
    queueInsert(desiredFloor);
  }
  
  if (digitalRead(button) == 0 && Q == 0) {
    Q = 1;
  }
}

void queueInsert(int desiredFloor)
{
  if(movingUp)
  {
    upwardsQueue.insert(desiredFloor);
    upwardsQueue.sort();
    for(int i = 0; i < upwardsQueue.getSize(); i++)
    {
      Serial.print(*(upwardsQueue.getArrayPointer()+i));
    }
    Serial.println();
  }
}
