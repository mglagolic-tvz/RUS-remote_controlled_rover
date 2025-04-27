#include "pinovi.h"
#include <Servo.h>
#include <Wire.h>

// slave adresa arduina za i2c komunikaciju sa esp32-cam
#define I2C_SLAVE_ADDR 0x08

// postavke za paketnu komunikaciju
#define START_BYTE 0xAA
#define STOP_BYTE  0x55
#define PACKET_MAX_SIZE 32


// varijable za paketnu komunikaciju
uint8_t packet[PACKET_MAX_SIZE];
uint8_t packetSize = 0;
bool receiving = false;
unsigned long lastPacketTime = 0;
const unsigned long timeoutTime = 500; // ms

//const uint8_t kod_za_pocetno_stanje_servo = 0; //ako primi ovu velicinu postavlja se u dolje definirane pozicije
const int servoHRightLimit = 0;
const int servoHLeftLimit = 150;
const int servoHDefaultState = 72; // 0 desno 160 lijevo 72 ravno
const int servoVUpLimit = 60;
const int servoVdownLimit = 180;
const int servoVDefaultState = 160; // 60 gore 180 dolje 160 ravno
Servo servoH;
Servo servoV;

uint8_t flashIntensity = 0;

void setup() {
  pinMode(BUCK_5V_ENABLE_PIN, OUTPUT);
  pinMode(BUCK_3V_ENABLE_PIN, OUTPUT);
  pinMode(HC12_SET_PIN, OUTPUT);

  pinMode(MOTOR_1_PWM, OUTPUT);
  pinMode(MOTOR_1_A, OUTPUT);
  pinMode(MOTOR_1_B, OUTPUT);
  pinMode(MOTOR_2_PWM, OUTPUT);
  pinMode(MOTOR_2_A, OUTPUT);
  pinMode(MOTOR_2_B, OUTPUT);

  servoH.attach(SERVO_H_SIGNAL);
  servoV.attach(SERVO_V_SIGNAL);
  servoH.write(servoHDefaultState);
  servoV.write(servoVDefaultState);

  digitalWrite(BUCK_3V_ENABLE_PIN, HIGH); // ukljuci 3.3V napajanje
  digitalWrite(BUCK_5V_ENABLE_PIN, HIGH); // ukljuci 5V napajanje
  digitalWrite(HC12_SET_PIN, HIGH); //onemoguci AT mod

  Serial.begin(9600);

  Wire.begin(I2C_SLAVE_ADDR);
  Wire.onRequest(setFlashIntensity);

  digitalWrite(MOTOR_1_A, HIGH);
  digitalWrite(MOTOR_2_A, HIGH);
  digitalWrite(MOTOR_1_B, LOW);
  digitalWrite(MOTOR_2_B, LOW);

}

void loop() {
  receivePacket();

  // Timeout detekcija
  if (millis() - lastPacketTime > timeoutTime) {
    // Nema novih podataka -> sigurnosna reakcija
    stopRobot();
  }

}

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
        processPacket();
      } else {
        if (packetSize < PACKET_MAX_SIZE) {
          packet[packetSize++] = incomingByte;
        } else {
          // Prevelik paket, reset
          receiving = false;
          packetSize = 0;
        }
      }
    }
  }
}

void processPacket() {
  for (uint8_t i = 0; i < packetSize; i += 2) {
    if (i + 1 >= packetSize) break; // Provjera da li imamo cijeli par (ID + VALUE)

    uint8_t id = packet[i];
    uint8_t value = packet[i + 1];

    handleData(id, value);
  }
}

void handleData(uint8_t id, uint8_t value) {
  switch (id) {
    case 0x01: // motor 1 speed
      setMotor1Speed(value);
      //Serial.print("motor1");
      //Serial.println(value);
      break;
    case 0x02: // motor 2 speed
      setMotor2Speed(value);
      //Serial.print("motor2");
      //Serial.println(value);
      break;
    case 0x03: // motor 1 dir
      setMotor1Dir(value);
      //Serial.print("motor1dir");
      //Serial.println(value);
      break;    
    case 0x04: // motor 2 dir
      setMotor2Dir(value);
      //Serial.print("motor2dir");
      //Serial.println(value);
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

void setMotor1Speed(uint8_t speed){
  static uint8_t brzina = 0;
  if(brzina != speed){
    brzina = speed;
    analogWrite(MOTOR_1_PWM, speed);
  }
}
void setMotor2Speed(uint8_t speed){
  static uint8_t brzina = 0;
  if(brzina != speed){
    brzina = speed;
    analogWrite(MOTOR_2_PWM, speed);
  }
}
void setMotor1Dir(uint8_t dir){
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
void setMotor2Dir(uint8_t dir){
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
void setServoH(uint8_t angle){
  static uint8_t kut = -1;
  
  if(kut != angle ){
    kut = angle;
    Serial.println(kut);
    servoH.write((int) map(kut, 255, 0, servoHRightLimit, servoHLeftLimit));
  }

}
void setServoV(uint8_t angle){
  static uint8_t kut = -1;

  if(kut != angle ){
    kut = angle;
    Serial.println(kut);
    servoV.write((int) map(kut, 0, 255, servoVUpLimit, servoVdownLimit));
  }
}
void setFlashIntensity(uint8_t intensity){
  flashIntensity = intensity;
}

void stopRobot() {
  setMotor1Speed(0);
  setMotor2Speed(0);

  setFlashIntensity(0);

}

// callback funkcija za postavljanje intenziteta flash ledice esp32-cam modula
void setFlashIntensity() {
  Wire.write(flashIntensity);     // Pošalji intenzitet za flash led

}

/*
if(kut != angle && kod_za_pocetno_stanje_servo != angle){
    kut = angle;
    Serial.println(kut);
    servoV.write((int) map(kut, 0, 255, 60, 120));
  }
  if(kut != angle && kod_za_pocetno_stanje_servo == angle){
    kut = angle;
    servoV.write(servoHDefaultState);
  }
*/
