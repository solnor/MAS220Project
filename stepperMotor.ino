#include <dac.h>

// Stepper-driver
const uint8_t A_PHASE = 68
            , A_ENABLE = 69
            , B_PHASE = 66
            , B_ENABLE = 67;

// Stepper driving
uint8_t stepA_CCW[] = {0, 0, 1, 1, 1, 0, 0, 0}
      , stepB_CCW[] = {0, 0, 0, 0, 1, 1, 1, 0}
      , dacA_CCW[] = {1, 0, 1, 1, 1, 0, 1, 1}
      , dacB_CCW[] = {1 ,1, 1, 0, 1, 1, 1, 0}
      , stepA_CW[] = {0, 0, 0, 1, 1, 1, 0, 0}
      , stepB_CW[] = {0, 1, 1, 1, 0, 0, 0, 0}
      , dacA_CW[] = {1, 1, 0, 1, 1, 1, 0, 1}
      , dacB_CW[] = {0, 1, 1, 1, 0, 1, 1, 1};

uint8_t stepCounter = 0;
unsigned long prevTime = 0;
const unsigned int stepInterval = 6000; // Microseconds between each step, lower value increases speed
int steps = 0;


void runStepper(int open_close) { /* Runs stepper motor until desired position is reached */
  if (micros() - prevTime > stepInterval) {
    prevTime = micros();

    // Clockwise / OPEN
    if (open_close == OPEN && !doorOpen) {
      if ( stepCounter < 8) {
        digitalWrite(A_PHASE, stepA_CW[stepCounter]);
        digitalWrite(B_PHASE, stepB_CW[stepCounter]);
        digitalWrite(A_ENABLE, dacA_CW[stepCounter]);
        digitalWrite(B_ENABLE, dacB_CW[stepCounter]);
        stepCounter++;
        steps++;
        if ( steps == 400) {
          doorOpen = true;
        }
        if ( stepCounter == 8) {
          stepCounter = 0;
        }
      }
    }

    // Counterclockwise / CLOSE
    if (open_close == CLOSE && doorOpen) {
      if ( stepCounter < 8) {
        digitalWrite(A_PHASE, stepA_CCW[stepCounter]);
        digitalWrite(B_PHASE, stepB_CCW[stepCounter]);
        digitalWrite(A_ENABLE, dacA_CCW[stepCounter]);
        digitalWrite(B_ENABLE, dacB_CCW[stepCounter]);
        stepCounter++;
        steps--;
        if ( steps == 0) {
          doorOpen = false;
        }
        if ( stepCounter == 8) {
          stepCounter = 0;
        }
      }
    }
  }
}


void doorStepperInit(){ /* Initializes stepper motor */
  for (int i = 0; i < 4; i++) {
    pinMode(i + 66, OUTPUT);
  }
  dac_init();
  set_dac(1000, 1000);
  digitalWrite(A_ENABLE, HIGH); // Enables current to be controlled through the phases
  digitalWrite(B_ENABLE, HIGH);

  // The following if-statements make sure the OPEN/CLOSE-function doesn't
  // get stuck in a loop
  if(doorOpen == 0){
    steps = 0;
  }
  if(doorOpen == 400){
    steps = 400;
  }
}
