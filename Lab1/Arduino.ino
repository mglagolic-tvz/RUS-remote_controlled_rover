#include <TimerOne.h>

#define LED1 11   // LED1 pin
#define LED2 12   // LED2 pin
#define LED3 13   // LED3 pin
#define LED4 8   // LED4 pin
#define BUTTON1 21 // Tipka 1 (vanjski prekid INT0)
#define BUTTON2 20 // Tipka 2 (vanjski prekid INT1)
#define BUTTON3 19 // Tipka 3 (vanjski prekid INT2)
#define PIR 2  
#define SPEAKER 22

volatile unsigned long startTimeOfBlockingISR = 0;
volatile unsigned long stopTimeOfBlockingISR = 0;
volatile bool blockingISREnded = false;

volatile bool button3Pressed = false;

volatile bool movementDetected = false;

const int alarmTimout = 2000;

void setup() {
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(PIR, INPUT_PULLUP);
  pinMode(SPEAKER, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON1), invertLED1StateISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON2), blockingISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON3), button3ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIR), pirISR, RISING);

  Timer1.initialize(30000); // 30 ms
  Timer1.attachInterrupt(dimLED4);
  
  Serial.begin(115200);
}

void loop() {
  if(button3Pressed){
    const int maxBrojPonavljanja = 10;
    const int pauza = 500;
    static int brojac = 0;
    static unsigned long lastMillis = 0;
    static bool led1State = false;
    unsigned long newMillis = millis();


    if((brojac < maxBrojPonavljanja)
        && (newMillis - lastMillis > pauza)){

      
      led1State = !led1State;
      digitalWrite(LED3, led1State);
      brojac++;
      lastMillis = newMillis;
      Serial.println("Poruka");
    }
    if(brojac >= maxBrojPonavljanja ){
      brojac = 0;      
      button3Pressed = false;
    }
    
  }

  if(blockingISREnded){
    blockingISREnded = false;
    Serial.println("\n   Blocking ISR happened!");
    Serial.println("   Start time: " + String(startTimeOfBlockingISR)
                      + "  End time: " + String(stopTimeOfBlockingISR) + "\n");
  }

  if(movementDetected ){
    tone(SPEAKER, 1000, alarmTimout);
    movementDetected = false;
  }

}

void invertLED1StateISR(){
  const unsigned int debounceDelay = 50;
  static unsigned long lastInterruptTime = 0;
  unsigned long currentMillis = millis();  // Dohvati trenutno vrijeme u milisekundama

  static bool led1State = false;

  // Provjeri da li je prošlo dovoljno vremena od posljednjeg prepoznatog prekida (debouncing)
  if (currentMillis - lastInterruptTime > debounceDelay) {
    // Ako je prošlo više od debounceDelay, to znači da je tipka stabilna
    led1State = !led1State;
    digitalWrite(LED1, led1State);
    lastInterruptTime = currentMillis;  // Ažuriraj vrijeme kada je zadnji prekid bio obrađen
  }
}

void blockingISR(){
  // zabilježi millis po ulasku u ISR
  startTimeOfBlockingISR = millis();
  
  static bool ledState = false;
  for(int i = 0; i < 50; i++){
    ledState = !ledState;
    digitalWrite(LED2, ledState);
    // ova petlja se koristi umjesto delay jer delay ne radi unutar ISR
    for(unsigned long b = 0; b < 100000; b++){
      asm("NOP");
    }
  }

  stopTimeOfBlockingISR = millis();
  blockingISREnded = true;
}

void button3ISR(){
  const unsigned int debounceDelay = 50;
  static unsigned long lastInterruptTime = 0;
  unsigned long currentMillis = millis();  // Dohvati trenutno vrijeme u milisekundama

  // Provjeri da li je prošlo dovoljno vremena od posljednjeg prepoznatog prekida (debouncing)
  if (currentMillis - lastInterruptTime > debounceDelay) {
    // Ako je prošlo više od debounceDelay, to znači da je tipka stabilna
    button3Pressed = true;
    lastInterruptTime = currentMillis;  // Ažuriraj vrijeme kada je zadnji prekid bio obrađen
  }

  
  
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

void pirISR(){
  movementDetected = true;
}
