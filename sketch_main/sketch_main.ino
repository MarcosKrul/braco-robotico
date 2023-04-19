#include <Servo.h>

#define DEBUG 1
#define CONST_UART_BUFFER_SIZE 24
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


char buffer2serialData[CONST_UART_BUFFER_SIZE];
byte angle_base, angle_mid, angle_hand;
ServoMotor base = ServoMotor(90, 0, 180);
ServoMotor mid = ServoMotor(90, 0, 90);
ServoMotor hand = ServoMotor(90, 55, 180);


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
  delay(50);
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
