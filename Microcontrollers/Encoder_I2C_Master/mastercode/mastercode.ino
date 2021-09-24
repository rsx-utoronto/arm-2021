#include <Wire.h>

int motor_position;
char motor_direction;
long start_time;
char c;

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output

  start_time = millis();

  motor_position = 0;
}

void loop() {

  long cur_time = millis();
  if (cur_time >= start_time + 1000) {
    start_time = cur_time;

    // asks user for the motor direction via serial
    Serial.println("Type motor direction (a/b/n) and speed (0 to 255):");
    if(Serial.available()){
      String command = Serial.readString();
      //motor_direction = command.c_str();
      motor_direction = command.c_str()[0];
      Serial.println(motor_direction);
    }
    
    Wire.beginTransmission(8); //starts transmission, uses slave's address
    Wire.write(motor_position); //asks for data from slave
    Wire.endTransmission(); 
    
    Wire.requestFrom(8, 1);    // request 1 byte from slave device #8
    if (Wire.available()) {
      motor_position = Wire.read();
    }

    while (Wire.available()) { // slave may send less than requested
      c = Wire.read(); // receive a byte as character (a or b)
      Serial.println(c);         // print the character
    }
  }
  

  
  


  
}
