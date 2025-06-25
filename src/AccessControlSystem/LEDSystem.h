#ifndef LED_SYSTEM_H
#define LED_SYSTEM_H

void updateHeartbeat(unsigned long currentMillis);
void handlePostBlinking(System& sys, unsigned long currentMillis);

#endif