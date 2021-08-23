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
char button;

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

  // Setup Serial Monitor
  Serial.begin(9600);
  delay(2000);
  Serial.println("Type motor direction (a/b/n) and speed (0 to 255):");
  start_time = millis();
  
  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);

  // Call updateEncoder() when any high/low changed seen
  // on interrupt 0 (pin 2), or interrupt 1 (pin 3)
  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);
}

void loop() {
  long cur_time = millis();
  if (cur_time >= start_time + 5000) {
    start_time = cur_time;
    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);
  }
  
  // Motor turning
  if(Serial.available()){
    String command = Serial.readString();
    char *motor_direction = strtok(command.c_str(), " ");
    //Serial.println(motor_direction);
    int motor_speed = atoi(strtok(NULL, " "));
    //Serial.println(motor_speed);

    if (!strcmp(motor_direction, "ccw")) {
      Serial.print("Motor turning ccw at ");
      Serial.println(motor_speed);
      digitalWrite(motor_pin1, LOW);
      digitalWrite(motor_pin2, HIGH);
      analogWrite(motor_en, motor_speed);
    } 
    else if (!strcmp(motor_direction, "cw")) {
      Serial.print("Motor turning cw at ");
      Serial.println(motor_speed);
      digitalWrite(motor_pin1, HIGH);
      digitalWrite(motor_pin2, LOW);
      analogWrite(motor_en, motor_speed);
    } 
    else if (!strcmp(motor_direction, "off")) {
      Serial.println("Motor shutting down...");
      digitalWrite(motor_pin1, LOW);
      digitalWrite(motor_pin2, LOW);
    }
    else {
      Serial.println("Invalid direction");
    }
    /*Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);
    */
    Serial.println("Type motor direction (a/b/n) and speed (0 to 255):");
  }
   
   //manual_delay(5000, currentDir, counter);
}

/*int manual_delay( int input_wait, String currentDir, int counter) {
  long cur_time = millis();
  long start_time = cur_time;
  
  if (cur_time >= start_time + 5000) {
    start_time = millis();
    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);
  }
}
*/
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
    i2c_rcv = Write.read();
  }
}

void requestEvent() {
    Wire.write("hello "); // respond with message of 6 bytes
}
