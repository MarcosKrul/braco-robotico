#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H


#include <Arduino.h>
#include <Servo.h>


class ServoMotor {
  
private:
  byte max;
  byte min;

protected:
  byte defaultAngle;
  byte currentAngle;
  int offset;
  unsigned int delayTime;
  Servo servo;

  bool IsAngleValid(byte);

public:
  ServoMotor(byte,byte,byte,unsigned int=0,int=0);
  void Setup(byte);
  virtual void Write(byte);
};


#endif