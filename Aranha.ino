#include <Servo.h>
#include "Corpo.h"
#include <stdlib.h> 
#include "fmath.h"


// ================================================================
// ================================================================

int incomingByte = 0;

Corpo *corpo;

void setup ()
{
  Serial.begin (9600);
  // ================  Servos  ==================
  delay(100);   
  corpo = new Corpo();
  delay(2000);	

}

void loop (){
  if (Serial.available() > 0) {
    // read the incoming byte:
    int leu  = Serial.parseInt();
    //incomingByte =  Serial.parseInt();
    if (leu!=0){
      incomingByte = leu;
    }
    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
    /*if(incomingByte == 1){
     corpo->andarParaFrente(); 
    }else if(incomingByte == 2){
       corpo->andarParaTraz();
    }*/
  }
  if(incomingByte == 1){
     corpo->andarParaFrente(); 
  }else if(incomingByte == 2){
     corpo->andarParaTraz();
  }else if(incomingByte == 3){
     corpo->giraEixo(true);
  }else if(incomingByte == 4){
     corpo->giraEixo(false);
  }
}



int8_t satoi( char *str, int len ){  
  int8_t r = 0;  
  for(int i=0; i<len; i++){  
    //Check if this is a number  
    if ( str[i] < 0x3a && str[i] > 0x2f){  
      // is a ASCII number, return it  
      r = r * 10;  
      r += (str[i]-0x30);  
    }
    else{  
      i = len; //exit!  
      r = -1;  
      break;  
    }    
  }  
  return r;   
}  
int8_t satoi( char *str ){  
  int8_t r = 0;  
  int len = strlen(str);  
  for(int i=0; i<len; i++){  
    //Check if this is a number  
    if ( str[i] < 0x3a && str[i] > 0x2f){  
      // is a ASCII number, return it  
      r = r * 10;  
      r += (str[i]-0x30);  
    }
    else{  
      i = len; //exit!  
      r = -1;  
      break;  
    }    
  }  
  return r;   
}  

String split(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {
    0, -1          };
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
      found++;
      strIndex[0] = strIndex[1]+1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String leStringSerial(){
  String ret = "";
  char character;
  while(Serial.available()>0) {
    character = Serial.read();
    ret.concat(character);
  }
  return ret;
}

