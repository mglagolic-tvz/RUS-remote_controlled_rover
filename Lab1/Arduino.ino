#define LED1 11   // LED1 pin
#define LED2 12   // LED2 pin
#define LED3 13   // LED3 pin
#define LED4 7    // LED4 pin
#define BUTTON1 21 // Tipka 1 (vanjski prekid INT0)
#define BUTTON2 20 // Tipka 2 (vanjski prekid INT1)
#define BUTTON3 19 // Tipka 3 (vanjski prekid INT2)

void setup() {
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BUTTON1), invertLED1StateISR, FALLING);

  
}

void loop() {
  // Pokazatelj da se loop petlja nesmetano vrti
  digitalWrite(LED3, HIGH);  
  delay(1000);               
  digitalWrite(LED3, LOW);   
  delay(1000);
  
}

void invertLED1StateISR(){
  static bool led1State = false;
  led1State = !led1State;
  digitalWrite(LED1, led1State);
}

