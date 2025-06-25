#ifndef ACCESS_CONTROL_SYSTEM_H
#define ACCESS_CONTROL_SYSTEM_H

#include <Arduino.h>
#include <ESP32Servo.h>

struct System {
  Servo* servo;
  int sensorPin;
  int greenLed;
  int redLed;
  int pos;
  int minPos;
  int maxPos;
  bool movingUp;
  bool movingDown;
  bool pause;
  bool blinking;
  bool postBlinking;
  bool directionUp;
  unsigned long lastMoveTime;
  unsigned long pauseStart;
  unsigned long lastBlinkTime;
  int blinkCount;
  bool blinkToggleState;
  unsigned int moveUpInterval;
  unsigned int moveDownInterval;
  bool reversed;
  bool waitingForButton;
};

// Shared constants
const unsigned long blinkInterval = 200;
const int totalAltBlinks = 4;
const int postBlinkCountMax = 6;
const int piezoPin = 23;
const int heartbeatLedPin = 25;
const int buttonPin = 2;
const unsigned long heartbeatInterval = 500;

// Shared heartbeat
extern unsigned long lastHeartbeatMillis;
extern bool heartbeatLedState;

// Global system instances
extern System sys1;
extern System sys2;
extern bool paused;
#endif