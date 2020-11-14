#include <Arduino.h>
#include "pins_mvt.h"
#include "turn_and_go.h"
#include "PWMServo.h"
#include "commFrame.h"
#include "commKeywords.h"
#include "mvtActions.h"
#include "debug.h"

PWMServo ServoGauche;
PWMServo ServoDroite;
PWMServo Servo3;

TurnAndGo turn_and_go;
float distance = -500;

bool go = false;

commFrame stratFrame(FRAMESTARTER);
uint8_t moveAvailable = 0;
bool moveFinished = false;

float X, Y;
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
  //Verification etat de rotation des steppers
  if(turn_and_go.run()==STOP){
    moveFinished = true;
  }

  //Ecoute du port serie
  if(Strat.available()>0){

    if(Strat.read()==stratFrame.getStarter()){
      //Trame detectée
      while(Strat.available()==0); //Attente de la reception de la suite du message
      delay(20);
      readFrame(stratFrame);
      actionsFSM(stratFrame,ServoGauche,ServoDroite,Servo3,turn_and_go,moveAvailable,moveFinished, X, Y);
      }
    }
  else{
    if(moveFinished and moveAvailable == 1){
      turn_and_go.goTo(X,Y);
    }
  }
  
  
  
}
  
