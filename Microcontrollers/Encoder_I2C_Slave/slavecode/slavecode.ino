// Rotary Encoder Inputs
#define CLK 3
#define DT 2
#include <Wire.h>

byte i2c_rcv;                  // data received from I2C bus

long counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";

int motor_en = 9;
int motor_pin1 = 8;
int motor_pin2 = 7;
char motor_direction = 'x';
byte storage[4];

long start_time;

void setup() {
  
  Wire.begin(8);                // join i2c bus with address #8
  //Wire.onReceive(dataRcv);             //even handler for received data
  Wire.onRequest(dataReq);      // register event
  
  // Set encoder pins as inputs
  pinMode(CLK,INPUT_PULLUP);
  pinMode(DT,INPUT_PULLUP);



    // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);

  // Call updateEncoder() when any high/low changed seen
  // on interrupt 0 (pin 2), or interrupt 1 (pin 3)
  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);
}

void loop() {

  updateEncoder();
  Wire.onRequest(dataReq);

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
    } else {
      // Encoder is rotating CW so increment
      counter ++;
    }
  }
  // Remember last CLK state
  lastStateCLK = currentStateCLK;
  
}



void dataReq() {
    storage[0] = (byte) counter;
    storage[1] = (byte) counter >> 8;
    storage[2] = (byte) counter >> 16;
    storage[3] = (byte) counter >> 24;
    Wire.write(storage, 4); // respond with the counter from updateEncoder
    Serial.print(counter);
}
