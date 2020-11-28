#include <motorLib.h>
#include <Queue.h>


unsigned long startTime = 0; // Used to calculate dt
float dt = 0; // The time it takes for the main loop to run

// Creating a Motor object which runs the DC-motor.
const uint8_t phasePin = 6
            , enablePin = 7
            , signal1 = 21
            , signal2 = 20
            , gearRatio = 131;
Motor motor(phasePin, enablePin, signal1, signal2);

// Creating two objects from the Queue class. These are used to handle requests
Queue upwardsQueue
    , downwardsQueue;

// Declaration of arrays containing pins used
const uint8_t buttonPins[] = {29, 28, 27, 26, 25, 24, 23, 22, 18, 19, 2} // All inputs
            , ledPins[] = {42, 43, 44, 45, 46, 47, 48, 49}; // All LEDs
uint8_t buttonPinsSize = 0
      , ledPinsSize = 0;

// States
bool stationary = true
    , moving = false
    , movingUp = true;

// Current floor, top and bottom floor
uint8_t currentFloor = 0
      , bottomFloor = 0
      , topFloor = 3;

int8_t desiredFloor = 0; // Desired floor. Not an unsigned integer since it will be negative if no desired floor is found

float desiredAngle = 0.0 // Desired amount of rotation in degrees on the load axle
    , degreesPerFloor = 360.0 // Amount of degrees to rotate until next floor is reached. This is arbitrary and can be set to anything.
    , angleTolerance = 1.5; // The acceptable inaccuracy in the motor in degrees

bool doorOpen = false; // False if closed, true if open. It is assumed the door starts off as closed

bool arrivedAtFloor = false; // Boolean set true when a desired floor is reached


void setup() 
{
  Serial.begin(115200);
  // Initializing pins, DC-motor and stepper motor
  initPins();
  initMotor();
  doorStepperInit();
}


void loop() 
{
  startTime = micros();

  // Checking for internal- and external floor requests
  internalFloorSelection();
  externalFloorSelection();
  externalRequestHandler();
  // Setting desiredFloor
  desiredFloor = findNextDesiredFloor();
  if(desiredFloor >= 0 && !doorOpen)
  {
    // Converting desired floor to a desired angle, passing this as an argument to the 
    // PID controller and setting the duty cycle to the output of the controller
    desiredAngle = float(desiredFloor) * degreesPerFloor;
    motor.setDutyCycle(PID(desiredAngle));
  }
  // Displaying the elevator's position with LEDs.
  elevatorIndicator();

  // Running different code based on states
  if(stationary)
  {
    stationaryState();
  }
  if(moving)
  {
    movingState();
  }  

  dt = (micros() - startTime) * pow(10, -6);
}


void initPins()
{ /* Initializes Arduino Mega 2560 pins */
  buttonPinsSize = sizeof(buttonPins) / sizeof(buttonPins[0]);
  ledPinsSize = sizeof(ledPins) / sizeof(ledPins[0]);

  // Initializes all inputs
  for (int i = 0; i < buttonPinsSize; i++) {

    pinMode(buttonPins[i], INPUT);
  }
  
  // Initializes all outputs
  for (int i = 0; i < ledPinsSize; i++) {

    pinMode(ledPins[i], OUTPUT);
  }
}


void initMotor()
{ /* Initializes DC motor */
  // Setting up interrupts for the two signals from the DC motor encoder
  attachInterrupt(digitalPinToInterrupt(signal1), signal1Change, CHANGE);
  attachInterrupt(digitalPinToInterrupt(signal2), signal2Change, CHANGE);
  
  motor.gearRatio = gearRatio;
  // Setting prescaler to one on the ATmega2560, which gives the highest PWM-frequency
  TCCR4B = 0b00000001;
}
