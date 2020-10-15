#include <Arduino.h>
#include "pins_mvt.h"
#include "turn_and_go.h"
#include "PWMServo.h"

PWMServo ServoGauche;
PWMServo ServoDroite;
TurnAndGo turn_and_go;
float distance = -500;

void ServosUp();
void ServosDown();

bool go = false;

int state = 0; //variable d'etat pour direction de mouvement du robot

void setup() {
  Strat.begin(9600); //init canal comm Strat <-> mvt
  Serial.begin(9600);

  ServoDroite.attach(SERVO2);
  ServoGauche.attach(SERVO1);
  pinMode(13,OUTPUT);

  while(Strat.available()==0){
    //Attente du message de démarrage de la teensy strat avant de continuer l'initialisation
    digitalWrite(13,HIGH);
  }
  digitalWrite(13,LOW);

  if(Strat.read()==0x7E){
    Strat.write(0x7E); //Envoi de la trame de confirmation de démarrage
    digitalWrite(13,HIGH);
  }
  digitalWrite(13,LOW);
  


  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  
  
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);

  ServosUp();
  while(!go){
    //Attente du message GO
    if(Strat.available()>0 and Strat.read()==0x56){
      go = true;
      digitalWrite(LED1,LOW);
      digitalWrite(LED2,LOW);
      digitalWrite(13,HIGH);
    }
  }


}

void loop() {


  // put your main code here, to run repeatedly:
  if (turn_and_go.run() == STOP){
    delay(1000);
    switch (state)
    {
    case 0:
      turn_and_go.goTo(distance,0);
      ServosDown();
      state++;
      break;

    case 1:
      turn_and_go.goTo(distance,distance);
      state++;
      ServosUp();
      break;

    case 2:
      turn_and_go.goTo(0,distance);
      ServosDown();
      state++;
      break;

    case 3:
      turn_and_go.goTo(0,0);
      ServosUp();
      state=0;
      break;
    default:
      break;
    }
    
    //distance = -distance;
  }
}

void ServosUp(){
  ServoGauche.write(140);
  ServoDroite.write(140);
}

void ServosDown(){
  ServoGauche.write(50);
  ServoDroite.write(50);
}