const int sensor_pin = A0;
const int trig_pin = 9; // transmitter
const int echo_pin = 10; // receiver

int lower = 0;
int upper = 0;

float offset = 0;
float intercept = 0;
bool flag = false;


void setup() {
  Serial.begin(9600);

  Serial.println("Enter the calibration range (lower and upper bounds).");

  while (!Serial.available());
  String input = Serial.readStringUntil('\n');
  sscanf(input.c_str(), "%d %d", &lower, &upper);

  Serial.print("lower = ");
  Serial.println(lower);
  Serial.print("upper = ");
  Serial.println(upper);
}
void loop() {
  if (flag == false) {
    calibrate(lower, upper);
  } else {
    Serial.print("Distance is: ");
    distance();
    delay(1000);
  }
}

void distance() {
  int sensor_val = analogRead(sensor_pin);
  // Convert the analog value to distance DEPENDS ON SENSOR
  float distance = analogRead(sensor_pin) * 5 / 1024;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(100);
}

void calibrate(int lower, int upper) {
  int percentage = 90;
  int maxGap = 2;

  bool isEnoughData = false;
  int x[100];
  float y[100];
  int index = 0;
  int count = 0;

  digitalWrite(LED_BUILTIN, HIGH);

  while (!isEnoughData) {
    int sensorVal = analogRead(sensor_pin) * 5 / 1024;
    float distance = get_distance();
    if (distance >= lower && distance <= upper) {
      if (index == 0 || (distance - y[index - 1] <= maxGap)) {
        y[index] = distance;
        x[index] = sensorVal;
        index++;
        count++;
        Serial.println("Data collection in process...");
      }
    }

    if (count > 0 && (count * 100) / (upper - lower + 1) >= percentage) {
      isEnoughData = true;
    }
    delay(500);
  }
  digitalWrite(LED_BUILTIN, LOW);

  buildModel(x, y, count);
}

float get_distance() {
  // Trigger the ultrasonic: set 10us of HIGH on trig_pin
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  // Wait ultrasonic pulse to return
  unsigned long duration = pulseIn(echo_pin, HIGH);
  // Calculate the distance in centimeters using the speed of sound
  // (343 meters per second or 0.0343 centimeters per microsecond)
  float distance_cm = duration * 0.0343 / 2.0;
  return distance_cm;
}

void buildModel(int signalVals[], float distanceVals[], int dataSize) {
  offset = getOffset(signalVals, distanceVals, dataSize);
  intercept = getIntercept(signalVals, distanceVals, dataSize, offset);
  Serial.print("Model: y = ");
  Serial.print(offset);
  Serial.print(" * x + ");
  Serial.println(intercept);
  flag = true;
}

float getOffset(int x[], float y[], int n) {
  float sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;

  for (int i = 0; i < n; ++i) {
    sumX += x[i];
    sumY += y[i];
    sumXY += x[i] * y[i];
    sumX2 += x[i] * x[i];
  }

  float offset = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
  return offset;
}

float getIntercept(int x[], float y[], int n, float offset) {
  float sumX = 0, sumY = 0;

  for (int i = 0; i < n; ++i) {
    sumX += x[i];
    sumY += y[i];
  }

  float intercept = (sumY - offset * sumX) / n;
  return intercept;
}
