#include <motorLib.h>
#include <Queue.h>

#define outputA 18
#define outputB 19
#define button 2

#define phasePin 6
#define enablePin 7
#define signal1 21
#define signal2 20

Motor motor(phasePin, enablePin, signal1, signal2);
const int gearRatio = 131;

Queue upwardsQueue;
Queue downwardsQueue;

int *queuePointer = 0;
int floorState[] = {0, 0, 0, 0};

int buttonPins[] = {29, 28, 27, 26, 25, 24, 23, 22, 18, 19, 2}; // All inputs
int buttonPinsSize;
int ledPins[] = {42, 43, 44, 45, 46, 47, 48, 49}; // All outputs
int ledPinsSize;

bool idle = false;
bool moving = false;
bool movingUp = true;

void setup() {
  Serial.begin(115200);
  pinMode(0, INPUT);
  initPins();
  initMotor();
  //aLastState = digitalRead(outputA);
}

void loop() {

  floorSelection();
  //movingUp();
  //movingDown(); 

  changeStates();
  if(idle)
  {
    //idleState();
  }
  /*if(isMovingUp)
  {
    
    movingUp();
  }*/
  if(moving)
  {
    movingState();
  }
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

void initMotor()
{
  attachInterrupt(digitalPinToInterrupt(signal1), signal1Change, CHANGE);
  attachInterrupt(digitalPinToInterrupt(signal2), signal2Change, CHANGE);
  motor.gearRatio = gearRatio;
}

void changeStates()
{
  if(digitalRead(buttonPins[0]) == HIGH)
  {
    idle = !idle;
  }
  if(digitalRead(buttonPins[1]) == HIGH)
  {
    moving = !moving;
  }
  /*if(digitalRead(buttonPins[2]) == HIGH)
  {
    isMovingDown = !isMovingDown;
  }*/
}
