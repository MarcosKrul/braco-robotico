#include <ServoMotor.h>


#define DEBUG 1
#define CONST_UART_BUFFER_SIZE 24
#define PIN_SERVO_BASE 9
#define PIN_SERVO_MID 10
#define PIN_SERVO_HAND 11


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
