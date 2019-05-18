#include "SerialCommunication.h"
#include "Arduino.h"

//Construtor
SerialCommunication::SerialCommunication(int baud){
  this->_baud = baud;
}

//Setup
void SerialCommunication::begin(){
  Serial.begin(this->_baud);
}

//Ler mensagem da serial
bool SerialCommunication::read(char msg[]){

  //Se está recebendo algo da serial
  if(Serial.available()){
    unsigned long starttime=millis();

    //Fica no loop enquanto o número de bytes recebidos for menor que MSG_SIZE
    //e enquanto o tempo dentro do loop for menor que 1 seg
    while(Serial.available() < MSG_SIZE && millis() - starttime < 1000){}

    //Se o tamanho da mensagem recebida for maior ou igual a MSG_SIZE
    if(Serial.available() >= MSG_SIZE){

      //Armazena a mansagem
       for(int n=0; n<MSG_SIZE; n++) msg[n] = Serial.read();

      //Ajusta o vetor
      this->ajustArray(msg);

       return true;
    }
    else{

      //Erro

      //Zera o buffer da serial
      Serial.flush();

      return false;
    }
  }
  else {
    return false;
  }
}

//Ajusta a mensagem recebida
void SerialCommunication::ajustArray(char msg[]){
  char aux;

  for(int i = 0; i < MSG_SIZE; i++){
    if(msg[0] == START )
      break;

    if(msg[i] == START){
      aux = msg[0];
      for(int k=0; k<MSG_SIZE-1; k++){
        msg[k] = msg[k+1];
      }
      msg[MSG_SIZE-1] = aux;
      i = 0;
    }
  }
    
}
