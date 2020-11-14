#include "mvtActions.h"
#include "commFrame.h"
#include "commKeywords.h"
#include "turn_and_go.h"
#include "PWMServo.h"
#include "debug.h"

#include <Arduino.h>


void actionsFSM(commFrame &frame, PWMServo &servo1, PWMServo &servo2, PWMServo &servo3, 
TurnAndGo &steppers, uint8_t &moveAvailable, bool &moveFinished, float &X, float &Y){

    #ifdef debugActions
        Serial.println("FSM BEGIN");
    #endif
    uint8_t ID = frame.getId();
    float value2,value1,value0;

    frame.getValues(value2,value1,value0);

    #ifdef debugActions
        Serial.print("  ID : ");
        Serial.println(ID,HEX);
        Serial.print("  Value2: ");
        Serial.println(value2);
        Serial.print("  Value1: ");
        Serial.println(value1);
        Serial.print("  Value0: ");
        Serial.println(value0);
    #endif
    switch (ID)
    {
    case ID_STOP:
        /* code */
        //steppers.stop();
        break;
    
    case ID_goTo_XY:
        /* code */
        #ifdef debugActions
            Serial.println("STEPPER XY!");
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
        #ifdef debugActions
            Serial.println("SERVO... ");
        #endif
        
        if((int)value0 == 1){
            servo1.write((int)value1);
            #ifdef debugActions
                Serial.println("SERVO 1");
            #endif
        }
        else if ((int)value0 == 2){
            servo2.write((int)value1);
            #ifdef debugActions
                Serial.println("SERVO 2");
            #endif
        }
        else if ((int)value0 == 3){
            servo3.write((int)value1);
            #ifdef debugActions
                Serial.println("SERVO 3");
            #endif
        }
        break;

    case ID_SPEED_CONF:
        /* code */
        #ifdef debugActions
            Serial.println("SPEED");
        #endif
        steppers.setMaximumSpeed(value0);   
        break;

    case ID_ACCEL_CONF:
        /* code */
        #ifdef debugActions
            Serial.println("ACCEL");
        #endif
        steppers.setAcceleration(value0);
        break;

    default:
        #ifdef debugActions
            Serial.println("CASE DEFAULT!");
        #endif
        break;
    }
    #ifdef debugActions
        Serial.println("END FSM");
    #endif
}