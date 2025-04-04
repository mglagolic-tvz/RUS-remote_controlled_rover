#include <TimerOne.h>

// Definicije pinova
#define LED1 11     // LED1 - upravljanje putem tipke 1
#define LED2 12     // LED2 - trepće unutar ISR (tipka 2)
#define LED3 13     // LED3 - trepće 10 puta nakon pritiska tipke 3
#define LED4 8      // LED4 - dimmer efekt preko Timer interrupta
#define BUTTON1 21  // Tipka 1 - vanjski prekid INT0
#define BUTTON2 20  // Tipka 2 - vanjski prekid INT1
#define BUTTON3 19  // Tipka 3 - vanjski prekid INT2
#define PIR 2       // PIR senzor pokreta - vanjski prekid
#define SPEAKER 22  // Zvučnik

// Varijable za praćenje trajanja blokirajućeg ISR-a
volatile unsigned long startTimeOfBlockingISR = 0;
volatile unsigned long stopTimeOfBlockingISR = 0;
volatile bool blockingISREnded = false;

// Zastavica za tipku 3
volatile bool button3Pressed = false;

// Zastavica za detekciju pokreta
volatile bool movementDetected = false;

// Vrijeme trajanja alarma u milisekundama
const int alarmTimout = 2000;

void setup() {
  // Inicijalizacija pinova
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(PIR, INPUT_PULLUP);
  pinMode(SPEAKER, OUTPUT);

  // Postavljanje vanjskih prekida
  attachInterrupt(digitalPinToInterrupt(BUTTON1), invertLED1StateISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON2), blockingISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON3), button3ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIR), pirISR, RISING);

  // Postavljanje Timer1 prekida za dimmer efekt (LED4)
  Timer1.initialize(30000); // 30 ms
  Timer1.attachInterrupt(dimLED4);

  Serial.begin(115200);
}

void loop() {
  // Reakcija na pritisak tipke 3 (treptanje LED3 deset puta)
  if (button3Pressed) {
    const int maxBrojPonavljanja = 10;
    const int pauza = 500;
    static int brojac = 0;
    static unsigned long lastMillis = 0;
    static bool led1State = false;
    unsigned long newMillis = millis();

    if ((brojac < maxBrojPonavljanja) && (newMillis - lastMillis > pauza)) {
      led1State = !led1State;
      digitalWrite(LED3, led1State);
      brojac++;
      lastMillis = newMillis;
      Serial.println("Poruka");
    }

    if (brojac >= maxBrojPonavljanja) {
      brojac = 0;
      button3Pressed = false;
    }
  }

  // Ispis vremena trajanja blokirajućeg ISR-a
  if (blockingISREnded) {
    blockingISREnded = false;
    Serial.println("\n   Blocking ISR se dogodio!");
    Serial.println("   Početak: " + String(startTimeOfBlockingISR) +
                   "  Kraj: " + String(stopTimeOfBlockingISR) + "\n");
  }

  // Reakcija na detekciju pokreta
  if (movementDetected) {
    tone(SPEAKER, 1000, alarmTimout);
    movementDetected = false;
  }
}

// ISR za tipku 1 - invertira stanje LED1 uz debouncing
void invertLED1StateISR() {
  const unsigned int debounceDelay = 50;
  static unsigned long lastInterruptTime = 0;
  unsigned long currentMillis = millis();

  static bool led1State = false;

  if (currentMillis - lastInterruptTime > debounceDelay) {
    led1State = !led1State;
    digitalWrite(LED1, led1State);
    lastInterruptTime = currentMillis;
  }
}

// ISR za tipku 2 - blokirajući ISR koji trepće LED2
void blockingISR() {
  startTimeOfBlockingISR = millis();

  static bool ledState = false;
  for (int i = 0; i < 50; i++) {
    ledState = !ledState;
    digitalWrite(LED2, ledState);
    
    // Ručno kašnjenje (zamjena za delay u ISR-u)
    for (unsigned long b = 0; b < 100000; b++) {
      asm("NOP");
    }
  }

  stopTimeOfBlockingISR = millis();
  blockingISREnded = true;
}

// ISR za tipku 3 - postavlja zastavicu za treptanje LED3
void button3ISR() {
  const unsigned int debounceDelay = 50;
  static unsigned long lastInterruptTime = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - lastInterruptTime > debounceDelay) {
    button3Pressed = true;
    lastInterruptTime = currentMillis;
  }
}

// Timer ISR za stvaranje dimmer efekta na LED4
void dimLED4() {
  static int intensity = 100;
  static bool rising = true;

  if (rising) {
    if (intensity >= 255) {
      rising = false;
    } else {
      intensity += 5;
    }
  } else {
    if (intensity <= 10) {
      rising = true;
    } else {
      intensity -= 5;
    }
  }

  analogWrite(LED4, intensity);
}

// ISR za PIR senzor - detekcija pokreta
void pirISR() {
  movementDetected = true;
}
