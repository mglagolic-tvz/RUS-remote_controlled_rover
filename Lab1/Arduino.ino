/**
 * @file    Arduino.ino
 * @brief   Edukacijski projekt za rad s prekidima u ugrađenim platformama.
 * 
 * Ovaj projekt demonstrira korištenje vanjskih prekida, timer prekida i osnovne obrade ulaznih signala (tipke i senzor pokreta).
 * Koristi se za praktično upoznavanje s ISR-ovima (Interrupt Service Routine) i njihovim utjecajem na tijek programa.
 * 
 * Komponente:
 * - 3 tipke koje aktiviraju razne ISR-ove
 * - PIR senzor pokreta
 * - 4 LED-ice
 * - zvučnik
 * 
 * Funkcionalnosti:
 * - Reakcija LED-ica i zvučnika na prekide
 * - Debounce logika za tipke
 * - Detekcija pokreta uz aktivaciju alarma
 * - Primjer blokirajućeg ISR-a
 * 
 * Napomena: Projekt je dio vježbe s ciljem uvježbavanja i razumijevanja rada prekida (interrupts) u realnom vremenu.
 * 
 * 
 * @author  Matija Glagolić
 * @date    4.4.2025
 * @version 1.0
 */

#include <TimerOne.h>

// === Definicije pinova ===
#define LED1 11     ///< LED1 - uključuje/isključuje se na pritisak tipke 1
#define LED2 12     ///< LED2 - trepće unutar ISR-a (tipka 2) kao primjer blokirajućeg prekida
#define LED3 13     ///< LED3 - trepne 5 puta nakon pritiska tipke 3
#define LED4 8      ///< LED4 - stvara efekt "disanja" putem timer prekida
#define BUTTON1 21  ///< Tipka 1 - aktivira prekid INT0
#define BUTTON2 20  ///< Tipka 2 - aktivira prekid INT1 (blokirajući ISR)
#define BUTTON3 19  ///< Tipka 3 - aktivira prekid INT2 (pokreće treptanje LED3)
#define PIR 2       ///< PIR senzor pokreta - aktivira alarm
#define SPEAKER 22  ///< Zvučnik - aktivira se na detekciju pokreta

// === Globalne varijable ===
volatile unsigned long startTimeOfBlockingISR = 0; ///< Početak trajanja ISR-a za tipku 2 (za mjerenje trajanja blokade)
volatile unsigned long stopTimeOfBlockingISR = 0;  ///< Kraj trajanja ISR-a
volatile bool blockingISREnded = false;            ///< Oznaka da je ISR završio (za ispis u loop-u)

volatile bool button3Pressed = false;              ///< Zastavica za signal da je tipka 3 pritisnuta
volatile bool movementDetected = false;            ///< Zastavica da je detektiran pokret PIR senzorom

const int alarmTimout = 2000;                      ///< Trajanje alarma u milisekundama

/**
 * @brief Inicijalizacija svih pinova, serijskog porta, timer-a i prekida.
 */
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

  // Postavljanje vanjskih prekida
  attachInterrupt(digitalPinToInterrupt(BUTTON1), invertLED1StateISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON2), blockingISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON3), button3ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIR), pirISR, RISING);

  // Timer prekid za LED4 (efekt disanja)
  Timer1.initialize(30000); // 30 ms interval
  Timer1.attachInterrupt(dimLED4);

  Serial.begin(115200);
}

/**
 * @brief Glavna petlja programa - provjerava zastavice koje su postavljene u prekidima.
 */
void loop() {
  // Reakcija na pritisak tipke 3 - treptanje LED3
  if(button3Pressed){
    const int maxBrojPonavljanja = 10;
    const int pauza = 500;
    static int brojac = 0;
    static unsigned long lastMillis = 0;
    static bool led1State = false;
    unsigned long newMillis = millis();

    if((brojac < maxBrojPonavljanja) && (newMillis - lastMillis > pauza)){
      led1State = !led1State;
      digitalWrite(LED3, led1State);
      brojac++;
      lastMillis = newMillis;
    }

    if(brojac >= maxBrojPonavljanja){
      brojac = 0;
      button3Pressed = false;
    }
  }

  // Ako je ISR za tipku 2 završio, ispiši trajanje
  if(blockingISREnded){
    blockingISREnded = false;
    Serial.println("\n   Blocking ISR se dogodio!");
    Serial.println("   Početak: " + String(startTimeOfBlockingISR) +
                   "  Kraj: " + String(stopTimeOfBlockingISR) + "\n");
  }

  // Aktivacija alarma nakon detekcije pokreta
  if(movementDetected){
    tone(SPEAKER, 1000, alarmTimout);
    movementDetected = false;
  }
}

/**
 * @brief ISR za tipku 1 - invertira stanje LED1.
 * 
 * Koristi se za demonstraciju reakcije na pritisak tipke bez blokiranja glavne petlje.
 * Uključuje debounce logiku kako bi se izbjegla višestruka aktivacija.
 */
void invertLED1StateISR(){
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

/**
 * @brief Blokirajući ISR za tipku 2 - treptanje LED2.
 * 
 * Namjerno blokira procesor kako bi se demonstrirala opasnost dugih ISR funkcija.
 * Mjerenje trajanja pokazuje koliko to može utjecati na rad ostatka sustava.
 */
void blockingISR(){
  startTimeOfBlockingISR = millis();

  static bool ledState = false;
  for(int i = 0; i < 50; i++){
    ledState = !ledState;
    digitalWrite(LED2, ledState);
    for(unsigned long b = 0; b < 100000; b++){
      asm("NOP");
    }
  }

  stopTimeOfBlockingISR = millis();
  blockingISREnded = true;
}

/**
 * @brief ISR za tipku 3 - postavlja zastavicu za LED3.
 * 
 * U glavnoj petlji se koristi zastavica za treptanje LED3, jer ISR ne smije dugo trajati.
 * Uključena je debounce zaštita.
 */
void button3ISR(){
  const unsigned int debounceDelay = 50;
  static unsigned long lastInterruptTime = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - lastInterruptTime > debounceDelay) {
    button3Pressed = true;
    lastInterruptTime = currentMillis;
  }
}

/**
 * @brief Timer prekid za LED4 - stvara "disanje" (dimanje).
 * 
 * Intenzitet LED4 se mijenja svakih 30ms pomoću PWM-a.
 */
void dimLED4() {
  static int intensity = 100;
  static bool rising = true;

  if(rising){    
    if(intensity >= 255){
      rising = false;
    } else {
      intensity += 5;
    }
  } else {
    if(intensity <= 10){
      rising = true;
    } else {
      intensity -= 5;
    }
  }

  analogWrite(LED4, intensity);
}

/**
 * @brief ISR za PIR senzor pokreta.
 * 
 * Postavlja zastavicu za aktivaciju alarma u glavnoj petlji.
 */
void pirISR(){
  movementDetected = true;
}
