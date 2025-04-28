#include "pinovi.h"
#include <Servo.h>
#include <Wire.h>

// slave adresa arduina za i2c komunikaciju sa esp32-cam
#define I2C_SLAVE_ADDR 0x08

// postavke za paketnu komunikaciju
#define START_BYTE 0xAA
#define STOP_BYTE  0x55
#define PACKET_MAX_SIZE 32

// napon baterije ispod kojeg se baterija smatra praznom
#define NAPON_BATERIJE_LIMIT 7.4 // Volta


// varijable za paketnu komunikaciju
uint8_t packet[PACKET_MAX_SIZE]; ///< Paket za komunikaciju
uint8_t packetSize = 0; ///< Trenutna veličina paketa
bool receiving = false; ///< Indikator da li se trenutno prima paket
unsigned long lastPacketTime = 0; ///< Vrijeme zadnje zaprimljene poruke
const unsigned long packetTimeoutTime = 500; ///< Maksimalni interval čekanja na paket u ms

unsigned long lastBatteryCheckTime = 0; ///< Vrijeme zadnje provjere baterije
const unsigned long batteryChackInterval = 1000; ///< Interval provjere baterije u ms

// konstante za postavke servo motora
const int servoHRightLimit = 0; ///< Desna granica za horizontalni servo
const int servoHLeftLimit = 150; ///< Lijeva granica za horizontalni servo
const int servoHDefaultState = 72; ///< Defaultna pozicija za horizontalni servo (ravno)
const int servoVUpLimit = 60; ///< Gornja granica za vertikalni servo
const int servoVdownLimit = 180; ///< Donja granica za vertikalni servo
const int servoVDefaultState = 160; ///< Defaultna pozicija za vertikalni servo (ravno)
Servo servoH; ///< Horizontalni servo
Servo servoV; ///< Vertikalni servo

uint8_t flashIntensity = 0; ///< Intenzitet flash ledice

bool trenutni_sleep_state = false; ///< Trenutno stanje spavanja

/**
 * @brief Funkcija koja se poziva pri početnoj inicijalizaciji sustava.
 */
void setup() {
  // Inicijalizacija pinova
  pinMode(BUCK_5V_ENABLE_PIN, OUTPUT);
  pinMode(BUCK_3V_ENABLE_PIN, OUTPUT);
  pinMode(HC12_SET_PIN, OUTPUT);
  pinMode(ESP_SLEEP_PIN, OUTPUT);

  pinMode(MOTOR_1_PWM, OUTPUT);
  pinMode(MOTOR_1_A, OUTPUT);
  pinMode(MOTOR_1_B, OUTPUT);
  pinMode(MOTOR_2_PWM, OUTPUT);
  pinMode(MOTOR_2_A, OUTPUT);
  pinMode(MOTOR_2_B, OUTPUT);

  pinMode(BATTERY_VOLTAGE_PIN, INPUT);

  // Inicijalizacija servo motora
  servoH.attach(SERVO_H_SIGNAL);
  servoV.attach(SERVO_V_SIGNAL);
  servoH.write(servoHDefaultState);
  servoV.write(servoVDefaultState);

  // Uključivanje napajanja
  digitalWrite(BUCK_3V_ENABLE_PIN, HIGH);
  digitalWrite(BUCK_5V_ENABLE_PIN, HIGH);
  digitalWrite(HC12_SET_PIN, HIGH); 
  digitalWrite(ESP_SLEEP_PIN, HIGH);

  // Pokretanje serijske komunikacije
  Serial.begin(9600);

  // Inicijalizacija I2C komunikacije
  Wire.begin(I2C_SLAVE_ADDR);
  Wire.onRequest(setFlashIntensity);

  // Postavljanje početnih smjerova za motore
  digitalWrite(MOTOR_1_A, HIGH);
  digitalWrite(MOTOR_2_A, HIGH);
  digitalWrite(MOTOR_1_B, LOW);
  digitalWrite(MOTOR_2_B, LOW);
}

/**
 * @brief Glavna funkcija koja se izvršava u petlji.
 */
void loop() {
  receivePacket(); ///< Primanje paketa

  // Timeout detekcija
  if (millis() - lastPacketTime > packetTimeoutTime) {
    // Nema novih podataka -> sigurnosna reakcija
    //stopRobot();
    if (trenutni_sleep_state == false){
      trenutni_sleep_state = true;
      enterSleep(); ///< Prelaz u stanje spavanja
    }    
  } else {
    if (trenutni_sleep_state == true){
      trenutni_sleep_state = false;
      wakeUp(); ///< Buđenje sustava
    }
  }

  // Provjera baterije
  if (millis() - lastBatteryCheckTime > batteryChackInterval) {
    lastBatteryCheckTime = millis();
    checkBatteryState(); ///< Provjera stanja baterije
  }
}

/**
 * @brief Funkcija za primanje paketa sa serijskog porta.
 */
void receivePacket() {
  while (Serial.available()) {
    uint8_t incomingByte = Serial.read();

    if (!receiving) {
      if (incomingByte == START_BYTE) {
        receiving = true;
        packetSize = 0;
      }
    } else {
      if (incomingByte == STOP_BYTE) {
        receiving = false;
        lastPacketTime = millis();
        processPacket(); ///< Obrada paketa nakon što je primljen
      } else {
        if (packetSize < PACKET_MAX_SIZE) {
          packet[packetSize++] = incomingByte; ///< Dodavanje bytea u paket
        } else {
          // Prevelik paket, reset
          receiving = false;
          packetSize = 0;
        }
      }
    }
  }
}

/**
 * @brief Funkcija za obradu primljenih podataka iz paketa.
 */
void processPacket() {
  for (uint8_t i = 0; i < packetSize; i += 2) {
    if (i + 1 >= packetSize) break; // Provjera da li imamo cijeli par (ID + VALUE)

    uint8_t id = packet[i]; ///< ID podatka
    uint8_t value = packet[i + 1]; ///< Vrijednost podatka

    handleData(id, value); ///< Obrada podataka
  }
}

/**
 * @brief Funkcija za upravljanje podacima na temelju ID-a.
 * 
 * @param id ID podatka koji se prima
 * @param value Vrijednost koja se prima za dani ID
 */
void handleData(uint8_t id, uint8_t value) {
  switch (id) {
    case 0x01: // motor 1 speed
      setMotor1Speed(value);
      break;
    case 0x02: // motor 2 speed
      setMotor2Speed(value);
      break;
    case 0x03: // motor 1 dir
      setMotor1Dir(value);
      break;    
    case 0x04: // motor 2 dir
      setMotor2Dir(value);
      break; 
    case 0x05: // servo H
      setServoH(value);
      break;
    case 0x06: // servo V
      setServoV(value);
      break;
    case 0x07: // flash
      setFlashIntensity(value);
      break;
    default:
      // Nepoznat ID, ignoriraj
      break;
  }
}

// Funkcije za upravljanje motorima, servo motorima i intenzitetom svjetla:

/**
 * @brief Funkcija za postavljanje brzine motora 1.
 */
void setMotor1Speed(uint8_t speed) {
  static uint8_t brzina = 0;
  if(brzina != speed){
    brzina = speed;
    analogWrite(MOTOR_1_PWM, speed);
  }
}

/**
 * @brief Funkcija za postavljanje brzine motora 2.
 */
void setMotor2Speed(uint8_t speed) {
  static uint8_t brzina = 0;
  if(brzina != speed){
    brzina = speed;
    analogWrite(MOTOR_2_PWM, speed);
  }
}

/**
 * @brief Funkcija za postavljanje smjera motora 1.
 */
void setMotor1Dir(uint8_t dir) {
  static uint8_t lastDir = 2;
  if(lastDir != dir){
    if(dir > 0){
      digitalWrite(MOTOR_1_A, HIGH);
      digitalWrite(MOTOR_1_B, LOW);
      lastDir = 1;
    }
    else{
      digitalWrite(MOTOR_1_A, LOW);
      digitalWrite(MOTOR_1_B, HIGH);
      lastDir = 0;
    }
  }
}

/**
 * @brief Funkcija za postavljanje smjera motora 2.
 */
void setMotor2Dir(uint8_t dir) {
  static uint8_t lastDir = 2;
  if(lastDir != dir){
    if(dir > 0){
      digitalWrite(MOTOR_2_A, HIGH);
      digitalWrite(MOTOR_2_B, LOW);
      lastDir = 1;
    }
    else{
      digitalWrite(MOTOR_2_A, LOW);
      digitalWrite(MOTOR_2_B, HIGH);
      lastDir = 0;
    }
  }
}

/**
 * @brief Funkcija za postavljanje horizontalnog servo motora.
 */
void setServoH(uint8_t angle) {
  static uint8_t kut = -1;
  
  if(kut != angle ){
    kut = angle;
    servoH.write((int) map(kut, 255, 0, servoHRightLimit, servoHLeftLimit));
  }
}

/**
 * @brief Funkcija za postavljanje vertikalnog servo motora.
 */
void setServoV(uint8_t angle) {
  static uint8_t kut = -1;

  if(kut != angle ){
    kut = angle;
    servoV.write((int) map(kut, 0, 255, servoVUpLimit, servoVdownLimit));
  }
}

/**
 * @brief Funkcija za memorira zadani intenzitet flash LED-a.
 */
void setFlashIntensity(uint8_t intensity) {
  flashIntensity = intensity;
}

/**
 * @brief Funkcija za zaustavljanje robota.
 */
void stopRobot() {
  setMotor1Speed(0);
  setMotor2Speed(0);

  setFlashIntensity(0);
}

/**
 * @brief Funkcija za prelazak u stanje spavanja.
 */
void enterSleep() {
  Serial.println("spavam");
  stopRobot();
  digitalWrite(BUCK_5V_ENABLE_PIN, LOW);
  digitalWrite(ESP_SLEEP_PIN, LOW);
}

/**
 * @brief Funkcija za buđenje robota.
 */
void wakeUp() {
  Serial.println("budim se");
  digitalWrite(BUCK_5V_ENABLE_PIN, HIGH);
  digitalWrite(ESP_SLEEP_PIN, HIGH);
}

/**
 * @brief Funkcija za provjeru stanja baterije.
 */
void checkBatteryState() {
  unsigned int izmjerenaVrijednost = analogRead(BATTERY_VOLTAGE_PIN);
  unsigned int napon = map(izmjerenaVrijednost, 0, 1023, 0, 20);
  if (napon < NAPON_BATERIJE_LIMIT) {
    Serial.write(0x01); // Obavijest o praznoj bateriji
  } else {
    Serial.write(0x00); // Baterija nije prazna
  }
}

/**
 * @brief Funkcija za postavljanje intenziteta flash LED-a via I2C.
 */
void setFlashIntensity() {
  Wire.write(flashIntensity);     // Pošalji intenzitet za flash led
}
