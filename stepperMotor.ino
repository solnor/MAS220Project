#include <dac.h>

//Stepper-driver
const int A_PHASE = 68;
const int A_ENABLE = 69;
const int B_PHASE = 66;
const int B_ENABLE = 67;


//Stepper driving
int stepA_CCW[] = {0, 0, 1, 1, 1, 0, 0, 0};
int stepB_CCW[] = {0, 0, 0, 0, 1, 1, 1, 0};
int dacA_CCW[] = {1, 0, 1, 1, 1, 0, 1, 1};
int dacB_CCW[] = {1 ,1, 1, 0, 1, 1, 1, 0};
int stepA_CW[] = {0, 0, 0, 1, 1, 1, 0, 0};
int stepB_CW[] = {0, 1, 1, 1, 0, 0, 0, 0};
int dacA_CW[] = {1, 1, 0, 1, 1, 1, 0, 1};
int dacB_CW[] = {0, 1, 1, 1, 0, 1, 1, 1};
int i = 0;
unsigned long prevTime = 0;
long stepInterval = 8000; //Microseconds between each step, lower value increases speed
int steps = 0;

//void loop() {
//runStepper(OPEN); //OPEN turns the stepper one rev, CLOSE turns it back on rev.
//}




void runStepper(int open_close) {
  if (micros() - prevTime > stepInterval) {
    prevTime = micros();

    //Clockwise
    if (open_close == OPEN && doorState == 0) {
      if ( i < 8) {
        digitalWrite(A_PHASE, stepA_CW[i]);
        digitalWrite(B_PHASE, stepB_CW[i]);
        digitalWrite(A_ENABLE, dacA_CW[i]);
        digitalWrite(B_ENABLE, dacB_CW[i]);
        i++;
        steps++;
        if ( steps == 400) {
          doorState = true;
        }
        if ( i == 8) {
          i = 0;
        }
      }
    }

    //Counterclockwise
    if (open_close == CLOSE && doorState == 1) {
      if ( i < 8) {
        digitalWrite(A_PHASE, stepA_CCW[i]);
        digitalWrite(B_PHASE, stepB_CCW[i]);
        digitalWrite(A_ENABLE, dacA_CCW[i]);
        digitalWrite(B_ENABLE, dacB_CCW[i]);
        i++;
        steps--;
        if ( steps == 0) {
          doorState = false;
        }
        if ( i == 8) {
          i = 0;
        }
      }
    }
  }
}



void doorStepperInit(){
    for (int i = 0; i < 4; i++) {
    pinMode(i + 66, OUTPUT);
  }
  dac_init();
  set_dac(1000, 1000);
  digitalWrite(A_ENABLE, HIGH); //Enables current to be controlled through the phases
  digitalWrite(B_ENABLE, HIGH);

  //The following if-statements make sure the OPEN/CLOSE-function doesn't
  //get stuck in a loop
  if(doorState == 0){
    steps = 0;
  }
  if(doorState == 400){
    steps = 400;
  }
}
