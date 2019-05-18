#define INTERVALO 100       // intervalo definido para a contagem de milissegundos

#ifndef Hall_h
#define Hall_h

#include "Arduino.h"

#define INTERVALO_MINIMO 700
#define INTERVALO_MAXIMO 500

class Hall{
  private: 
		int _pin, _qtdPulsos;                         
	    boolean _interruptFlag;
	    

  public:
	  Hall(int pin, int qtdPulsos);
	  void iniciar(unsigned long);
	  void atualizaTempo(unsigned long);
	  void calcularVelocidade(int);
	  int _rpm;
	  unsigned long _tempoPulso, _tempoContagem, _tempoInicial;
};

#endif