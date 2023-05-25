#include "ServoMotor.h"

bool ServoMotor::ValidateAngle(float* angle) {
  if (*angle >= this->min && *angle <= this->max) return true;

  Serial.println("ERROR AT: ServoMotor.IsAngleValid");
  Serial.print("Angle: ");
  Serial.println(*angle);
  Serial.print("Interval: (");
  Serial.print(this->min);
  Serial.print(",");
  Serial.print(this->max);
  Serial.print(")");
  Serial.println();

  if (*angle < this->min) *angle = this->min;
  if (*angle > this->max) *angle = this->max;

  return false;
}

ServoMotor::ServoMotor(byte defaultAngle, byte min, byte max, unsigned int delayTime=0, int offset=0) {
  this->defaultAngle = defaultAngle;
  this->min = min;
  this->max = max;
  this->offset = offset;
  this->delayTime = delayTime;
}

void ServoMotor::Setup(byte pin) {
  this->servo.attach(pin);
  this->servo.write(this->defaultAngle + this->offset);
  this->currentAngle = this->defaultAngle;
}

void ServoMotor::Write(float angle) {
  if (angle == this->currentAngle) return;

  this->ValidateAngle(&angle);
  int step = this->currentAngle < angle ? 1 : -1;

  for (
    int i=this->currentAngle + 1 ; 
    (this->currentAngle<angle && i<=angle) || (this->currentAngle>angle && i>=angle); 
    i = i+step
  ) {
    this->servo.write(i);
    delay(this->delayTime);
  }

  this->currentAngle = angle;
  this->servo.write(this->currentAngle + this->offset);
}