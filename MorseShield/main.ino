#define SEND_SIGNAL_PIN 9
#define GET_SIGNAL_PIN 2

#define DATA_PIN 7
#define LATCH_PIN 5
#define CLOCK_PIN 3

#define SENSOR_PIN A0

#define SWITCH_PIN 12

//int TU = 100;
int TU = 0; //read A0
int switch_status = 0;
int status_transmitter = 0;
int status_receiver = 1;

bool is_transmitted=false;

String received_message = "";
String result = "";
const char separator = '/';

const char letterArr[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

String morseCode[] = {
  ".-", "-...", "-.-.",
  
  "-..", ".", "..-.",
  
  "--.", "....", "..",
  
  ".---", "-.-", ".-..",
  
  "--", "-.", "---",
  
  ".--.", "--.-", ".-.",
  
  "...", "-", "..-",
  
  "...-", ".--", "-..-",
  
  "-.--", "--.."
};


byte letters[] = {
  0b11101110, //  A
  0b00111110, //  B
  0b10011010, //  C
  0b01111100, //  D
  0b10011110, //  E
  0b10001110, //  F
  0b10111010, //  G
  0b0101110,  //  H
  0b00100000, //  I
  0b01111000, //  J
  0b10101110, //  K
  0b00011010, //  L
  0b10101100, //  M
  0b00101100, //  N
  0b00111100, //  O
  0b11001110, //  P 
  0b11100110, //  Q
  0b00001100, //  R
  0b10110110, //  S
  0b00011110, //  T
  0b01111010, //  U
  0b111000,   //  V
  0b1010110,  //  W
  0b01101110, //  X
  0b01110110, //  Y
  0b11011100, //  Z
  0b00000000  //  empty
};



void setup()
{
  Serial.begin(9600);
  
  pinMode(SEND_SIGNAL_PIN, OUTPUT);
  pinMode(GET_SIGNAL_PIN, INPUT);
  
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  
  pinMode(SENSOR_PIN, INPUT);
  pinMode(SWITCH_PIN, INPUT);
  
  if (digitalRead(SWITCH_PIN) == 0) {
    switch_status = status_transmitter;
    Serial.println("--I AM TRANSMITTER--");
  } else {
    switch_status = status_receiver; 
    Serial.println("--I AM RECEIVER--");
  }
  
}

void loop()
{
  if (digitalRead(SWITCH_PIN) != switch_status) {
    //changing the status using the slide switch
    switch_status = digitalRead(SWITCH_PIN);
  }
  TU  = analogRead(SENSOR_PIN); //curr value = 82
  //Serial.print("----TU: ");
  //Serial.println(TU);
  
  if (switch_status == status_transmitter) { 
    digitalWrite(SEND_SIGNAL_PIN, HIGH);//start transmitting
    //Serial.println("Input message: ");
    if(Serial.available()){//user input
      	String user_input = Serial.readString();
      if (user_input.length() > 0){
        user_input.toUpperCase();
    	send_morse_code(user_input);
      }
    }
  } else {
    receive_morse_code();
  }
}


//////transmitting//////
void send_morse_code(String data){
  Serial.print("Sending message: ");
  Serial.println(data);
  
  for (int i = 0; i < data.length(); i++) {
    char current_letter = data.charAt(i);
    
    if (current_letter != ' ') {
      int index = 0;
      for (int j = 0; j < 26; j++) {
        if (letterArr[j] == current_letter) {
          index = j;
          break;
        }
      }
      String symbol = morseCode[index];
      Serial.print("Send: ");
      Serial.println(symbol);
      
      //encode message
      for (int k = 0; k < symbol.length(); k++) {
        if (symbol[k] == '.') {
          send_signal(1);
        } 
        else if (symbol[k] == '-') {
          send_signal(3);
        } 
      }
       
      delay(3 * TU);//dist between letters
    }else{
      delay(7 * TU);//dist between words
    }
  }
  digitalWrite(SEND_SIGNAL_PIN, HIGH);
}


void send_signal(int duration){
  digitalWrite(SEND_SIGNAL_PIN, LOW);
  delay(duration * TU);
  digitalWrite(SEND_SIGNAL_PIN, HIGH);
  delay(TU);//dist between dot and dash
}
//////end transmitting//////


//////receiving//////
void receive_morse_code(){
  delay(1);
  int idle = 0;
  while (digitalRead(GET_SIGNAL_PIN) == HIGH) {//idle
    delay(TU);
    //Serial.println(TU);
    idle++;
    if (idle > 10) {
      is_transmitted = true;
      break;
    }
  }
  if (is_transmitted == true) {
    show_message(result);
    is_transmitted = false;
    result = "";
  }

  if (idle == 7) {// delay processing
    received_message += separator;
  } 
  else if (idle == 3) {// distance between letters
    received_message += " ";
  } 
  else if (idle > 7) {// end of transmission
    Serial.println(idle);
    result += decode_morse_code(received_message) + " ";
    received_message = "";
  }

  int signal_duration = 0;
  while (digitalRead(GET_SIGNAL_PIN) == LOW) {//message
    delay(TU);
    signal_duration++;
  }

  if (signal_duration == 1) {
    received_message += '.';
  } 
  else if (signal_duration == 3) {
    received_message += '-';
  }

  Serial.println(received_message);
  delay(TU);
}

void show_message(String message) {
  for (int i = 0; i < message.length(); i++) {
    char current_char = message[i];
    
    for (int j = 0; j < sizeof(letterArr); j++) {
      if (letterArr[j] == current_char) {
        set_display(letters[j]);
      }
    }
    delay(200);
    set_display(0b00000000);//clear display
    delay(200);
  }
}

void set_display(byte code) {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, code);
  digitalWrite(LATCH_PIN, HIGH);
}

String decode_morse_code(String code) {
  char current_char = 'a';
  String current_letter = "";
  String decoded_message = "";
  
  for (int i = 0; i <= code.length(); i++) {
    if(i < code.length()){
    	current_char = code.charAt(i);
    }else{
    	current_char = ' ';
    }
    
    if (current_char != ' ' && current_char != separator) {
      current_letter += current_char;
    } else {
      char decoded_letter = 'a';
      for (int j = 0; j < 26; j++) {
        if (current_letter == morseCode[j]) {
          decoded_letter = letterArr[j];
          break;
        }
      }
      if (current_letter.length() != 0) {
        decoded_message += decoded_letter;
        current_letter = "";
      }

      if (current_char == separator) {//division into words
        decoded_message += " ";
        current_letter = "";
      }
    }
  }

  return decoded_message;
}

//////end receiving//////
