#include <IBusBM.h>
#include "HardwareSerial.h"
//#include <Servo.h>

// static const uint8_t TX = 7;
// static const uint8_t RX = 6;

IBusBM IBus;    // IBus object

#define BUTTON_ON 2000
#define WEAPON D10

#define WEAPON_MIN_PWM 60
#define WEAPON_MAX_PWM 250

#define LEFT_FORWARD D0
#define LEFT_BACKWARD D1
#define RIGHT_FORWARD D2
#define RIGHT_BACKWARD D3

void ledOn() {
  digitalWrite(LED_BUILTIN, 1);
}

void ledOff() {
  digitalWrite(LED_BUILTIN, 0);
}

void setup() {
  
  Serial.begin(115200);
  Serial.println("Hello, world?");
  analogWriteFrequency(WEAPON, 200);

  IBus.begin(Serial1, 0, D7, D6);    // iBUS object connected to serial0 RX pin
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RIGHT_BACKWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(LEFT_BACKWARD, OUTPUT);
  pinMode(WEAPON, OUTPUT);
  


  //ledOn();

  Serial.println("ok");

  analogWrite(LEFT_FORWARD ,0);
  analogWrite(LEFT_BACKWARD ,0);
  analogWrite(RIGHT_FORWARD ,0);
  analogWrite(RIGHT_BACKWARD ,0);
  analogWrite(WEAPON , 127);

}

int bit = 0, button = 0, ver = 0, hor = 0, throttle = 0, left=0, right=0;

int readChannels() {
  int v = IBus.readChannel(0);
  int h = IBus.readChannel(1);
  int t = IBus.readChannel(2); 
  int b = IBus.readChannel(5);


  if(v > 2000 || h > 2000 || v < 1000 || h < 1000 || t < 1000 || t > 2000) {
    ver = 0;
    hor = 0;
    throttle = 127;
    Serial.println("0");
    return 1; 
  }

  ver = map(v, 1000, 2000, -255, 255);
  hor =  map(h, 1000, 2000, -255, 255);
  throttle = map(t, 0, 2000, 0, 255);
  button = b;

  Serial.print(throttle, DEC);
  Serial.print(" ");
  Serial.print(ver, DEC);
  Serial.print(" ");
  Serial.print(hor, DEC);
  Serial.println();
  
  return 0;
}

void loop() {
  readChannels();
  delay(10);
  Serial.write("_");

  if(button == BUTTON_ON) {
    hor = -hor;
  }  
  left = ver + hor;
  right = ver - hor;
  analogWrite(WEAPON, throttle);

  if(left >= 0) {
    analogWrite(LEFT_FORWARD, min(255, left));
    analogWrite(LEFT_BACKWARD, 0);
  } else {
    analogWrite(LEFT_BACKWARD, min(255, -left));
    analogWrite(LEFT_FORWARD, 0);
  }  
  
  if(right >= 0) {
    analogWrite(RIGHT_FORWARD, min(255, right));
    analogWrite(RIGHT_BACKWARD, 0);
  } else {
    analogWrite(RIGHT_BACKWARD, min(255, -right));
    analogWrite(RIGHT_FORWARD, 0);
  }
}