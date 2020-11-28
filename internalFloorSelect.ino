// Encoder variables
int counter = 0;
int aState = 0, aLastState = 0;
int encoderState = 0;
int Q = 1;

// Encoder pins
const uint8_t outputA = 18
            , outputB = 19
            , button = 2;


void internalFloorSelection() { /* Checks for internal floor requests */
  aState = digitalRead(outputA);
  // Code picks up that the encoder is rotated and which way it rotates 
  if (aState != aLastState) {

    if (digitalRead(outputB) != aState) {
      counter ++;
    } else {
      counter --;
    }
    // Steps between each output, for a more accurate read off
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

  // Enables the button to choose what floor you want to go to
  if (digitalRead(button) == 1 && Q == 1) {
    Serial.print("You chose the ");

    if (encoderState == 0) {
      Serial.println("1st floor");
      Q = 0;
    }
    else if (encoderState == 1) {
      Serial.println("2nd floor");
      Q = 0;
    }
    else if (encoderState == 2) {
      Serial.println("3rd floor");
      Q = 0;
    }
    else {
      Serial.println("4th floor");
      Q = 0;
    }
    internalRequestHandler(encoderState);
  }
  
  if (digitalRead(button) == 0 && Q == 0) {
    Q = 1;
  }
}


void internalRequestHandler(int value)
{ /* Evalutates the internal requests and puts them in the proper queue */
  // Initiating a pointer of type Queue to an invalid memory address
  Queue *queuePointer = 0;
  
  if(movingUp)
  {
    // If the elevator is moving upwards, but the selected floor is beneath its current position, the floor will be added to the downwardsQueue
    if(value >= currentFloor)
    {
      queuePointer = &upwardsQueue;
    }
    else
    {
      queuePointer = &downwardsQueue;
    }
  }
  else
  {
    if(value <= currentFloor)
    {
      queuePointer = &downwardsQueue;
    }
    else
    {
      queuePointer = &upwardsQueue;
    }
  }
  // Inserting values to queue the pointer points to, before sorting and flipping in the case the pointer points to the downwardsQueue
  (*(queuePointer)).insert(value);
  (*(queuePointer)).sort();
  if(queuePointer == &downwardsQueue)
  {
    downwardsQueue.flip();
  }
}
