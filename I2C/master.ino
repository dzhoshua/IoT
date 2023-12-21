#include <Wire.h>
#define sensor_pin A0

bool flag = false;

void setup() {
  Serial.begin(9600);
  Wire.begin(); // join I2C bus as master no address needed
}

void loop() {
  int val = analogRead(sensor_pin);
  Serial.print("Master sensor value: ");
  Serial.println(val);

  Wire.requestFrom(8, 1);
  if (Wire.available()){
    char c = Wire.read();
    if (c == 'f')
    {
      Wire.beginTransmission(8);
      Wire.write((uint8_t*)&val, sizeof(val));
      Wire.endTransmission();
      flag=true;
    }
    if (c == 't' && flag){
      Serial.println("Calibration complete.");
      flag=false;
    }
  }
  delay(1000);
}
