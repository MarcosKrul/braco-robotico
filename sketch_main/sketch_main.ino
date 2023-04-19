#include <Servo.h>

#define DEBUG 1

#define PIN_SERVO_BASE 9
#define PIN_SERVO_MID 10
#define PIN_SERVO_HAND 11

class ServoMotor {
  
private:
  byte defaultAngle;
  byte max;
  byte min;
  Servo servo;

  bool IsAngleValid(byte angle) {
    if (angle >= this->min && angle <= this->max) return true;

    Serial.println();
    Serial.println("ERROR AT: ServoMotor.IsAngleValid");
#if DEBUG
    Serial.print("Angle: ");
    Serial.println(angle);
    Serial.print("Interval: (");
    Serial.print(this->min);
    Serial.print(",");
    Serial.print(this->max);
    Serial.print(")");
#endif
    Serial.println();
    
    return false;
  }

public:
  ServoMotor(byte defaultAngle, byte min, byte max) {
    this->defaultAngle = defaultAngle;
    this->min = min;
    this->max = max;
  }

  void Setup(byte pin) {
    this->servo.attach(pin);
    this->servo.write(this->defaultAngle);
  }

  void Write(byte angle) {
    if (this->IsAngleValid(angle))
      this->servo.write(angle);
  }
};

ServoMotor base = ServoMotor(90, 0, 180);
ServoMotor mid = ServoMotor(90, 0, 90);
ServoMotor hand = ServoMotor(90, 0, 180);

void setup() {
  Serial.begin(9600);
  
  base.Setup(PIN_SERVO_BASE);
  mid.Setup(PIN_SERVO_MID);
  hand.Setup(PIN_SERVO_HAND);
}

void loop() {
}
