/***************************************************************************************************************************
 * Biblioteca a ser utilizada na MSP430G2 para o controle do chute e passe dos robôs da equipe Maracatronics - Versão 1.0
 *
 * por Natália Souza Soares e Victor Lyra - <maracatronics@gmail.com> 
****************************************************************************************************************************/
#include "AcoesMSP.h"
#include "wiring_analog.c" 


// Construtoras 
AcoesMSP::AcoesMSP(){               
  this->_chutePWM = chute_PWM;               
  this->_disparo = disparo;                 
  this->_adcChute = adc_chute;             
  this->_capacitorCarregado = false; 
  this->_estaChutando = false;   
  this->_descarregouCap = true;
}

AcoesMSP::AcoesMSP(uint8_t c, uint8_t dis, uint8_t adcChut){
  this->_chutePWM = c;
  this->_disparo = dis;
  this->_adcChute = adcChut;
  this->_capacitorCarregado = false;
  this->_estaChutando = false;   
  this->_descarregouCap = true;
}

// Métodos
void AcoesMSP::configurarMSP(){
  pinMode(this->_chutePWM, OUTPUT);                          
  pinMode(this->_disparo, OUTPUT);
  pinMode(this->_adcChute, INPUT);
  pinMode(P1_6, OUTPUT);                                   // Pino LED indicador da MSP ligada ou não
    
  digitalWrite(this->_disparo, LOW);
  digitalWrite(this->_chutePWM, LOW);                      // Impedir a corrente no transistor -novo
  delay(500);
  digitalWrite(P1_6, HIGH);

  Serial.begin(9600);
  analogFrequency(3000);                                   // Define a frequência do analogWrite como 3000
}

void AcoesMSP::carregarCapacitor(){ 
  static unsigned long tempoDelay = millis();              
  int valor_capacitor, tensao_capacitor;
  valor_capacitor = analogRead(this->_adcChute);            // Valor atual da tensão no capacitor em unidades do ADC
  this->_carga_capacitor = (DEN_CHUTE * valor_capacitor);   // Valor atual da carga do capacitor em volts
  if(this->_mensagemRecebida[1] == 'C')
    tensao_capacitor = 100;                                 // Valor pretendido para a carga do capacitor (CHUTE)
  else
    tensao_capacitor = 100;                                 // Valor pretendido para a carga do capacitor (DEFAULT)
  if(!this->_estaChutando && this->_mensagemRecebida[1] != 'S'){
    if(this->_carga_capacitor < tensao_capacitor){
      if(millis() - tempoDelay >= 10){
        analogWrite(this->_chutePWM, DUTY);                   // similar ao PWMWrite(pin, resolution, duty, frequency) -> frequency definida no configurarMSP
        this->_descarregouCap = false;
        tempoDelay = millis();
      }
      this->_capacitorCarregado = false;
    }
    else{
      this->_capacitorCarregado = true;                       // Capacitor já está carregado
      digitalWrite(this->_chutePWM, LOW);                     // Entrada do transistor é barrada  -novo
    }
  }   
  else  
    digitalWrite(this->_chutePWM, LOW);
}

void AcoesMSP::receberComando(){
  byte aux[3];
  if(Serial.available()){
    unsigned long starttime = millis();

    while(Serial.available() < MSG_SIZE && millis() - starttime < 500){}          // Espera um certo tempo para sincronizar

    if(Serial.available() >= MSG_SIZE){                                           // Se o conteúdo da serial tiver pelo menos o mesmo tamanho da mensagem esperada
       for(int n = 0; n < MSG_SIZE; n++){       
           aux[n] = Serial.read();                                                // Lê o conteúdo da serial até o tamanho da mensagem esperada
       }
       if(aux[0] == 'M' && (aux[1] == 'C' || aux[1] == 'P' || aux[1] == 'N' || aux[1] == 'S') && (aux[2] == '1' || aux[2] == '0')){
        this->_mensagemRecebida[0] = aux[0];
        this->_mensagemRecebida[1] = aux[1];
        this->_mensagemRecebida[2] = aux[2];
       }       
    }
  }
}

void AcoesMSP::chutar(){
  static unsigned long tempoDelay = millis();
  if((this->_capacitorCarregado && this->_mensagemRecebida[2] == '1' && this->_mensagemRecebida[1] != 'N') || (this->_mensagemRecebida[1] == 'S' && !this->_descarregouCap)){  // Se o comando for chute ou passe 
    digitalWrite(this->_disparo, HIGH);           // Realiza o disparo
    this->_capacitorCarregado = false;            // Capacitor ficou descarregado
    this->_descarregouCap = true;
    tempoDelay = millis();
  }
  else{                                           // Caso contrário, realiza tudo o oposto 
    if(millis() - tempoDelay >= 20) {
      digitalWrite(this->_disparo, LOW);                                      
      this->_estaChutando = false;
    }
    else{
      this->_estaChutando = true;
    }
  }
}

void AcoesMSP::enviarInfo(){
  int valor_capacitor = (int) (this->_carga_capacitor);
  char mensagem_enviar[2];
  mensagem_enviar[0] = 'M';
  mensagem_enviar[1] = (char) (valor_capacitor);
  Serial.write(mensagem_enviar);
}
