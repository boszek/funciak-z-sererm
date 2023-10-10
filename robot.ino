#include <IBusBM.h>
#include <Servo.h>

IBusBM IBus;    // IBus object

#define BUTTON_ON 2000
#define WEAPON 11

#define WEAPON_MIN_PWM 60
#define WEAPON_MAX_PWM 250

#define LEFT_FORWARD 3
#define LEFT_BACKWARD 5
#define RIGHT_FORWARD 6
#define RIGHT_BACKWARD 10

void ledOn() {
  digitalWrite(LED_BUILTIN, 1);
}

void ledOff() {
  digitalWrite(LED_BUILTIN, 0);
}

void setup() {
  IBus.begin(Serial);    // iBUS object connected to serial0 RX pin
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RIGHT_BACKWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);

  analogWrite(LEFT_FORWARD ,0);
  analogWrite(LEFT_BACKWARD ,0);
  digitalWrite(RIGHT_FORWARD ,0);
  digitalWrite(RIGHT_BACKWARD ,0);
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
    return 1; 
  }

  ver = map(v, 1000, 2000, -255, 255);
  hor =  map(h, 1000, 2000, -255, 255);
  throttle = map(t, 0, 2000, 0, 255);
  button = b;
  
  return 0;
}

void loop() {
  readChannels();
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
  
  if(right == 0) {
    digitalWrite(RIGHT_BACKWARD, 0);
    digitalWrite(RIGHT_FORWARD, 0);
      delay(5);
  } else {
      digitalWrite(right < 0 ? RIGHT_BACKWARD : RIGHT_FORWARD, 1);
      delayMicroseconds(10*min(255, abs(right)));
      digitalWrite(RIGHT_BACKWARD, 0);
      digitalWrite(RIGHT_FORWARD, 0);
      delayMicroseconds(10*(255-min(255, abs(right))));
  }
}