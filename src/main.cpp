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

unsigned long time;
unsigned long delai = 250;
bool led13_state;

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

  turn_and_go.goTo(10,10); //Ligne d'initialisation des moteurs
  //Necessaire pour piloter les moteurs dans la loop, raison inconnue

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
  digitalWrite(13, led13_state);
  if(millis()-time > delai){
    time = millis();
    led13_state = !led13_state;
  }
  if(turn_and_go.run()==STOP){
    moveFinished = true;
  }

  //Ecoute du port serie
  if(Strat.available()>0){

  #ifdef debug
    Serial.println("MSG RECUE");
  #endif

    if(Strat.read()==stratFrame.getStarter()){
      //Trame detectée

      #ifdef debug
        Serial.println("TRAME RECUE");
      #endif

      while(Strat.available()==0); //Attente de la reception de la suite du message
      delay(20);
      readFrame(stratFrame); //Lecture de la trame si detectee

      #ifdef debug
        affichageFrame(stratFrame);
      #endif

      //Verification du checksum:
      if(calculateChecksum(stratFrame) == stratFrame.getChecksum()){
        //Action du robot en fonction de la trame decodee
        actionsFSM(stratFrame,ServoGauche,ServoDroite,Servo3,turn_and_go,moveAvailable,moveFinished, X, Y);
      } 
      else{
        //Rien pour le moment...
      }
    }
  }
  else{
    if(moveFinished and moveAvailable == 1){
      turn_and_go.goTo(X,Y);
      //Si on avait un deplacement en attente, on y va...
    }
  }

}

  
