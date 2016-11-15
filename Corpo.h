#include <ArduinoRobotMotorBoard.h>
#include <EasyTransfer2.h>
#include <LineFollow.h>
#include <Multiplexer.h>

#include <Arduino.h>
#include <WProgram.h>
#include <Servo.h>
#include "Perna.h"
#include "fmath.h"

// Define Leg Numbers
#define DF 0 // Direita Frontal
#define DM 1 // Direita Meio
#define DA 2 // Direita Atraz
#define EA 3 // Esquerda Atraz
#define EM 4 // Esquerda Meio
#define EF 5 // Esquerda Frontal

#define DFP 34 // Direita Frontal pe
#define DFM 32 // Direita Frontal meio
#define DFC 30 // Direita Frontal coxa

#define DMP 33 // Direita Meio pe
#define DMM 26 // Direita Meio meio
#define DMC 24 // Direita Meio coxa

#define DRP 53 // Direita Atraz pe
#define DRM 51 // Direita Atraz meio
#define DRC 45 // Direita Atraz coxa

#define EFP 40 // Esquerda Frontal pe
#define EFM 38 // Esquerda Frontal meio
#define EFC 36 // Esquerda Frontal coxa

#define EMP 46 // Esquerda Meio pe
#define EMM 44 // Esquerda Meio meio
#define EMC 42 // Esquerda Meio coxa

#define ERP 52 // Esquerda Atraz pe
#define ERM 50 // Esquerda Atraz meio
#define ERC 48 // Esquerda Atraz coxa

class Corpo {
private:
  static const int CoxaLength  = 10;          //Length of the Coxa [mm]
  static const int FemurLength = 70;          //Length of the Femur [mm]
  static const int TibiaLength = 65;          //Lenght of the Tibia [mm]

  static const int INDICE_MOV_PE  = 1;          //Length of the Coxa [mm]
  static const int INDICE_MOV_MEIO = 2;          //Length of the Femur [mm]
  static const int INDICE_MOV_COXA = 3;          //Lenght of the Tibia [mm]

  Servo legServo[18];

  Perna *direitaFrontal;
  Perna *direitaMeio;
  Perna *direitaPosterior;
  Perna *esquerdaFrontal;
  Perna *esquerdaMeio;
  Perna *esquerdaPosterior;

  int delayAcao;
  int movimentoCoxa;
  int alturaMeioLevantada;
  int alturaMeio;
  int passoMovimentoPe;
  int tamanhoPasso;

  void init();

  void fechaPe(Perna *perna);

  void abrePe(Perna *perna);

  void levantarMeioCorpo(Perna *perna);

  void abaixarMeioCorpo(Perna *perna);

  void levantarMeio(Perna *perna);

  void abaixarMeio(Perna *perna);

  void resetarMeio(Perna *perna);

  void resetarCoxa(Perna *perna);

  void moverCoxaFrente(Perna *perna);

  void moverCoxaTraz(Perna *perna);

  void attachServo(Servo servo,int servoPort);

  void moverFaseDoPassoMS(Perna *pF,Perna *pMInv,Perna *pP,int indiceF,int indiceM,int indiceP,bool invF,bool invM,bool invP,int angulacao);


public: 

  Corpo();

  void attachServos();

  void resetaCorpo();

  void andarParaFrente();

  void andarParaTraz();

  void setPosicaoPasso(int step);

  void passoLevanta1(int ms);

  void passoMoveFrente1(int ms);

  void passoMoveTraz1(int ms);

  void passoAbaixa1(int ms);
  
  void passoLevanta2(int ms);

  void passoMoveFrente2(int ms);

  void passoMoveTraz2(int ms);

  void passoAbaixa2(int ms);

  void movimentaY(bool frente);
  
  void giraEixo(bool direita);

};

Corpo::Corpo(){    
  this->init();
}

void Corpo::init(){
  delayAcao = 100;
  alturaMeioLevantada = 600;//Posicao inicial + altura
  movimentoCoxa = 250;//Posicao inicial + altura
  alturaMeio = 5;//Posicao inicial + altura
  passoMovimentoPe = 5;//Posicao inicial + altura
  //tamanhoPasso = alturaMeioLevantada*2;//Posicao inicial + altura
  /*direitaFrontal=    new Perna(legServo[0],legServo[1],legServo[2],110,75,35,true);  // Direita Frontal;
   direitaMeio=       new Perna(legServo[3],legServo[4],legServo[5],100,110,70,true);   // Direita Meio ;
   direitaPosterior=  new Perna(legServo[6],legServo[7],legServo[8],120,60,145,true);   // Direita Atraz ;
   
   esquerdaFrontal=   new Perna(legServo[9],legServo[10],legServo[11],85,100,145,false);// Esquerda Frontal
   esquerdaMeio=      new Perna(legServo[12],legServo[13],legServo[14],110,75,89,false);   // Esquerda Meio;
   esquerdaPosterior= new Perna(legServo[15],legServo[16],legServo[17],100,80,35,false);   // Esquerda Atraz;*/
  //PMC
  direitaFrontal= new Perna(legServo[0],legServo[1],legServo[2],90,90,95,true);  // Direita Frontal
  direitaMeio= new Perna(legServo[3],legServo[4],legServo[5],90,85,85,true);   // Direita Meio 
  direitaPosterior= new Perna(legServo[6],legServo[7],legServo[8],90,90,90,true);   // Direita Atraz 
  esquerdaFrontal= new Perna(legServo[9],legServo[10],legServo[11],103,79,90,false);   // Esquerda Frontal
  esquerdaMeio= new Perna(legServo[12],legServo[13],legServo[14],125,90,98,false);   // Esquerda Meio
  esquerdaPosterior= new Perna(legServo[15],legServo[16],legServo[17],90,100,107,false);   // Esquerda Atraz

  /*direitaFrontal=    new Perna(legServo[0],legServo[1],legServo[2],80,125,55,true);  // Direita Frontal
   direitaMeio=  new  Perna(legServo[3],legServo[4],legServo[5],90,105,100,true);   // Direita Meio 
   direitaPosterior=  new  Perna(legServo[6],legServo[7],legServo[8],90,90,110,true);   // Direita Atraz 
   esquerdaFrontal=  new  Perna(legServo[9],legServo[10],legServo[11],80,100,145,false);   // Esquerda Frontal
   esquerdaMeio=  new  Perna(legServo[12],legServo[13],legServo[14],90,90,100,false);   // Esquerda Meio
   esquerdaPosterior=   new Perna(legServo[15],legServo[16],legServo[17],90,90,35,false);   // Esquerda Atraz*/

  /*direitaFrontal= new Perna(legServo[0],legServo[1],legServo[2],90,90,90,true);  // Direita Frontal
   direitaMeio= new Perna(legServo[3],legServo[4],legServo[5],90,90,90,true);   // Direita Meio 
   direitaPosterior= new Perna(legServo[6],legServo[7],legServo[8],90,90,90,true);   // Direita Atraz 
   esquerdaFrontal= new Perna(legServo[9],legServo[10],legServo[11],90,90,90,false);   // Esquerda Frontal
   esquerdaMeio= new Perna(legServo[12],legServo[13],legServo[14],90,90,90,false);   // Esquerda Meio
   esquerdaPosterior= new Perna(legServo[15],legServo[16],legServo[17],90,90,90,false);   // Esquerda Atraz*/

  this->resetaCorpo();
  this->attachServos();
  this->resetaCorpo();
}

void Corpo::resetaCorpo(){
  this->direitaFrontal->moveServosPosicaoInicial();
  this->direitaMeio->moveServosPosicaoInicial();
  this->direitaPosterior->moveServosPosicaoInicial();
  this->esquerdaFrontal->moveServosPosicaoInicial();
  this->esquerdaMeio->moveServosPosicaoInicial();
  this->esquerdaPosterior->moveServosPosicaoInicial();

}

void Corpo::andarParaFrente(){
  movimentaY(true);

}

void Corpo::andarParaTraz(){
  movimentaY(false);
}

void Corpo::setPosicaoPasso(int step){
  if (step == 0){

  }
}

void Corpo::attachServos(){
  attachServo(legServo[0],DFP);// Direita Frontal pe
  attachServo(legServo[1],DFM);// Direita Frontal meio
  attachServo(legServo[2],DFC);// Direita Frontal coxa

  attachServo(legServo[3],DMP);// Direita Meio pe
  attachServo(legServo[4],DMM);// Direita Meio meio
  attachServo(legServo[5],DMC);// Direita Meio coxa

  attachServo(legServo[6],DRP);// Direita Atraz pe
  attachServo(legServo[7],DRM);// Direita Atraz meio
  attachServo(legServo[8],DRC);// Direita Atraz coxa

  attachServo(legServo[9],EFP);// Esquerda Frontal pe
  attachServo(legServo[10],EFM);// Esquerda Frontal meio
  attachServo(legServo[11],EFC);// Esquerda Frontal coxa

  attachServo(legServo[12],EMP);// Esquerda Meio pe
  attachServo(legServo[13],EMM);// Esquerda Meio meio
  attachServo(legServo[14],EMC);// Esquerda Meio coxa

  attachServo(legServo[15],ERP);// Esquerda Atraz pe
  attachServo(legServo[16],ERM);// Esquerda Atraz meio
  attachServo(legServo[17],ERC);// Esquerda Atraz coxa
}

void Corpo::attachServo(Servo servo,int servoPort){
  servo.attach(servoPort);
  delay(20);
}
void Corpo::fechaPe(Perna *perna){
  perna->fechaPe(passoMovimentoPe);
}

void Corpo::abrePe(Perna *perna){
  perna->abrePe(passoMovimentoPe);
}

void Corpo::levantarMeioCorpo(Perna *perna){
  perna->levantarMeio(alturaMeio);
}

void Corpo::abaixarMeioCorpo(Perna *perna){
  perna->abaixarMeio(alturaMeio);
}

void Corpo::levantarMeio(Perna *perna){
  perna->levantarMeio(alturaMeioLevantada);
}

void Corpo::abaixarMeio(Perna *perna){
  perna->abaixarMeio(alturaMeioLevantada);
}

void Corpo::resetarMeio(Perna *perna){
  perna->resetarMeio();
}

void Corpo::resetarCoxa(Perna *perna){
  perna->resetarCoxa();
}

void Corpo::moverCoxaFrente(Perna *perna){
  perna->moverCoxaFrente(tamanhoPasso);
}

void Corpo::moverCoxaTraz(Perna *perna){
  perna->moverCoxaTraz(tamanhoPasso);
}

// Servo Frontal 1 , Medial e Posterior1 , Indice é qual a parte que vai mexer , 1=Pe 2=Meio 3=Coxa
// Os inv sao para falar se a angulacao vai ser positiva ou negativa
void Corpo::moverFaseDoPassoMS(Perna *pF,Perna *pMInv,Perna *pP,int indiceF,int indiceM,int indiceP,bool invF,bool invM,bool invP,int angulacaoMS){
  bool fok = false;
  bool pok = false;
  bool mok = false;
  int angP;
  if (indiceP == Corpo::INDICE_MOV_PE){
    angP = invP?pP->getPeMicro()-angulacaoMS:pP->getPeMicro()+angulacaoMS;
  }
  else if (indiceP == Corpo::INDICE_MOV_MEIO){
    angP = invP?pP->getMeioMicro()-angulacaoMS:pP->getMeioMicro()+angulacaoMS;
  }
  else if (indiceP == Corpo::INDICE_MOV_COXA){
    angP = invP?pP->getCoxaMicro()-angulacaoMS:pP->getCoxaMicro()+angulacaoMS;
  }
  int angF;
  if (indiceF == Corpo::INDICE_MOV_PE){
    angF = invF?pF->getPeMicro()-angulacaoMS:pF->getPeMicro()+angulacaoMS;
  }
  else if (indiceP == Corpo::INDICE_MOV_MEIO){
    angF = invF?pF->getMeioMicro()-angulacaoMS:pF->getMeioMicro()+angulacaoMS;
  }
  else if (indiceP == Corpo::INDICE_MOV_COXA){
    angF = invF?pF->getCoxaMicro()-angulacaoMS:pF->getCoxaMicro()+angulacaoMS;
  }
  int angM;
  if (indiceM == Corpo::INDICE_MOV_PE){
    angM = invM?pMInv->getPeMicro()-angulacaoMS:pMInv->getPeMicro()+angulacaoMS;
  }
  else if (indiceP == Corpo::INDICE_MOV_MEIO){
    angM = invM?pMInv->getMeioMicro()-angulacaoMS:pMInv->getMeioMicro()+angulacaoMS;
  }
  else if (indiceP == Corpo::INDICE_MOV_COXA){
    angM = invM?pMInv->getCoxaMicro()-angulacaoMS:pMInv->getCoxaMicro()+angulacaoMS;
  }
  while (!mok&&!pok&&!fok){
    if (!pok){
      int ang=angP;
      int pm = 0;
      if (indiceP == Corpo::INDICE_MOV_PE){
        pm = pP->getPeMicro();
      }
      else if (indiceP == Corpo::INDICE_MOV_MEIO){
        pm = pP->getMeioMicro();
      }
      else if (indiceP == Corpo::INDICE_MOV_COXA){
        pm = pP->getCoxaMicro();
      }
      if (pm != ang){
        if (pm > ang){
          pm--;
        }
        else{
          pm++;
        }
        if (indiceP == Corpo::INDICE_MOV_PE){
          pP->setPeMicro(pm);
        }
        else if (indiceP == Corpo::INDICE_MOV_MEIO){
          pP->setMeioMicro(pm);
        }
        else if (indiceP == Corpo::INDICE_MOV_COXA){
          pP->setCoxaMicro(pm);
        }
      }
      bool retMove = false;
      if (indiceP == Corpo::INDICE_MOV_PE){
        retMove = pP->movePeMS(pm);
      }
      else if (indiceP == Corpo::INDICE_MOV_MEIO){
        retMove = pP->moveMeioMS(pm);
      }
      else if (indiceP == Corpo::INDICE_MOV_COXA){
        retMove = pP->moveCoxaMS(pm);
      }
      if (retMove){
        if (pm == ang){
          pok = true;
        }
      }
      else{
        pok = true;
      }
    }
    if (!mok){
      int ang = angM;
      int mm = 0;
      if (indiceM == Corpo::INDICE_MOV_PE){
        mm = pMInv->getPeMicro();
      }
      else if (indiceM == Corpo::INDICE_MOV_MEIO){
        mm = pMInv->getMeioMicro();
      }
      else if (indiceM == Corpo::INDICE_MOV_COXA){
        mm = pMInv->getCoxaMicro();
      }
      if (mm != ang){
        if (mm > ang){
          mm--;
        }
        else{
          mm++;
        }
        if (indiceM == Corpo::INDICE_MOV_PE){
          pMInv->setPeMicro(mm);
        }
        else if (indiceM == Corpo::INDICE_MOV_MEIO){
          pMInv->setMeioMicro(mm);
        }
        else if (indiceM == Corpo::INDICE_MOV_COXA){
          pMInv->setCoxaMicro(mm);
        }
      }
      bool retMove = false;
      if (indiceM == Corpo::INDICE_MOV_PE){
        retMove = pMInv->movePeMS(mm);
      }
      else if (indiceM == Corpo::INDICE_MOV_MEIO){
        retMove = pMInv->moveMeioMS(mm);
      }
      else if (indiceM == Corpo::INDICE_MOV_COXA){
        retMove = pMInv->moveCoxaMS(mm);
      }
      if (retMove){
        if (mm == ang){
          mok = true;
        }
      }
      else{
        mok = true;
      }
    }

    if (!fok){
      int ang = angF;
      int fm = 0;
      if (indiceF == Corpo::INDICE_MOV_PE){
        fm = pF->getPeMicro();
      }
      else if (indiceF == Corpo::INDICE_MOV_MEIO){
        fm = pF->getMeioMicro();
      }
      else if (indiceF == Corpo::INDICE_MOV_COXA){
        fm = pF->getCoxaMicro();
      }
      if (fm != ang){
        if (fm > ang){
          fm--;
        }
        else{
          fm++;
        }
        if (indiceF == Corpo::INDICE_MOV_PE){
          pF->setPeMicro(fm);
        }
        else if (indiceF == Corpo::INDICE_MOV_MEIO){
          pF->setMeioMicro(fm);
        }
        else if (indiceF == Corpo::INDICE_MOV_COXA){
          pF->setCoxaMicro(fm);
        }
      }
      bool retMove = false;
      if (indiceF == Corpo::INDICE_MOV_PE){
        retMove = pF->movePeMS(fm);
      }
      else if (indiceF == Corpo::INDICE_MOV_MEIO){
        retMove = pF->moveMeioMS(fm);
      }
      else if (indiceF == Corpo::INDICE_MOV_COXA){
        retMove = pF->moveCoxaMS(fm);
      }
      if (retMove){
        if (fm == ang){
          fok = true;
        }
      }
      else{
        fok = true;
      }
    }

  }
}  

void Corpo::passoLevanta1(int ms){
  //Levanta 1
    moverFaseDoPassoMS(
    this->direitaFrontal,this->esquerdaMeio,this->direitaPosterior,
    INDICE_MOV_MEIO,INDICE_MOV_MEIO,INDICE_MOV_MEIO,
    true,false,true,
    ms);
    
}

void Corpo::passoMoveFrente1(int ms){
  //Move para frente 1
    moverFaseDoPassoMS(
    this->direitaFrontal,this->esquerdaMeio,this->direitaPosterior,
    INDICE_MOV_COXA,INDICE_MOV_COXA,INDICE_MOV_COXA,
    true,false,true,
    ms);
}

void Corpo::passoMoveTraz1(int ms){
  //Move para traz 1
    moverFaseDoPassoMS(
    this->direitaFrontal,this->esquerdaMeio,this->direitaPosterior,
    INDICE_MOV_COXA,INDICE_MOV_COXA,INDICE_MOV_COXA,
    false,true,false,
    ms);
}

void Corpo::passoAbaixa1(int ms){
  //Abaixa 1
    moverFaseDoPassoMS(
    this->direitaFrontal,this->esquerdaMeio,this->direitaPosterior,
    INDICE_MOV_MEIO,INDICE_MOV_MEIO,INDICE_MOV_MEIO,
    false,true,false,
    ms);
}

void Corpo::passoLevanta2(int ms){
  //Levanta 2
    moverFaseDoPassoMS(
    this->esquerdaFrontal,this->direitaMeio,this->esquerdaPosterior,
    INDICE_MOV_MEIO,INDICE_MOV_MEIO,INDICE_MOV_MEIO,
    false,true,false,
    ms);
    
}

void Corpo::passoMoveFrente2(int ms){
  //Move para frente 2
    moverFaseDoPassoMS(
    this->esquerdaFrontal,this->direitaMeio,this->esquerdaPosterior,
    INDICE_MOV_COXA,INDICE_MOV_COXA,INDICE_MOV_COXA,
    false,true,false,
    ms);
}

void Corpo::passoMoveTraz2(int ms){
  //Move para traz 2
    moverFaseDoPassoMS(
    this->esquerdaFrontal,this->direitaMeio,this->esquerdaPosterior,
    INDICE_MOV_COXA,INDICE_MOV_COXA,INDICE_MOV_COXA,
    true,false,true,
    ms);
}

void Corpo::passoAbaixa2(int ms){
  //Abaixa 2
    moverFaseDoPassoMS(
    this->esquerdaFrontal,this->direitaMeio,this->esquerdaPosterior,
    INDICE_MOV_MEIO,INDICE_MOV_MEIO,INDICE_MOV_MEIO,
    true,false,true,
    ms);
}

void Corpo::movimentaY(bool frente){
  // Fazer um sistema de passos, cada passo entra em um for que seta os 3 servos .. de milisegundos em milisegundos 
  // ate atingir o ponto ideal
  // Exemplo de andar : http://www.kickstarter.com/projects/arcbotics/hexy-the-hexapod-low-cost-six-legged-open-robot

  if (frente){
    // Parte 1
    passoLevanta1(alturaMeioLevantada);
    passoMoveTraz2(movimentoCoxa);
    passoMoveFrente1(movimentoCoxa);
    passoAbaixa1(alturaMeioLevantada);
    //delay(1000);
    // Parte 2
    passoLevanta2(alturaMeioLevantada);
    passoMoveTraz1(movimentoCoxa);
    passoMoveFrente2(movimentoCoxa);
    passoAbaixa2(alturaMeioLevantada);
    //delay(1000);
  }else{
    
    passoLevanta1(alturaMeioLevantada);
    passoMoveTraz1(movimentoCoxa);
    passoMoveFrente2(movimentoCoxa);    
    passoAbaixa1(alturaMeioLevantada);
    
    // Parte 1
    passoLevanta2(alturaMeioLevantada);
    passoMoveFrente1(movimentoCoxa);
    passoMoveTraz2(movimentoCoxa);
    passoAbaixa2(alturaMeioLevantada);
    //delay(1000);
    // Parte 2
    /*passoLevanta1(alturaMeioLevantada);
    passoMoveFrente2(movimentoCoxa);
    passoMoveTraz1(movimentoCoxa);
    passoAbaixa1(alturaMeioLevantada);*/
    //delay(1000);
  }
} 


void Corpo::giraEixo(bool esquerda){
  if (esquerda){
    
    passoLevanta2(alturaMeioLevantada);
    passoMoveTraz2(movimentoCoxa/2);
    passoAbaixa2(alturaMeioLevantada);
    
    passoLevanta1(alturaMeioLevantada);
    passoMoveFrente2(movimentoCoxa/2);
    passoAbaixa1(alturaMeioLevantada);
    

    
  }else{
    
    passoLevanta1(alturaMeioLevantada);
    delay(1000);
    passoMoveTraz1(movimentoCoxa/2);
    passoAbaixa1(alturaMeioLevantada);
    passoLevanta2(alturaMeioLevantada);
    passoMoveFrente1(movimentoCoxa/2);
    passoAbaixa2(alturaMeioLevantada);
    
  }
}



