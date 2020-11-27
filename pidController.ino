//unsigned long startTime = 0;
//float dt = 10000;
float thetaDot = 0;
float accumulatedError = 0;
float error = 0;
float theta = 0;
float thetaPrev = 0;
float P, I, D;
float sumOutput;
float filteredD;
float kP = 3,  kI = 0, kD = 0.010;
float PLimit = 35, ILimit = 10,  DLimit = 40;


int PID(float angleInput) { //Returns a value between 0 and 255;
  currentAngle = motor.getAngle();
  //Turns the motor off when the error is less than 1.5 degrees
  if(abs(angleInput - currentAngle) < 1.5){
    motor.powerOff();
  }else{
  motor.powerOn();
  }

  //PID-calculations:
  P = (angleInput - currentAngle) * kP;
  if (P > PLimit) {
    P = PLimit;
  }
  if (P < -PLimit) {
    P = - PLimit;
  }
  I += (angleInput - currentAngle) * kI * dt;
  if (I > ILimit) {
    I = ILimit;
  }
  if (I < -ILimit) {
    I = - ILimit;
  }
  D = -kD * (currentAngle - previousAngle) / dt;
  filteredD = 0.9*filteredD + 0.1*D;
  previousAngle = currentAngle;
  if (filteredD > DLimit) {
    filteredD = DLimit;
  }
  if (filteredD < -DLimit) {
    filteredD = - DLimit;
  }
  sumOutput = P + I + filteredD + 127;
  if (sumOutput > 255) {
    sumOutput = 255;
  }
  if (sumOutput < 0) {
    sumOutput = 0;
  }
  return sumOutput;
}
