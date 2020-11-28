float P = 0
    , I = 0
    , D = 0;
    
float sumOutput = 0;

float filteredD = 0;

// Gains
float kP = 3
    , kI = 0.002
    , kD = 0.010;
    
// Maximum contributions from the regulators
float PLimit = 40
    , ILimit = 10
    , DLimit = 40;
    
float currentAngle = 0.0
    , previousAngle = 0.0;


int PID(float angleInput) { /* Returns a value between 0 and 255 */
  currentAngle = motor.getAngle();
  // Turns the motor off when the error is less than angle tolerance
  if(abs(angleInput - currentAngle) < angleTolerance){
    motor.powerOff();
  }else{
  motor.powerOn();
  }
  // PID-calculations:
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
