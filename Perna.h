#include <Arduino.h>
#include <WProgram.h>
#include "fmath.h"

#ifndef PERNA_H
#define PERNA_H

class Perna {
private:
  static const int SERVO_MIN_PULSE_WIDTH = 544;
  static const int SERVO_MAX_PULSE_WIDTH = 2400;
  static const int SERVO_MIN_ANGLE_WIDTH = 0;
  static const int SERVO_MAX_ANGLE_WIDTH = 180;
  // servo constrains
  int servoMin[3];

  int servoMax[3];

  Servo servoPe;
  Servo servoMeio;

  int iniPe;
  int iniMeio;
  int iniCoxa;

  int pe;
  int meio;
  int coxa;

  int peMicro;
  int meioMicro;
  int coxaMicro;

public:

  Servo servoCoxa;

  boolean comandosPositivos;
  Perna();
  Perna(Servo servoP, Servo servoM, Servo servoC, int vp, int vm, int vc,
  boolean comandosP);

  void moveServosPosicaoInicial();

  void resetarPe();
  void resetarMeio();
  void resetarCoxa();

  bool movePe(int anguloNovo);
  bool moveMeio(int anguloNovo);
  bool moveCoxa(int anguloNovo);

  bool movePeMS(int ms);
  bool moveMeioMS(int ms);
  bool moveCoxaMS(int ms);

  void levantarMeio(int alturaAMais);
  void abaixarMeio(int alturaAMenos);

  void moverCoxaFrente(int anguloAMais);
  void moverCoxaTraz(int anguloAMenos);

  void fechaPe(int vlr);
  void abrePe(int vlr);

  static unsigned int degree2Micro(unsigned int degrees);
  static unsigned int micro2Degree(unsigned int micro);

  void getDelay();

  void setPe(int pe);
  void setMeio(int x);
  void setCoxa(int coxa);
  int getPe();
  int getMeio();
  int getCoxa();

  void setPeMicro(int pe);
  void setMeioMicro(int x);
  void setCoxaMicro(int coxa);
  int getPeMicro();
  int getMeioMicro();
  int getCoxaMicro();
  
  int getIniPeMicro();
  int getIniMeioMicro();
  int getIniCoxaMicro();

  void refreshPe();
  void refreshCoxa();
  void refreshMeio();
  /*float degree2Micro(float degrees){
   	 return 1000.0 + degrees * 50.0/9.0;
   	 }*/
  void updateLegAngulo(int p,int m,int c);
  void updateLegMicro(int p,int m,int c);

  bool setServoPosMS(Servo servo,int ms);
  bool setServoPos(Servo servo,int angulo);

  int calculaPosLimiteMS(int ms);
  int calculaPosLimite(int angulo);
};

Perna::Perna() {

  servoMax[0] = 2200;
  servoMax[1] = 1800;
  servoMax[2] = 2000;

  servoMin[0] = 800;
  servoMin[1] = 780;
  servoMin[2] = 1200;

  this->pe = 0;
  this->meio = 0;
  this->coxa = 0;
  this->peMicro = 0;
  this->meioMicro = 0;
  this->coxaMicro = 0;
  this->iniPe = 0;
  this->iniMeio = 0;
  this->iniCoxa = 0;
  this->comandosPositivos;
}

Perna::Perna(Servo servoP, Servo servoM, Servo servoC, int vp, int vm, int vc,
boolean comandosP) {
  //Serial.println("Instanciando perna ...");
  servoPe = servoP;
  servoMeio = servoM;
  servoCoxa = servoC;
  this->pe = vp;
  this->meio = vm;
  this->coxa = vc;
  this->peMicro = degree2Micro(vp);
  this->meioMicro = degree2Micro(vm);
  this->coxaMicro = degree2Micro(vc);
  this->iniPe = vp;
  this->iniMeio = vm;
  this->iniCoxa = vc;

  this->comandosPositivos = comandosP;
}
void Perna::moveServosPosicaoInicial() {
  /*this->resetarPe();
   	this->resetarMeio();
   	this->resetarCoxa();*/
  this->updateLegAngulo(this->iniPe,this->iniMeio,this->iniCoxa);
}

void Perna::resetarPe() {
  bool writeStatusOk = setServoPosMS(servoPe,degree2Micro(this->iniPe));
  this->getDelay();
}

void Perna::resetarMeio() {
  bool writeStatusOk = setServoPosMS(servoMeio,degree2Micro(this->iniMeio));
  this->getDelay();
}

void Perna::resetarCoxa() {
  bool writeStatusOk = setServoPosMS(servoCoxa,degree2Micro(this->iniCoxa));
  this->getDelay();
}

bool Perna::movePe(int anguloNovo) {
  bool writeStatusOk = setServoPosMS(servoPe,degree2Micro(anguloNovo));
  this->setPe(calculaPosLimite(anguloNovo));
  this->setPeMicro(calculaPosLimiteMS(degree2Micro(anguloNovo)));
  this->getDelay();
  return writeStatusOk;
}

bool Perna::moveMeio(int anguloNovo) {
  bool writeStatusOk = setServoPosMS(servoMeio,degree2Micro(anguloNovo));
  this->setMeio(calculaPosLimite(anguloNovo));
  this->setMeio(calculaPosLimiteMS(degree2Micro(anguloNovo)));
  this->getDelay();
  return writeStatusOk;
}

bool Perna::moveCoxa(int anguloNovo) {
  bool writeStatusOk = setServoPosMS(servoCoxa,degree2Micro(anguloNovo));
  this->setCoxa(calculaPosLimite(anguloNovo));
  this->setCoxa(calculaPosLimiteMS(degree2Micro(anguloNovo)));
  this->getDelay();
  return writeStatusOk;
}

bool Perna::movePeMS(int ms) {
  bool writeStatusOk = setServoPosMS(servoPe,ms);
  this->setPeMicro(calculaPosLimiteMS(ms));
  this->setPe(calculaPosLimite(micro2Degree(ms)));
  this->getDelay();
  return writeStatusOk;
}

bool Perna::moveMeioMS(int ms) {
  //Serial.print("moveMeioMS : ");
  bool writeStatusOk = setServoPosMS(servoMeio,ms);
  //Serial.println(calculaPosLimiteMS(ms),DEC);
  this->setMeioMicro(calculaPosLimiteMS(ms));
  this->setMeio(calculaPosLimite(micro2Degree(ms)));
  this->getDelay();
  return writeStatusOk;
}

bool Perna::moveCoxaMS(int ms) {
  bool writeStatusOk = setServoPosMS(servoCoxa,ms);
  this->setCoxaMicro(calculaPosLimiteMS(ms));
  this->setCoxa(calculaPosLimite(micro2Degree(ms)));
  this->getDelay();
  return writeStatusOk;
}

void Perna::levantarMeio(int alturaAMais) {
  int angulo;
  if (this->comandosPositivos) {
    angulo = (this->meio - alturaAMais);
  } 
  else {
    angulo = (this->meio + alturaAMais);
  }
  this->moveMeio(angulo);
}

void Perna::abaixarMeio(int alturaAMenos) {
  return levantarMeio(alturaAMenos * -1);
}

void Perna::abrePe(int vlr) {
  int angulo;
  if (this->comandosPositivos) {
    angulo = (this->pe - vlr);
  } 
  else {
    angulo = (this->pe + vlr);
  }
  this->movePe(angulo);
}

void Perna::fechaPe(int vlr) {
  return abrePe(vlr * -1);
}

void Perna::moverCoxaFrente(int anguloAMais) {
  int angulo;
  if (this->comandosPositivos) {
    angulo = (this->coxa - anguloAMais);
  } 
  else {
    angulo = (this->coxa + anguloAMais);
  }
  this->moveCoxa(angulo);
}

void Perna::moverCoxaTraz(int anguloAMenos) {
  this->moverCoxaFrente(anguloAMenos * -1);
}

unsigned int Perna::degree2Micro(unsigned int degrees) {
  return RadToMicroV2(DegToRad(degrees));
}

unsigned int Perna::micro2Degree(unsigned int micro) {
  return map(micro, SERVO_MIN_ANGLE_WIDTH, SERVO_MAX_ANGLE_WIDTH, SERVO_MIN_PULSE_WIDTH, SERVO_MAX_PULSE_WIDTH);
}

/*float degree2Micro(float degrees){
 return 1000.0 + degrees * 50.0/9.0;
 }*/

void Perna::updateLegAngulo(int p,int m,int c) {
  /*Serial.print("updateLegAngulo(");
  Serial.print(p,DEC);
  Serial.print(",");
  Serial.print(m,DEC);
  Serial.print(",");
  Serial.print(c,DEC);
  Serial.println(")");*/
  updateLegMicro(degree2Micro(p),degree2Micro(m),degree2Micro(c));
}

void Perna::updateLegMicro(int p,int m,int c) {

  bool mok = false;
  bool pok = false;
  bool cok = false;
  while (!mok&&!pok&&!cok){
    /*Serial.println("---------------------------------------");
    Serial.print("this->getCoxaMicro() : ");
    Serial.println(this->getCoxaMicro());
    Serial.print("this->getMeioMicro() : ");
    Serial.println(this->getMeioMicro());
    Serial.print("this->getPeMicro() : ");
    Serial.println(this->getPeMicro());
    Serial.println("---------------------------------------");*/
    if (!mok){
      if (this->getMeioMicro() != m){
        if (this->getMeioMicro() > m){
          this->setMeioMicro(this->getMeioMicro()-1);
        }else{
          this->setMeioMicro(this->getMeioMicro()+1);
        }
      }
      if (moveMeioMS(this->getMeioMicro())){
        if (this->getMeioMicro() == m){
          mok = true;
        }
      }else{
        mok = true;
      }
    }
    if (!pok){
      if (this->getPeMicro() != p){
        if (this->getPeMicro() > p){
          this->setPeMicro(this->getPeMicro()-1);
        }
        else{
          this->setPeMicro(this->getPeMicro()+1);
        }
      }
      if (movePeMS(this->getPeMicro())){
        if (this->getPeMicro() == p){
          pok = true;
        }
      }
      else{
        pok = true;
      }
    }
    if (!cok){
      if (this->getCoxaMicro() != c){
        if (this->getCoxaMicro() > c){
          this->setCoxaMicro(this->getCoxaMicro()-1);
        }
        else{
          this->setCoxaMicro(this->getCoxaMicro()+1);
        }
      }
      if (moveCoxaMS(this->getCoxaMicro())){
        if (this->getCoxaMicro() == c){
          cok = true;
        }
      }
      else{
        cok = true;
      }
    }
  }
}

void Perna::setPe(int pe) {
  this->pe = pe;
}

void Perna::setMeio(int x) {
  this->meio = x;
}

void Perna::setCoxa(int coxa) {
  this->coxa = coxa;
}

int Perna::getPe() {
  return this->pe;
}

int Perna::getMeio() {
  return this->meio;
}

int Perna::getCoxa() {
  return this->coxa;
}

void Perna::setPeMicro(int pe) {
  this->peMicro = pe;
}

void Perna::setMeioMicro(int x) {
  this->meioMicro = x;
}

void Perna::setCoxaMicro(int coxa) {
  this->coxaMicro = coxa;
}

int Perna::getPeMicro() {
  return this->peMicro;
}

int Perna::getMeioMicro() {
  return this->meioMicro;
}

int Perna::getCoxaMicro() {
  return this->coxaMicro;
}

int Perna::getIniPeMicro() {
  return degree2Micro(this->iniPe);
}

int Perna::getIniMeioMicro() {
  return degree2Micro(this->iniMeio);
}

int Perna::getIniCoxaMicro() {
  return degree2Micro(this->iniCoxa);
}

void Perna::getDelay() {
  
}

/*retorna false se atingiu algum max*/
bool Perna::setServoPosMS(Servo servo,int ms){
  servo.writeMicroseconds(calculaPosLimiteMS(ms));
  if (ms <= Perna::SERVO_MIN_PULSE_WIDTH || ms >= Perna::SERVO_MAX_PULSE_WIDTH){
    return false;
  }
  else{
    return true;
  }

}

/*retorna false se atingiu algum max*/
bool Perna::setServoPos(Servo servo,int angulo){
  servo.write(calculaPosLimite(angulo));
  if (angulo <= Perna::SERVO_MIN_ANGLE_WIDTH || angulo >= Perna::SERVO_MAX_ANGLE_WIDTH){
    return false;
  }
  else{
    return true;
  }
}

int Perna::calculaPosLimiteMS(int ms){
  if (ms > Perna::SERVO_MIN_PULSE_WIDTH && ms < SERVO_MAX_PULSE_WIDTH){
    return ms;
  }
  else if (ms < Perna::SERVO_MIN_PULSE_WIDTH ){
    return Perna::SERVO_MIN_PULSE_WIDTH;
  }
  else if (ms > Perna::SERVO_MAX_PULSE_WIDTH){
    return Perna::SERVO_MAX_PULSE_WIDTH;
  }
}

int Perna::calculaPosLimite(int angulo){
  if (angulo > SERVO_MIN_ANGLE_WIDTH && angulo < SERVO_MAX_ANGLE_WIDTH){
    return angulo;
  }
  else if (angulo < SERVO_MIN_ANGLE_WIDTH ){
    return SERVO_MIN_ANGLE_WIDTH;
  }
  else if (angulo > SERVO_MAX_ANGLE_WIDTH){
    return SERVO_MAX_ANGLE_WIDTH;
  }
}
#endif

