// Rotary Encoder Inputs
#define CLK 3
#define DT 2
#include <Wire.h>

byte i2c_rcv;                  // data received from I2C bus

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";

int motor_en = 9;
int motor_pin1 = 8;
int motor_pin2 = 7;
char motor_direction = 'x';

long start_time;

void setup() {
  
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(dataRcv);             //even handler for received data
  Wire.onRequest(requestEvent); // register event
  
  // Set encoder pins as inputs
  pinMode(CLK,INPUT_PULLUP);
  pinMode(DT,INPUT_PULLUP);

  // Powering motor
  pinMode(motor_en, OUTPUT);
  pinMode(motor_pin1, OUTPUT);
  pinMode(motor_pin2, OUTPUT);

    // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);

  // Call updateEncoder() when any high/low changed seen
  // on interrupt 0 (pin 2), or interrupt 1 (pin 3)
  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);
}

void loop() {

  // with master's info, set the motor direction a or b (cw or ccw)
  if (motor_direction == 'a') {
      digitalWrite(motor_pin1, HIGH);
      digitalWrite(motor_pin2, LOW);
  }
  else if (motor_direction == 'b') {
      digitalWrite(motor_pin1, LOW);
      digitalWrite(motor_pin2, HIGH);
  }
  else {
      digitalWrite(motor_pin1, LOW);
      digitalWrite(motor_pin2, LOW);
  }

  // report back to master which direction the motor is spinning
  if (motor_pin1 == HIGH && motor_pin2 == LOW) {  // clockwise
    motor_direction = 'a';
  } 
  else if (motor_pin1 == LOW && motor_pin2 == HIGH) { // counterclockwise
    motor_direction = 'b';
  }
  else {
    motor_direction = 'x';
  }
  
}

void updateEncoder(){
  // Read the current state of CLK
  currentStateCLK = digitalRead(CLK);

  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT) != currentStateCLK) {
      counter --;
      currentDir ="CCW";
    } else {
      // Encoder is rotating CW so increment
      counter ++;
      currentDir ="CW";
    }
  }
  // Remember last CLK state
  lastStateCLK = currentStateCLK;
}

void dataRcv(int numBytes) {
  while(Wire.available()) {            // read all bytes received
    motor_direction = Wire.read();
    Serial.println(motor_direction);
  }
  Serial.println("Hello");
}

void requestEvent() {
    Wire.write(motor_direction); // respond with message of 1 byte (a or b for cw/ccw)
}
