#include <PS3BT.h>
#include <usbhub.h>

#define ENABLE_UHS_DEBUGGING 1

// Zadovoljiti IDE koji mora vidjeti include izjavu u ino datoteci
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

// Definicije
#define START_BYTE 0xAA
#define STOP_BYTE 0x55
#define PACKET_MAX_SIZE 32  ///< Maksimalna veličina paketa

// Definicija pinova
#define LED_EMPTY_BATTERY 13  ///< Pin za LED koji pokazuje praznu bateriju

USB Usb; ///< USB objekt za inicijalizaciju
BTD Btd(&Usb); ///< Bluetooth Dongle objekt
PS3BT PS3(&Btd); ///< PS3 Bluetooth objekt

// Paket koji ćemo slati
uint8_t packet[PACKET_MAX_SIZE];  ///< Paket podataka koji će biti poslan
uint8_t packetSize = 0;  ///< Trenutna veličina paketa
unsigned long lastSendTime = 0;  ///< Posljednje vrijeme slanja paketa
const unsigned long sendInterval = 100;  ///< Interval slanja paketa u milisekundama

// Globalne varijable za kontrole koje se šalju
const uint8_t servoHDefaultPosition = 127;  ///< Početna pozicija horizontalnog servoa
const uint8_t servoVDefaultPosition = 250;  ///< Početna pozicija vertikalnog servoa
uint8_t motor1pwm = 0;  ///< PWM signal za prvi motor
uint8_t motor2pwm = 0;  ///< PWM signal za drugi motor
uint8_t motor1dir = 1;  ///< Smjer za prvi motor (1 - naprijed, 0 - nazad)
uint8_t motor2dir = 1;  ///< Smjer za drugi motor (1 - naprijed, 0 - nazad)
uint8_t servoH = servoHDefaultPosition;  ///< Trenutna pozicija horizontalnog servoa
uint8_t servoV = servoVDefaultPosition;  ///< Trenutna pozicija vertikalnog servoa
uint8_t ledIntensity = 0;  ///< Intenzitet LED-a
int led_intensity_counter;  ///< Brojač za promjenu intenziteta LED-a

void setup() {
  pinMode(LED_EMPTY_BATTERY, OUTPUT);  ///< Postavljanje pina za LED
  Serial.begin(9600);  ///< Početak serijske komunikacije na 9600 bps

  // Provjera inicijalizacije USB veze
  if (Usb.Init() == -1) {
    // Ako USB nije inicijaliziran, treperi LED kako bi signalizirao grešku
    while (1) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
    }
  }
  // LED signalizira da je baterija prisutna
  digitalWrite(LED_EMPTY_BATTERY, LOW);
}

void loop() {

  Usb.Task();  ///< Provjera USB statusa i upravljanje vezama

  // Provjera je li primljen signal za stanje baterije
  if(Serial.available()){
    uint8_t battery_state = Serial.read();
    if(battery_state == 0x01){
      digitalWrite(LED_EMPTY_BATTERY, HIGH);  ///< LED svijetli ako je baterija prazna
    }
    if(battery_state == 0x00){
      digitalWrite(LED_EMPTY_BATTERY, LOW);  ///< LED svijetli ako je baterija puna
    }
  }

  unsigned long currentMillis = millis();  ///< Trenutno vrijeme u milisekundama

  // Ako je PS3 kontroler povezan i prošlo je dovoljno vremena za slanje paketa
  if (PS3.PS3Connected && currentMillis - lastSendTime >= sendInterval) {
    lastSendTime = currentMillis;  ///< Ažuriraj vrijeme posljednjeg slanja

    // Ako nije pritisnut gumb R1, upravljanje motorima pomoću analognih palica
    if (!PS3.getButtonPress(R1)) {
      if (PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117) {
        // Mapiranje analogne vrijednosti na PWM signal za prvi motor
        int pwm = map(PS3.getAnalogHat(LeftHatY), 0, 255, 255, -255);

        // Postavljanje smjera i vrijednosti PWM za prvi motor
        if (pwm >= 0) {
          motor1dir = 1;  ///< Smjer naprijed
          motor1pwm = pwm;
        }
        if (pwm < 0) {
          motor1dir = 0;  ///< Smjer unazad
          motor1pwm = -pwm;
        }

      } else {
        motor1pwm = 0;  ///< Ako nije pomaknuta palica, motor je isključen
      }

      if (PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117) {
        // Mapiranje analogne vrijednosti na PWM signal za drugi motor
        int pwm = map(PS3.getAnalogHat(RightHatY), 0, 255, 255, -255);

        // Postavljanje smjera i vrijednosti PWM za drugi motor
        if (pwm >= 0) {
          motor2dir = 1;  ///< Smjer naprijed
          motor2pwm = pwm;
        }
        if (pwm < 0) {
          motor2dir = 0;  ///< Smjer unazad
          motor2pwm = -pwm;
        }

      } else {
        motor2pwm = 0;  ///< Ako nije pomaknuta palica, motor je isključen
      }

      // Postavljanje početnih pozicija servoa
      servoH = servoHDefaultPosition;
      servoV = servoVDefaultPosition;
    }
    // Ako je pritisnut gumb R1, upravljanje servoima
    if (PS3.getButtonPress(R1)) {
      servoV = PS3.getAnalogHat(RightHatY);  ///< Podešavanje vertikalnog servoa
      servoH = PS3.getAnalogHat(RightHatX);  ///< Podešavanje horizontalnog servoa
      motor1pwm = 0;  ///< Isključivanje prvog motora
      motor2pwm = 0;  ///< Isključivanje drugog motora
    }

    // Povećanje intenziteta LED-a ako je pritisnut gumb za strelicu gore
    if (PS3.getButtonClick(UP)) {
      if (led_intensity_counter < 5){
        led_intensity_counter++;
        ledIntensity = led_intensity_counter * 40;  ///< Povećaj intenzitet LED-a
      }
    }
    // Smanjenje intenziteta LED-a ako je pritisnut gumb za strelicu dolje
    if (PS3.getButtonClick(DOWN)) {
      if (led_intensity_counter > 0){
        led_intensity_counter--;
        ledIntensity = led_intensity_counter * 40;  ///< Smanji intenzitet LED-a
      }
    }

    // Ako je pritisnut gumb PS, isključi kontroler
    if (PS3.getButtonClick(PS)) {
      PS3.disconnect();  ///< Isključi PS3 kontroler
    }

    // Pripremi novi paket podataka
    createPacket();

    // Pošalji paket podataka putem serijske komunikacije
    Serial.write(packet, packetSize);
  }
}

/**
 * @brief Priprema paket podataka za slanje.
 *
 * Funkcija dodaje različite informacije u paket koji će biti poslan putem serijske komunikacije.
 */
void createPacket() {
  packetSize = 0;  ///< Resetiranje veličine paketa

  // Početni bajt paketa
  packet[packetSize++] = START_BYTE;

  // Dodavanje podataka u paket (vrijednosti za motore, servoe, LED)
  addToPacket(0x01, motor1pwm);
  addToPacket(0x02, motor2pwm);
  addToPacket(0x03, motor1dir);
  addToPacket(0x04, motor2dir);
  addToPacket(0x05, servoH);
  addToPacket(0x06, servoV);
  addToPacket(0x07, ledIntensity);

  // Kraj paketa
  packet[packetSize++] = STOP_BYTE;
}

/**
 * @brief Dodaje podatak u paket.
 *
 * Funkcija dodaje ID i vrijednost u paket podataka.
 *
 * @param id ID podatka koji se dodaje u paket.
 * @param value Vrijednost podatka koji se dodaje u paket.
 */
void addToPacket(uint8_t id, uint8_t value) {
  if (packetSize + 2 < PACKET_MAX_SIZE) {
    packet[packetSize++] = id;     ///< Dodaj ID podatka
    packet[packetSize++] = value;  ///< Dodaj vrijednost podatka
  }
}
