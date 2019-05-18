#include <AcoesTiva.h>

AcoesTiva tiva;
//PD_6 -> RX
//PD_7 -> TX 
   
void setup() {
  tiva.configurarTiva();
}

void loop() {
  char protocolo[6] = {'M',64,100,100,100,'F'};
  tiva.enviarComando(protocolo); 
}
