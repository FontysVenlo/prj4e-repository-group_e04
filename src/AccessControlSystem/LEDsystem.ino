#include "AccessControlSystem.h"
#include "LEDSystem.h"

void updateHeartbeat(unsigned long currentMillis) {
  if (currentMillis - lastHeartbeatMillis >= heartbeatInterval) {
    heartbeatLedState = !heartbeatLedState;
    digitalWrite(heartbeatLedPin, heartbeatLedState ? HIGH : LOW);
    lastHeartbeatMillis = currentMillis;
  }
}

void handlePostBlinking(System& sys, unsigned long currentMillis) {
  if (!sys.postBlinking) return;

  if (currentMillis - sys.lastBlinkTime >= blinkInterval) {
    sys.blinkToggleState = !sys.blinkToggleState;
    sys.lastBlinkTime = currentMillis;

    bool ledIsRed = sys.directionUp;
    digitalWrite(sys.redLed, ledIsRed && sys.blinkToggleState ? HIGH : LOW);
    digitalWrite(sys.greenLed, !ledIsRed && sys.blinkToggleState ? HIGH : LOW);

    if (!sys.blinkToggleState) sys.blinkCount++;

    if (sys.blinkCount >= postBlinkCountMax / 2) {
      sys.postBlinking = false;
      digitalWrite(sys.redLed, ledIsRed ? HIGH : LOW);
      digitalWrite(sys.greenLed, ledIsRed ? LOW : HIGH);

      if (sys.directionUp) {
        sys.movingUp = true;
        if (&sys == &sys1) tone(piezoPin, 1500, 300);
      } else {
        sys.movingDown = true;
        if (&sys == &sys1) tone(piezoPin, 600, 300);
      }
    }
  }
}