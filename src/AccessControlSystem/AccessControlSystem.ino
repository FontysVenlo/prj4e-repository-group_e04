#include "AccessControlSystem.h"
#include "LEDSystem.h"
#include "SensorSystem.h"
#include "ServoSystem.h"

System sys1 = { new Servo(), 34, 14, 13, 0, 0, 120, false, false, false, false, false, true, 0, 0, 0, 0, false, 5, 5, false, false };
System sys2 = { new Servo(), 35, 15, 12, 110, 0, 110, false, false, false, false, false, true, 0, 0, 0, 0, false, 2, 5, true, false };
unsigned long lastHeartbeatMillis = 0;
bool heartbeatLedState = false;

bool paused = false;
const int pauseButtonPin = 0;
unsigned long lastPauseButtonPress = 0;
const unsigned long debounceDelay = 250;

void setup() {
  setupSystem(sys1, 22);
  setupSystem(sys2, 21);
  pinMode(piezoPin, OUTPUT);
  pinMode(heartbeatLedPin, OUTPUT);
  digitalWrite(heartbeatLedPin, LOW);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(pauseButtonPin, INPUT_PULLUP);
}

void handlePauseButton(unsigned long currentMillis) {
  if (digitalRead(pauseButtonPin) == LOW) {
    if (currentMillis - lastPauseButtonPress > debounceDelay) {
      paused = !paused;
      lastPauseButtonPress = currentMillis;

      if (paused) {
        stopAllSystems();
      } else {
        resumeAllSystems();
      }
    }
  }
}

void stopAllSystems() {
  sys1.servo->write(sys1.pos);
  sys2.servo->write(sys2.pos);

  digitalWrite(sys1.greenLed, LOW);
  digitalWrite(sys1.redLed, LOW);
  digitalWrite(sys2.greenLed, LOW);
  digitalWrite(sys2.redLed, LOW);

  noTone(piezoPin);

  sys1.movingUp = false;
  sys1.movingDown = false;
  sys1.pause = false;
  sys1.blinking = false;
  sys1.postBlinking = false;
  sys1.waitingForButton = false;

  sys2.movingUp = false;
  sys2.movingDown = false;
  sys2.pause = false;
  sys2.blinking = false;
  sys2.postBlinking = false;
  sys2.waitingForButton = false;
}

void resumeAllSystems() {
  digitalWrite(sys1.greenLed, HIGH);
  digitalWrite(sys1.redLed, LOW);
  digitalWrite(sys2.greenLed, HIGH);
  digitalWrite(sys2.redLed, LOW);

  sys1.movingUp = false;
  sys1.movingDown = false;
  sys1.pause = false;
  sys1.blinking = false;
  sys1.postBlinking = false;
  sys1.waitingForButton = false;

  sys2.movingUp = false;
  sys2.movingDown = false;
  sys2.pause = false;
  sys2.blinking = false;
  sys2.postBlinking = false;
  sys2.waitingForButton = false;

  sys1.servo->write(sys1.pos);
  sys2.servo->write(sys2.pos);
}

void loop() {
  unsigned long currentMillis = millis();
  updateHeartbeat(currentMillis);
  handlePauseButton(currentMillis);

// Continue only if not paused
if (!paused) {
  handleSystem(sys1, currentMillis);
  handleSystem(sys2, currentMillis);
  }
}
