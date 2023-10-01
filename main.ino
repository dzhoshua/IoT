#define sensor_pin A0
bool streaming = false;
bool send_one_value = false;
long previous_send_time = 0;
long send_count = 0;
int val = 0;
long current_time=0;

//for led
bool auto_mod=true;
bool siren_mod=false;
bool hand_mod=false;

void setup()
{
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  current_time = millis();
  val = analogRead(sensor_pin);
  data_reading();
  led_mods();
  if (streaming == true and 
      current_time / 100 != send_count) {
    send_photo_data();
    //previous_send_time = current_time;
    previous_send_time = previous_send_time + 100;
    send_count = current_time / 100;
    //Serial.print("Time in millis:");
    //Serial.println(millis());
  }
  if (send_one_value == true) {
    send_photo_data();
    send_one_value = false;
  }
}

void send_photo_data() {
  Serial.print("Sensor value:");
  Serial.println(val);  
}

//LED
void led_mods(){
  if(auto_mod){
    if(val<1){
      digitalWrite(12, HIGH);
    }else{
      digitalWrite(12, LOW);
    }
  }else if(siren_mod){
    if (current_time % 200 <= 100) digitalWrite(12, HIGH);
    else digitalWrite(12, LOW);
  }
  else if(hand_mod){
    digitalWrite(12, HIGH);
  }else if(!hand_mod){
    digitalWrite(12, LOW);
  }
}

void data_reading() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'p') { // p for Photo sensor
      streaming = true;
    } else if (command == 's') { // s for Single value
      streaming = false;
      send_one_value = true;
    }
    else {
      streaming = false;
    }
    
    //commands for led
    if (command == 'a') { //for auto_mod
      auto_mod = true;
      hand_mod=false;
      siren_mod=false;
    }
    else if (command == 'h') { //for hand_mod
      auto_mod = false;
      hand_mod=!hand_mod;
      siren_mod=false;
    }
    else if (command == 'b') { //for siren_mod
      auto_mod = false;
      hand_mod=false;
      siren_mod=true;
    }
  }
}