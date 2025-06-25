#include "AccessControlSystem.h"
#include "SensorSystem.h"

void checkManualButton(System& sys, unsigned long currentMillis) {
  if (sys.waitingForButton && digitalRead(buttonPin) == LOW) {
    sys.waitingForButton = false;
    sys.postBlinking = true;
    sys.blinking = false;
    sys.blinkCount = 0;
    sys.blinkToggleState = false;
    sys.lastBlinkTime = currentMillis;
    tone(piezoPin, 800, 100);
    delay(300);  // debounce
  }
}