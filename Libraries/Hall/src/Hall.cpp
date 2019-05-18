#include "Hall.h"

Hall::Hall(int pin, int qtdPulsos){
  this->_pin = pin;
  this->_qtdPulsos = qtdPulsos;
  this->_interruptFlag = true;
  this->_tempoPulso = 0;
  this->_rpm = 0;
}

void Hall::iniciar(unsigned long tempoAtual){
  _tempoContagem = tempoAtual;
  _tempoPulso = tempoAtual;
  _tempoInicial = tempoAtual;
}

void Hall::atualizaTempo(unsigned long tempoAtual){
  _tempoContagem = tempoAtual - _tempoInicial;
  if (_tempoContagem >= INTERVALO_MAXIMO)
    _tempoInicial = tempoAtual;
}

void Hall::calcularVelocidade(int contador){
  /*switch(_estado){                                //Compara o estado de leitura do sensor 
      case 0:
        if(digitalRead(_pin)==HIGH) _estado=1;       // e conta a quantidade de variações (pulsos)                    
      break;
      case 1:
        if(digitalRead(_pin)==LOW){
          _estado=0;
  
          _contador++;                              //Variável que recebe essa contagem
          Serial.println(_contador);
        }
      break;
    }
                                                              //|----------------------------------------------------------------------------|
    if(millis()-_t1>=INTERVALO){                               //| Para cada intervalo de 100 milissegundos                                   |
                                                              //|(por definição intervalo = 100) vai facilitar para calcular os ciclos       |
      _val=(((double)_contador/_qtdPulsos)/INTERVALO)*1000*60;   //| usa a quantidade de pulsos para calcular a quantidade de ciclos por minuto |
                                                              //|____________________________________________________________________________|
      _contador=0;

  
      _t1=millis();                                            // reinicia a contagem de tempo
    }*/

  _rpm = (((double)contador/_qtdPulsos)/_tempoContagem)*1000*60;  // essa é a variável de retorno da função em forma de ciclos por minuto                                             
}
