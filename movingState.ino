int duty = 127;
const int countsPerRev = 64;
float currentAngle = 0.0;
float previousAngle = 0.0;
float desiredAngle = 0.0; // desired amount of rotation in degrees on the load axle


unsigned long startTime = 0;
float dt = 10000;
float thetaDot = 0;
float accumulatedError = 0;
float error = 0;
float theta = 0;
float thetaPrev = 0;
float P, I, D;
float sumOutput;
float filteredD;
float kP = 3,  kI = 0, kD = 0.010;
float PLimit = 40, ILimit = 10,  DLimit = 40;




void movingState(){
  // Start engine, make LEDs light up, etc.

  startTime = micros();
  Serial.println("Moving");
  desiredAngle = (analogRead(0)/1023.0)*360.0;
  motor.setDutyCycle(PID(desiredAngle));
  dt = micros() - startTime;
  dt = dt / (1000000);
}


int PID(float angleInput) { //Returns a value between 0 and 255;
  currentAngle = motor.getAngle();
  //Turns the motor off when the error is less than 1.5 degrees
  if(abs(desiredAngle - currentAngle) < 1.5){
    digitalWrite(enablePin, LOW);
  }else{
  digitalWrite(enablePin, HIGH);
  }

  //PID-calculations:
  P = (desiredAngle - currentAngle) * kP;
  if (P > PLimit) {
    P = PLimit;
  }
  if (P < -PLimit) {
    P = - PLimit;
  }
  I += (desiredAngle - currentAngle) * kI * dt;
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


//ISR
void signal1Change()
{
  motor.updateCounter(signal1);
}
void signal2Change()
{
  motor.updateCounter(signal2);
}
