#include <TimerOne.h>

#define LED1 11   // LED1 pin
#define LED2 12   // LED2 pin
#define LED3 13   // LED3 pin
#define LED4 10   // LED4 pin
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
  //attachInterrupt(digitalPinToInterrupt(BUTTON2), blockingISR, FALLING);
  //attachInterrupt(digitalPinToInterrupt(BUTTON3), breakISR2, FALLING);

  Timer1.initialize(30000); // 30 ms
  Timer1.attachInterrupt(dimLED4);
  
}

void loop() {
  // Pokazatelj da se loop petlja nesmetano vrti
  digitalWrite(LED3, HIGH);  
  delay(500);               
  digitalWrite(LED3, LOW);   
  delay(500);
}

void invertLED1StateISR(){
  const unsigned int debounceDelay = 50;
  static bool led1State = false;
  static unsigned long lastInterruptTime = 0;
  unsigned long currentMillis = millis();  // Dohvati trenutno vrijeme u milisekundama

  // Provjeri da li je prošlo dovoljno vremena od posljednjeg prepoznatog prekida (debouncing)
  if (currentMillis - lastInterruptTime > debounceDelay) {
    // Ako je prošlo više od debounceDelay, to znači da je tipka stabilna
    led1State = !led1State;
    digitalWrite(LED1, led1State);
    lastInterruptTime = currentMillis;  // Ažuriraj vrijeme kada je zadnji prekid bio obrađen
  }

  
  
}

void blockingISR(){
  continueISR2 = true;
  digitalWrite(LED2, HIGH);

  for(unsigned long b = 0; b < 1000000; b++){
    asm("NOP");
  }
  digitalWrite(LED2, LOW);
  continueISR2 = false;
}

void breakISR2(){
  continueISR2 = false;
}

void dimLED4() {
  static int intensity = 100;
  static int rising = true;
  if(rising){    
    if(intensity == 255){
      rising = false;
    }
    else{
      intensity = intensity + 5;
    }
  }
  else{
    if(intensity == 10){
      rising = true;
    }
    else{
      intensity = intensity - 5;
    }
  }
  
  analogWrite(LED4, intensity);
}
