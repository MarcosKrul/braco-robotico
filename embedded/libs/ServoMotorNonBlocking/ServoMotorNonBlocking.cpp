#include "ServoMotorNonBlocking.h"

ServoMotorNonBlocking::ServoMotorNonBlocking(
  byte defaultAngle, byte min, byte max, unsigned int delayTime=0, int offset=0
) : ServoMotor(defaultAngle, min, max, delayTime, offset) {
  this->targetAngle = -1;
  this->lastWriteMillis = millis();
}

void ServoMotorNonBlocking::Write(byte angle) {
  if (!this->IsAngleValid(angle) || angle == this->currentAngle) 
    return;

  this->targetAngle = angle;
}

void ServoMotorNonBlocking::loop() {
  if (this->targetAngle == -1) return;

  int step = this->currentAngle < this->targetAngle ? 1 : -1;

  if (millis() > this->lastWriteMillis + this->delayTime) {
    this->servo.write(this->currentAngle += step);
    this->lastWriteMillis = millis();
  }

  if (this->currentAngle == this->targetAngle) {
    this->targetAngle = -1;
    this->servo.write(this->currentAngle + this->offset);
  }
}