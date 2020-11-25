#include <motorLib.h>

#define outputA 18
#define outputB 19
#define button 2

int buttonPins[] = {29, 28, 27, 26, 25, 24, 23, 22, 18, 19, 2}; // All inputs
int buttonPinsSize;
int ledPins[] = {42, 43, 44, 45, 46, 47, 48, 49}; // All outputs
int ledPinsSize;

//encoder variables
int counter = 0;
int aState;
int aLastState;
int encoderState = 0;
int desiredFloor;
int Q = 1;

bool isIdle = false;
bool isMovingUp = false;
bool isMovingDown = false;

void setup() {
  Serial.begin(115200);
  initPins();
  //aLastState = digitalRead(outputA);
}

void loop() {
  startTime = micros();
  desiredAngle = (analogRead(0)/1023.0)*360.0;
  motor.setDutyCycle(PID(desiredAngle));
  floorSelection();
  //movingUp();
  //movingDown(); 

  changeStates();
  if(isIdle)
  {
    idleState();
  }
  if(isMovingUp)
  {
    
    movingUp();
  }
  if(isMovingDown)
  {
    movingDown();
  }
  dt = micros() - startTime;
  dt = dt / (1000000);
}

void initPins()
{
  buttonPinsSize = sizeof(buttonPins) / sizeof(buttonPins[0]);
  ledPinsSize = sizeof(ledPins) / sizeof(ledPins[0]);

  
  //Initializes all inputs
  for (int i = 0; i < buttonPinsSize; i++) {

    pinMode(buttonPins[i], INPUT);
  }
  
  //Initializes all outputs
  for (int i = 0; i < ledPinsSize; i++) {

    pinMode(ledPins[i], OUTPUT);
  }
}

void changeStates()
{
  if(digitalRead(buttonPins[0]) == HIGH)
  {
    isIdle = !isIdle;
  }
  if(digitalRead(buttonPins[1]) == HIGH)
  {
    isMovingUp = !isMovingUp;
  }
  if(digitalRead(buttonPins[2]) == HIGH)
  {
    isMovingDown = !isMovingDown;
  }
}
