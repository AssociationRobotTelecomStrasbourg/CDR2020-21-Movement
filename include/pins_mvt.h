//Fichier de configuration des pins pour la Teensy 3.2 Mouvement
#ifndef PINS_MVT_H
#define PINS_MVT_H

//Pins Servo-moteurs
#define SERVO1 3
#define SERVO2 4
#define SERVO3 5

//Bus UART vers Strat
#define Strat Serial2

//Pins LEDs
#define LED1 15
#define LED2 14

//Pins TMC2130
    //1
#define DIR1 16
#define CS1 17
#define EN1 18
#define STEP1 20

    //2
#define DIR2 19
#define CS2 22
#define EN2 23
#define STEP2 21

//Pins SPI
#define SCK 13
#define SDO 12
#define SDI 11
#endif