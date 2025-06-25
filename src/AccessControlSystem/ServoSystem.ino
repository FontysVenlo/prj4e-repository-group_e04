#include "AccessControlSystem.h"
#include "ServoSystem.h"

void setupSystem(System& sys, int servoPin) {
  sys.servo->attach(servoPin);
  pinMode(sys.sensorPin, INPUT);
  pinMode(sys.greenLed, OUTPUT);
  pinMode(sys.redLed, OUTPUT);
  sys.servo->write(sys.pos);
  digitalWrite(sys.greenLed, HIGH);
  digitalWrite(sys.redLed, LOW);
}

void handleSystem(System& sys, unsigned long currentMillis) {
  if (paused) return;

  // Allow sys1 to resume when access button is pressed
  if (&sys == &sys1 && sys.waitingForButton) {
    if (digitalRead(buttonPin) == LOW) {
      sys.waitingForButton = false;
      sys.blinking = false;
      sys.postBlinking = true;
      sys.lastBlinkTime = currentMillis;
      digitalWrite(sys.greenLed, LOW);
      digitalWrite(sys.redLed, LOW);
      return;  // Exit early this loop to avoid triggering again
    } else {
      return;  // Still waiting for button
    }
  }
  int sensorValue = digitalRead(sys.sensorPin);

  if (sensorValue == HIGH && !sys.movingUp && !sys.movingDown && !sys.pause &&
      !sys.blinking && !sys.postBlinking && !sys.waitingForButton) {
    sys.blinking = true;
    sys.blinkCount = 0;
    sys.lastBlinkTime = currentMillis;
    sys.blinkToggleState = false;
  }

  handleBlinking(sys, currentMillis);
  handlePostBlinking(sys, currentMillis);
  handlePause(sys, currentMillis);
  moveServoUp(sys, currentMillis);
  moveServoDown(sys, currentMillis);
}

void handleBlinking(System& sys, unsigned long currentMillis) {
  if (!sys.blinking) return;

  if (currentMillis - sys.lastBlinkTime >= blinkInterval) {
    sys.blinkToggleState = !sys.blinkToggleState;
    digitalWrite(sys.greenLed, sys.blinkToggleState ? HIGH : LOW);
    digitalWrite(sys.redLed, sys.blinkToggleState ? LOW : HIGH);

    if (!sys.blinkToggleState) sys.blinkCount++;
    sys.lastBlinkTime = currentMillis;

    if (sys.blinkCount >= totalAltBlinks) {
      sys.blinkCount = 0;
      sys.blinkToggleState = false;

      if (&sys == &sys1) {
        sys.waitingForButton = true;
      } else {
        sys.blinking = false;
        sys.postBlinking = true;
        sys.lastBlinkTime = currentMillis;
        digitalWrite(sys.greenLed, LOW);
        digitalWrite(sys.redLed, LOW);
      }
    }
  }
}

void moveServoUp(System& sys, unsigned long currentMillis) {
  if (sys.movingUp && currentMillis - sys.lastMoveTime >= sys.moveUpInterval) {
    if ((sys.reversed && sys.pos > sys.minPos) || (!sys.reversed && sys.pos < sys.maxPos)) {
      sys.pos += sys.reversed ? -1 : 1;
      sys.servo->write(sys.pos);
      sys.lastMoveTime = currentMillis;
    } else {
      sys.movingUp = false;
      sys.pause = true;
      sys.pauseStart = currentMillis;
      sys.directionUp = false;
      digitalWrite(sys.redLed, HIGH);
      digitalWrite(sys.greenLed, LOW);
      noTone(piezoPin);
    }
  }
}

void moveServoDown(System& sys, unsigned long currentMillis) {
  if (sys.movingDown && currentMillis - sys.lastMoveTime >= sys.moveDownInterval) {
    if ((sys.reversed && sys.pos < sys.maxPos) || (!sys.reversed && sys.pos > sys.minPos)) {
      sys.pos += sys.reversed ? 1 : -1;
      sys.servo->write(sys.pos);
      sys.lastMoveTime = currentMillis;
    } else {
      sys.movingDown = false;
      sys.directionUp = true;
      digitalWrite(sys.greenLed, HIGH);
      digitalWrite(sys.redLed, LOW);
      noTone(piezoPin);
    }
  }
}

void handlePause(System& sys, unsigned long currentMillis) {
  if (sys.pause && currentMillis - sys.pauseStart >= 4000) {
    sys.pause = false;
    sys.blinking = true;
    sys.blinkCount = 0;
    sys.lastBlinkTime = currentMillis;
    sys.blinkToggleState = false;
  }
}