#define BUTTON_PIN 2 
#define PRESSED LOW

int counter = 0;
volatile long int press_time = 0;
volatile bool check_button = false;


void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  attachInterrupt(0, process_button_click, FALLING);
  
  cli(); // stop interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B = TCCR1B | (1 << CS10); //bitSet(TCCR1B, CS10);  
  // enable timer overflow interrupt
  TIMSK1 = TOIE1 | (1 << TOIE1); 
  pinMode(LED_BUILTIN, OUTPUT);
  sei(); // allow interrupts
  
}

ISR(TIMER1_OVF_vect) {
  if(check_button == true and
     digitalRead(BUTTON_PIN) == PRESSED){
    counter++;
    check_button = false;
  }
}

void loop() {
  delay(2000);
  if(counter > 0) {
    Serial.print("Pressed ");
    Serial.print(counter);
    Serial.println(" times");
    counter = 0; 
  }
}

void process_button_click() {
  if (check_button == false) {
    press_time = millis();
    check_button = true;
  }
}
