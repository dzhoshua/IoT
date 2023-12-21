#include <Wire.h>
#define sensor_pin A0
#define STEP 10

int masterVals[STEP]; 
int slaveVals[STEP]; 

int val = 0;
int master_val = 0;
volatile bool newData = false;
 
int collected = 0;
int prev = 0;

float conversionFactor = 1.0;

bool calibrationStart = false;
bool inProgress = false;
bool isComplete = false;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin(8); 
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == 'c') {
      calibrationStart = true;
    }
  }

  if (calibrationStart && !isComplete) {
    calibrate();
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);                      
    digitalWrite(LED_BUILTIN, LOW);   
    delay(100);
  }

  if (isComplete) {
    digitalWrite(LED_BUILTIN, HIGH);
    int sensorData = analogRead(sensor_pin);
    int calibratedData = int(sensorData * conversionFactor);

    Serial.print("Slave: ");
    Serial.println(sensorData);
    Serial.print("Slave calibrated: ");
    Serial.println(calibratedData);
  } 
  else {
    val = analogRead(sensor_pin);
    Serial.print("Slave:");
    Serial.println(val);
    if (newData) {
      Serial.print("Master:");
      Serial.println(master_val);
      newData = false;
    }
  }
  delay(1000);
}

void receiveEvent(int byteCount) {
  if (byteCount >= sizeof(master_val)) { 
    Wire.readBytes((uint8_t*)&master_val, sizeof(master_val));
    newData = true; 
  }
}

void requestEvent() {
  if (isComplete) {
    Wire.write("t");
  } else {
    Wire.write("f");
  }
}

void calibrate() {
  if (!inProgress) {
    Serial.println("Calibration started...");
    inProgress = true;

    collected = 0;
    masterVals[collected] = master_val;
    slaveVals[collected] = analogRead(sensor_pin);
    collected++;
  }

  int start_time = millis();
  if (collected >= STEP) {
    inProgress = false;
    isComplete = true;
    Serial.println("---Calibration complete!---");
    digitalWrite(LED_BUILTIN, HIGH);
    float masterAvg = average(masterVals, STEP);
    float slaveAvg = average(slaveVals, STEP);
    
    if (masterAvg != 0 && slaveAvg != 0) {
      conversionFactor = masterAvg / slaveAvg;
    }
  }
  else if (start_time - prev > 1000 && collected < STEP) {
  //else if (collected < STEP) {
    int masterData = master_val;
    int slaveData = analogRead(A0);


    if (!containsValue(masterVals, sizeof(masterVals) / sizeof(masterVals[0]), masterData) &&
     !containsValue(slaveVals, sizeof(slaveVals) / sizeof(slaveVals[0]), slaveData)) 
     {
      masterVals[collected] = master_val;
      slaveVals[collected] = analogRead(sensor_pin);
      Serial.println("Adding data...");
      collected++;
      prev = millis();
    }
  }
}

bool containsValue(int arr[], int length, int targetValue) {
  for (int i = 0; i < length; i++) {
    if (arr[i] == targetValue) {
      return true;
    }
  }
  return false;
}

float average(int arr[], int length) {
  float total = 0.0;
  for (int i = 0; i < length; i++) {
    total += arr[i];
  }
  return total / length;
}
