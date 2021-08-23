#include <Wire.h>

int motor_speed;
char *motor_direction;
long start_time;

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output

  start_time = millis();

  motor_speed = 0;
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
      motor_direction = strtok(command.c_str(), " ");
      //Serial.println(motor_direction);
    }
    
    Wire.beginTransmission(8); //starts transmission, uses slave's address
    Wire.write(motor_direction); //asks for data from slave
    Wire.endTransmission(); 
    
    Wire.requestFrom(8, 1);    // request 1 byte from slave device #8

    while (Wire.available()) { // slave may send less than requested
      motor_direction = Wire.read(); // receive a byte as character (a or b)
      Serial.print(motor_direction);         // print the character
    }
  }
  

  
  


  
}
