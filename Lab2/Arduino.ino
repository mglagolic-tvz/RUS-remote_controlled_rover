#include "LowPower.h"  ///< Uključivanje biblioteke za štednju energije na mikrokontroleru

#define wakeUpPin 2  ///< Pin na kojem se detektira prekid za buđenje (npr. vanjski signal)
#define led 13       ///< Pin na kojem je LED dioda (pin 13 je obično povezan s ugrađenom LED diodom na mnogim Arduinima)

bool ledState = false;  ///< Varijabla koja čuva stanje LED diode (uključeno/isključeno)

/**
 * @brief Prekidna rutina prekida za buđenje
 * 
 * Funkcija koja se poziva kada dođe do prekida na pinu `wakeUpPin`.
 * Ova funkcija je trenutno prazna jer netreba ništa izvršavati (služi samo da se može postaviti prekid).
 */
void wakeUp() {}

/**
 * @brief Inicijalizacija sustava
 * 
 * Ova funkcija se poziva jednom pri pokretanju mikrokontrolera. Postavlja pinove, 
 * inicijalizira serijsku komunikaciju i postavlja mikrokontroler u početno stanje.
 */
void setup() {
  pinMode(wakeUpPin, INPUT);  ///< Postavljanje pina za buđenje kao ulaz
  pinMode(led, OUTPUT);       ///< Postavljanje pina za LED kao izlaz
  Serial.begin(9600);         ///< Pokretanje serijske komunikacije na brzini od 9600 bps

  attachInterrupt(digitalPinToInterrupt(wakeUpPin), wakeUp, RISING);  ///< Postavljanje prekida za pin 2 (interni broj prekida 4). Kada se dogodi rastući rub signala (RISING), poziva se funkcija wakeUp()
}

/**
 * @brief Glavna petlja programa
 * 
 * Ova funkcija stalno pokreće uređaj. Postavlja prekid na pin `wakeUpPin` za detekciju 
 * vanjskog signala, stavlja uređaj u stanje spavanja i nakon buđenja pokreće odgovarajuće radnje.
 */
void loop() {
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);  ///< Aktivacija načina štednje energije (mikrokontroler će spavati)

  Serial.println("Registriran pokret!");  ///< Ispisivanje poruke na serijski monitor nakon buđenja
  Serial.flush();  ///< Čisti serijski bafer (osigurava da svi podaci budu poslani)

  invertLed13();  ///< Pozivanje funkcije koja mijenja stanje LED diode (uključuje/isključuje)

  delay(100);  ///< Kratko kašnjenje od 100 milisekundi prije ponovnog pokretanja petlje
}

/**
 * @brief Funkcija koja mijenja stanje LED diode (uključuje/isključuje)
 * 
 * Ova funkcija mijenja stanje LED diode na pinu 13 (ako je bila uključena, sada je isključuje, i obratno).
 */
void invertLed13() {
  ledState = !ledState;  ///< Promjena stanja LED diode (ako je bila uključena, sada je isključujemo, i obrnuto)
  digitalWrite(led, ledState);  ///< Postavljanje novog stanja na LED pin (13)
}
