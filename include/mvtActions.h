#include "commKeywords.h"
#include "commFrame.h"
#include "turn_and_go.h"
#include "PWMServo.h"
#include <Arduino.h>


void actionsFSM(commFrame &frame, PWMServo &servo1, PWMServo &servo2, PWMServo &servo3, TurnAndGo &steppers,
    uint8_t &moveAvailable, bool &moveFinished, float &X, float &Y);
