#ifndef SERVO_SYSTEM_H
#define SERVO_SYSTEM_H

#include "AccessControlSystem.h"

void setupSystem(System& sys, int servoPin);
void handleSystem(System& sys, unsigned long currentMillis);
void handleBlinking(System& sys, unsigned long currentMillis);
void moveServoUp(System& sys, unsigned long currentMillis);
void moveServoDown(System& sys, unsigned long currentMillis);
void handlePause(System& sys, unsigned long currentMillis);

#endif