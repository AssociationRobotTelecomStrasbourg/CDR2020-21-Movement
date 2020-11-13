#include "mvtActions.h"
#include "commFrame.h"
#include "commKeywords.h"
#include "turn_and_go.h"
#include "PWMServo.h"
#include "debug.h"

#include <Arduino.h>

void actionsFSM(commFrame &frame, PWMServo &servo1, PWMServo &servo2, PWMServo &servo3, 
TurnAndGo &steppers, uint8_t &moveAvailable, bool &moveFinished, float &X, float &Y){

    uint8_t ID = frame.getId();
    float value2,value1,value0;

    frame.getValues(value2,value1,value0);

    switch (ID)
    {
    case ID_STOP:
        /* code */
        steppers.stop();
        #ifdef debug
            Serial.println("STEPPER STOP!");
        #endif
        break;
    
    case ID_goTo_XY:
        /* code */
        #ifdef debug
            Serial.println("STEPPER STOP!");
        #endif
        if(moveFinished){
            moveFinished = false;
            steppers.goTo(X,Y);
        }
        else{
            moveAvailable = 1;
            X = value0;
            Y = value1;
        }
        break;

    case ID_SERVO:
        /*  */
        #ifdef debug
            Serial.println("SERVO ");
        #endif

        switch ((int)value0)
        {
        case 1:
            /* code */
            servo1.write((int)value1);
            #ifdef debug
                Serial.println("SERVO 1");
            #endif
            break;
        
        case 2:
            /* code */
            servo2.write((int)value1);
            #ifdef debug
                Serial.println("SERVO 2");
            #endif
            break;

        case 3:
            /* code */
            servo3.write((int)value1);
            #ifdef debug
                Serial.println("SERVO 3");
            #endif
            break;
        default:
            break;
        }

    case ID_SPEED_CONF:
        /* code */
        steppers.setMaximumSpeed(value0);
        #ifdef debug
            Serial.println("SPEED");
        #endif
        break;

    case ID_ACCEL_CONF:
        /* code */
        steppers.setAcceleration(value0);
        #ifdef debug
            Serial.println("ACCEL");
        #endif
        break;

    default:
        break;
    }
}