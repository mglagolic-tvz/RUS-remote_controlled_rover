#include <PS3BT.h>
#include <usbhub.h>

#define ENABLE_UHS_DEBUGGING 1

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

// Definicije
#define START_BYTE 0xAA
#define STOP_BYTE 0x55
#define PACKET_MAX_SIZE 32  // Maksimalna veličina paketa

// Definicija pinova
#define LED_EMPTY_BATTERY 13

USB Usb;
BTD Btd(&Usb);
PS3BT PS3(&Btd);

// Paket koji ćemo slati
uint8_t packet[PACKET_MAX_SIZE];
uint8_t packetSize = 0;
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 100;  // ms

// globalne varijable za kontrole koje se salju
const uint8_t servoHDefaultPosition = 127;
const uint8_t servoVDefaultPosition = 250;
uint8_t motor1pwm = 0;
uint8_t motor2pwm = 0;
uint8_t motor1dir = 1;
uint8_t motor2dir = 1;
uint8_t servoH = servoHDefaultPosition;
uint8_t servoV = servoVDefaultPosition;
uint8_t ledIntensity = 0;
int led_intensity_counter;

//bool sleep = false;

void setup() {
  pinMode(LED_EMPTY_BATTERY, OUTPUT);
  Serial.begin(9600);

  if (Usb.Init() == -1) {
    while (1) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
    }
  }
  digitalWrite(LED_EMPTY_BATTERY, LOW);
}

void loop() {

  Usb.Task();

  if(Serial.available()){
    uint8_t battery_state = Serial.read();
    if(battery_state == 0x01){
      digitalWrite(LED_EMPTY_BATTERY, HIGH);
    }
    if(battery_state == 0x00){
      digitalWrite(LED_EMPTY_BATTERY, LOW);
    }
  }

  unsigned long currentMillis = millis();

  if (PS3.PS3Connected && currentMillis - lastSendTime >= sendInterval) {
    //sleep = false;
    lastSendTime = currentMillis;

    if (!PS3.getButtonPress(R1)) {
      if (PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117) {

        // mapiranje
        int pwm = map(PS3.getAnalogHat(LeftHatY), 0, 255, 255, -255);

        //postavljanje pinova
        if (pwm >= 0) {
          motor1dir = 1;  //naprijed
          motor1pwm = pwm;
        }
        if (pwm < 0) {
          motor1dir = 0;  //unazad
          motor1pwm = -pwm;
        }

      } else {
        motor1pwm = 0;
      }

      if (PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117) {

        // mapiranje
        int pwm = map(PS3.getAnalogHat(RightHatY), 0, 255, 255, -255);

        //postavljanje pinova
        if (pwm >= 0) {
          motor2dir = 1;  //naprijed
          motor2pwm = pwm;
        }
        if (pwm < 0) {
          motor2dir = 0;  //unazad
          motor2pwm = -pwm;
        }

      } else {
        motor2pwm = 0;
      }
      servoH = servoHDefaultPosition;
      servoV = servoVDefaultPosition;
    }
    if (PS3.getButtonPress(R1)) {

      servoV = PS3.getAnalogHat(RightHatY);
      servoH = PS3.getAnalogHat(RightHatX);
      motor1pwm = 0;
      motor2pwm = 0;
    }



    // Analog button values can be read from almost all buttons
    //if (PS3.getAnalogButton(L2) || PS3.getAnalogButton(R2)) {
    //}

    // pojačaj rasvjetu pritiskom na strelicu gore
    if (PS3.getButtonClick(UP)) {
      if (led_intensity_counter < 5){
        led_intensity_counter++;
        ledIntensity = led_intensity_counter * 40;
      }
    }
    // smanji rasvjetu pritiskom na strelicu dolje
    if (PS3.getButtonClick(DOWN)) {
      if (led_intensity_counter > 0){
        led_intensity_counter--;
        ledIntensity = led_intensity_counter * 40;
      }
    }

    if (PS3.getButtonClick(PS)) {
      //sleep = true;
      PS3.disconnect();
    }
/*
    if (PS3.getButtonClick(TRIANGLE)) {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    if (PS3.getButtonClick(CIRCLE)) {
      digitalWrite(LED_BUILTIN, LOW);
    }*/
    /*if (PS3.getButtonClick(SELECT)) {
      sleep = true;
    }*/
    /*
    if (PS3.getButtonClick(CROSS)) {}
    if (PS3.getButtonClick(SQUARE)) {}
    if (PS3.getButtonClick(UP)) {}
    if (PS3.getButtonClick(RIGHT)) {}
    if (PS3.getButtonClick(DOWN)) {}
    if (PS3.getButtonClick(LEFT)) {}
    if (PS3.getButtonClick(L1)) {}
    if (PS3.getButtonClick(L3)) {}
    if (PS3.getButtonClick(R1)) {}
    if (PS3.getButtonClick(R3)) {}
    if (PS3.getButtonClick(SELECT)) {}
    if (PS3.getButtonClick(START)) {}
*/
    /*
    if (printAngle) {
    }
*/
    // Pripremi novi paket
    createPacket();

    // Pošalji paket
    Serial.write(packet, packetSize);
    //Serial.println("povezan sam");
  }
}

void createPacket() {
  packetSize = 0;

  // Start byte
  packet[packetSize++] = START_BYTE;

  // Dodavanje podataka - primjer:


  addToPacket(0x01, motor1pwm);
  addToPacket(0x02, motor2pwm);
  addToPacket(0x03, motor1dir);
  addToPacket(0x04, motor2dir);
  addToPacket(0x05, servoH);
  addToPacket(0x06, servoV);
  addToPacket(0x07, ledIntensity);

  // postavljanje paketa za spavanje
  /*if(sleep)
    addToPacket(0x09, 0x01);
  else{
    addToPacket(0x09, 0x00);
  }*/

  // Stop byte
  packet[packetSize++] = STOP_BYTE;
}

void addToPacket(uint8_t id, uint8_t value) {
  if (packetSize + 2 < PACKET_MAX_SIZE) {
    packet[packetSize++] = id;     // ID podatka
    packet[packetSize++] = value;  // Vrijednost
  }
}
