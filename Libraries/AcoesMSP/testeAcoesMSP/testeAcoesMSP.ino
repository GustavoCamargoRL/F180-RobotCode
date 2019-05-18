#include <AcoesMSP.h>

AcoesMSP msp;
//P1_1 -> RX
//P1_2 -> TX

void setup() {
  msp.configurarMSP();  
}

void loop() {
  msp.receberComando();
   
  Serial.print(msp._mensagemRecebida[0]);
  Serial.print(" ");
  Serial.print(msp._mensagemRecebida[1]);
  Serial.print(" ");
  Serial.println(msp._mensagemRecebida[2]);
}
