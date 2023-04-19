#include <Servo.h>

#define DEBUG 1
#define CONST_UART_BUFFER_SIZE 24
#define PIN_SERVO_BASE 9
#define PIN_SERVO_MID 10
#define PIN_SERVO_HAND 11


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

  bool IsAngleValid(byte angle) {
    if (angle >= this->min && angle <= this->max) return true;

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
  ServoMotor(byte defaultAngle, byte min, byte max, unsigned int delayTime=0, int offset=0) {
    this->defaultAngle = defaultAngle;
    this->min = min;
    this->max = max;
    this->offset = offset;
    this->delayTime = delayTime;
  }

  void Setup(byte pin) {
    this->servo.attach(pin);
    this->servo.write(this->defaultAngle + this->offset);
    this->currentAngle = this->defaultAngle;
  }

  virtual void Write(byte angle) {
    if (angle == this->currentAngle) return;

    if (this->IsAngleValid(angle)) {
      int step = this->currentAngle < angle ? 1 : -1;

      for (
        int i=this->currentAngle ; 
        (this->currentAngle<angle && i<=angle) || (this->currentAngle>angle && i>=angle); 
        i = i+step
      ) {
        this->servo.write(i);
        delay(this->delayTime);
      }

      this->currentAngle = angle;
      this->servo.write(this->currentAngle + this->offset);
    }
  }
};


class ServoMotorNonBlocking : public ServoMotor {

private:
  int targetAngle;
  unsigned long lastWriteMillis;

public:
  ServoMotorNonBlocking(
    byte defaultAngle, byte min, byte max, unsigned int delayTime=0, int offset=0
  ) : ServoMotor(defaultAngle, min, max, delayTime, offset) {
    this->targetAngle = -1;
    this->lastWriteMillis = millis();
  }

  void Write(byte angle) override {
    if (!this->IsAngleValid(angle) || angle == this->currentAngle) 
      return;

    this->targetAngle = angle;
  }

  void loop() {
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
};


char buffer2serialData[CONST_UART_BUFFER_SIZE];
byte angle_base, angle_mid, angle_hand;
ServoMotor base = ServoMotor(90, 0, 180, 10, -5);
ServoMotor mid = ServoMotor(90, 0, 90, 20, 10);
ServoMotor hand = ServoMotor(90, 35, 180);


void uartLoop(void(*)());
void handleUartReceivedData();


void setup() {
  Serial.begin(9600);
  
  base.Setup(PIN_SERVO_BASE);
  mid.Setup(PIN_SERVO_MID);
  hand.Setup(PIN_SERVO_HAND);
}

void loop() {
  uartLoop(handleUartReceivedData);
  delay(2);
}

void uartLoop(void(*cb)()) {
  int static index = 0;
  bool static hasData = false;

  if (Serial.available()) {
    hasData = true;
    buffer2serialData[index] = Serial.read();
    if ((index += 1) >= CONST_UART_BUFFER_SIZE) index = CONST_UART_BUFFER_SIZE-1; 
  }
  else {
    if (hasData) {
      buffer2serialData[index] = '\0';
      cb();
    }
    index = 0;
    hasData = false;
  }
}

void handleUartReceivedData() {
  char *token = strtok(buffer2serialData, ",");
  angle_base = atoi(token);

  token = strtok(NULL, ",");
  angle_mid = atoi(token);
  
  token = strtok(NULL, ",");
  angle_hand = atoi(token);

  base.Write(angle_base);
  mid.Write(angle_mid);
  hand.Write(angle_hand);
  
#if DEBUG
  Serial.print("Valores recebidos pela serial: ");
  Serial.print(angle_base);
  Serial.print(", ");
  Serial.print(angle_mid);
  Serial.print(", ");
  Serial.println(angle_hand);
#endif
}
