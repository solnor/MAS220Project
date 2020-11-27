#include <motorLib.h>
#include <Queue.h>

#define outputA 18
#define outputB 19
#define button 2

#define phasePin 6
#define enablePin 7
#define signal1 21
#define signal2 20

unsigned long startTime = 0;
/*
 * 
 * Kommenter på hvorfor dt er initialisert som 10000;
 * 
 */
float dt = 10000;

Motor motor(phasePin, enablePin, signal1, signal2);
const int gearRatio = 131;

Queue upwardsQueue;
Queue downwardsQueue;
int queueIterator = 0;

//int floorState[] = {0, 0, 0, 0};

int buttonPins[] = {29, 28, 27, 26, 25, 24, 23, 22, 18, 19, 2}; // All inputs
int buttonPinsSize;
int ledPins[] = {42, 43, 44, 45, 46, 47, 48, 49}; // All outputs
int ledPinsSize;

bool stationary = true;
bool moving = false;
bool movingUp = true;

void setup() {
  TCCR4B = TCCR4B & 0b11111000 | 0x01;
  Serial.begin(115200);
  pinMode(0, INPUT);
  initPins();
  initMotor();
  doorStepperInit();
  //aLastState = digitalRead(outputA);
}

void loop() {
  startTime = micros();
  floorSelection();
  readButtonsSetState();

  changeStates();
  if(stationary)
  {
    stationaryState();
  }
  if(moving)
  {
    movingState();
  }
  requestHandler();

//  if(millis() % 500 == 0)
//  {
//    Serial.print("Upwards: ");
//    for(int i = 0; i < upwardsQueue.getSize(); i++)
//    {
//      Serial.print(*(upwardsQueue.getArrayPointer()+i));
//    }
//    Serial.print(" downwards: ");
//    for(int i = 0; i < downwardsQueue.getSize(); i++)
//    {
//      Serial.print(*(downwardsQueue.getArrayPointer()+i));
//    }
//    Serial.println();
//  }
  
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

void initMotor()
{
  attachInterrupt(digitalPinToInterrupt(signal1), signal1Change, CHANGE);
  attachInterrupt(digitalPinToInterrupt(signal2), signal2Change, CHANGE);
  motor.gearRatio = gearRatio;
  motor.powerOff();
}

void changeStates()
{
  /*if(digitalRead(buttonPins[0]) == HIGH)
  {
    stationary = !stationary;
  }
  if(digitalRead(buttonPins[1]) == HIGH)
  {
    moving = !moving;
  }*/
  /*if(digitalRead(buttonPins[2]) == HIGH)
  {
    isMovingDown = !isMovingDown;
  }*/
}
