#define LED1 11   // LED1 pin
#define LED2 12   // LED2 pin
#define LED3 13   // LED3 pin
#define LED4 7    // LED4 pin
#define BUTTON1 21 // Tipka 1 (vanjski prekid INT0)
#define BUTTON2 19 // Tipka 2 (vanjski prekid INT1)
#define BUTTON3 20 // Tipka 3 (vanjski prekid INT2)

volatile bool continueISR2 = true;

void setup() {
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BUTTON1), invertLED1StateISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON2), blockingISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON3), breakISR2, FALLING);
  
}

void loop() {
  // Pokazatelj da se loop petlja nesmetano vrti
  digitalWrite(LED3, HIGH);  
  delay(500);               
  digitalWrite(LED3, LOW);   
  delay(500);
}

void invertLED1StateISR(){
  static bool led1State = false;
  led1State = !led1State;
  digitalWrite(LED1, led1State);
}

void blockingISR(){
  sei();
  continueISR2 = true;
  digitalWrite(LED2, HIGH);
  while(continueISR2){
    //ne radi ništa
  }
  digitalWrite(LED2, LOW);
  continueISR2 = false;
}

void breakISR2(){
  continueISR2 = false;
}
