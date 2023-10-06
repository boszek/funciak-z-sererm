#include <IBusBM.h>
#include <Servo.h>

IBusBM IBus;    // IBus object
Servo ESC;

#define BUTTON_ON 2000
#define WEAPON 11

#define WEAPON_MIN_PWM 60
#define WEAPON_MAX_PWM 250

#define LEFT_FORWARD 3
#define LEFT_BACKWARD 5
#define RIGHT_FORWARD 6
#define RIGHT_BACKWARD 10

// void initEsc() {
//   delay(2000);                                                                                           
//   ledOn();
//   analogWrite(WEAPON, WEAPON_MAX_PWM);
//   delay(2000);
//   analogWrite(WEAPON, 0);
//   delay(3000);
//   analogWrite(WEAPON, WEAPON_MIN_PWM);
//   delay(2000);
//   ledOff();
// }

void ledOn() {
  digitalWrite(LED_BUILTIN, 1);
}

void ledOff() {
  digitalWrite(LED_BUILTIN, 0);
}

void setup() {
  // put your setup code here, to run once:
  IBus.begin(Serial);    // iBUS object connected to serial0 RX pin
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(10, OUTPUT);

  analogWrite(LEFT_FORWARD ,0);
  analogWrite(LEFT_BACKWARD ,0);
  analogWrite(RIGHT_FORWARD ,0);
  // analogWrite(RIGHT_BACKWARD ,0);
  analogWrite(WEAPON , 0);

  ledOn();
  ESC.attach(WEAPON,1000,2000);
  ESC.write(2000);
  delay(2000);
  ESC.write(1000);
  delay(5000);
  ledOff();
  // ESC.detach();
  // pinMode(WEAPON, OUTPUT);
}

int bit = 0, button = 0, ver = 0, hor = 0, throttle = 0;

int readChannels() {
  int v = IBus.readChannel(0);
  int h = IBus.readChannel(1);
  int t = IBus.readChannel(2); 
  int b = IBus.readChannel(5);


  if(v > 2000 || h > 2000 || v < 1000 || h < 1000 || t < 1000 || t > 2000) {
    return 1; 
  }
  ver = map(v, 1000, 2000, -250, 250);
  hor =  map(h, 1000, 2000, -250, 250);
  throttle = t;
  button = b;
  
  return 0;
}

void loop() {
  readChannels();
  if(button == BUTTON_ON) {
    hor = -hor;
  }  
   int left = ver + hor;
   int right = ver - hor;
 
  analogWrite(LEFT_FORWARD, min(255, abs(max(0, left))));
  analogWrite(LEFT_BACKWARD, min(255, abs(min(0, left))));
  analogWrite(RIGHT_FORWARD, min(255, abs(max(0, right))));
  // analogWrite(RIGHT_BACKWARD, min(255, abs(min(0, right))));
  ESC.write(throttle);

  int right_back = min(255, abs(min(0, right)));
  if(right_back > 0) {
      digitalWrite(RIGHT_BACKWARD, 1);
      delayMicroseconds(10*right_back);
      digitalWrite(RIGHT_BACKWARD, 0);
      delayMicroseconds(10*(255-right_back));
    }  else {
      digitalWrite(RIGHT_BACKWARD, 0);
      delay(5);
    }
}