#ifndef SERVO_MOTOR_NON_BLOCKING_H
#define SERVO_MOTOR_NON_BLOCKING_H

#include <Arduino.h>
#include "ServoMotor.h"


class ServoMotorNonBlocking : public ServoMotor {

private:
  int targetAngle;
  unsigned long lastWriteMillis;

public:
  ServoMotorNonBlocking(byte,byte,byte,unsigned int=0,int=0);
  void Write(float) override;
  void loop();
};


#endif