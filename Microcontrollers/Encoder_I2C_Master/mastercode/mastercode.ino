
int motor_speed;
char motor_direction;
long start_time;
char c;
long counter = 0;
byte storage[4];

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output

  start_time = millis();

  motor_speed = 0;
  // Powering motor
  pinMode(motor_en, OUTPUT);
  pinMode(motor_pin1, OUTPUT);
  pinMode(motor_pin2, OUTPUT);
}

void loop() {

  long cur_time = millis();
  if (cur_time >= start_time + 1000) {
    start_time = cur_time;

    // asks user for the motor direction via serial
    Serial.println("Type speed (0 to 255):");
  
    
    if (Wire.available()) {
     for (int i = 0; i < 4; i++) {
      storage[i] = Wire.read();
     }
     counter = (long)(storage[3] << 24) | (storage[2] << 16) | (storage[1] << 8) | storage[0];
    }
    else {
      Wire.requestFrom(8, 4);    // request 4 byte from slave device #8
    }

   Serial.println(counter);

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
  

  
  


  
}
