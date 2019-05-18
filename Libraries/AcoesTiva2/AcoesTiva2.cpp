/************************************************************************************************************************
 * Biblioteca a ser utilizada na TIVA TM4C123 para o controle das ações dos robôs da equipe Maracatronics - Versão 2.0
 *
 * por Natália Souza Soares e Victor Lyra - <maracatronics@gmail.com> 
*************************************************************************************************************************/
#include "AcoesTiva2.h"
//#include "wiring_analog.c"

// Construtores para a Tiva 
AcoesTiva2::AcoesTiva2(){                   
  this->_infrared = infrared;            
  this->_drible = drible;  
  this->_adcBateria = adcBateria; 
  this->_chutePWM = chutePWM;
  this->_disparo = disparo;
  this->_adcChute = adcChute; 
  this->_capacitorCarregado = false;
  this->_estaChutando = false;            
}

// Construtores para a Tiva
AcoesTiva2::AcoesTiva2(uint8_t pinInfrared, uint8_t pinDrible, uint8_t pinAdcBateria, uint8_t pinChutePWM, uint8_t pinDisparo, uint8_t pinAdcChute){
  this->_infrared = pinInfrared;
  this->_drible = pinDrible;
  this->_adcBateria = pinAdcBateria;
  this->_chutePWM = pinChutePWM;
  this->_disparo = pinDisparo;
  this->_adcChute = pinAdcChute;
  this->_capacitorCarregado = false;
  this->_estaChutando = false;   
}

void AcoesTiva2::configurarTiva(){
 // Serial.begin(38400);

  pinMode(this->_infrared,INPUT);
  pinMode(this->_drible, OUTPUT);
  pinMode(this->_adcBateria, INPUT);
  pinMode(this->_chutePWM, OUTPUT);                          
  pinMode(this->_disparo, OUTPUT);
  pinMode(this->_adcChute, INPUT);

  digitalWrite(this->_disparo, LOW);
  digitalWrite(this->_chutePWM, LOW);                      // Impedir a corrente no transistor -novo
}

void AcoesTiva2::driblar(char protocolo[]){            // Protocolo = {'M', byteAção+ID, byteVelocMotor1, byteVelocMotor2, byteVelocMotor3, 1}
  if(protocolo[1] & DRIBLE_BIT)                       // Verifica se é para driblar - AND bit a bit entre o byte de ações+id do protocolo recebido e o bit do drible
    digitalWrite(this->_drible,HIGH);                 // Realiza o drible
  else                                                // Caso contrário
    digitalWrite(this->_drible,LOW);                  // Não dribla
}

int AcoesTiva2::possedeBola(){
  if(digitalRead(this->_infrared) == LOW)              // Se infrared for LOW, detecta posse de bola
    return  1;                                          // O robô está com a pose de bola
  return 0;                                             // Caso contrário, ele não está
}

void AcoesTiva2::nivelBat(){                                     // Faz a medição da tensão da bateria
  int valor_bateria;                                         
  valor_bateria = analogRead(this->_adcBateria);                // Faz a medição do ADC
  this->_carga_bateria = (char) (valor_bateria * DEN_BATERIA);  // Valor em Volts da bateria
}

void AcoesTiva2::horus(char sendFrame[], char id){
    
    sendFrame[0] = 'H';
    sendFrame[1] = (char) id & 0x07;                          // ID & 0b00000111
    sendFrame[2] = (char) 119; //10 * this->_carga_bateria;          // Para pegar a carga da bateria (valor int)
    sendFrame[3] = (char) 132; //this->_carga_capacitor;             // Carga do capacitor
    sendFrame[4] = (char) 1;//this->possedeBola();                // Posse de bola
    sendFrame[5] = (char) 1;//digitalRead(this->_drible);                // Status do drible
}

boolean AcoesTiva2::carregarCapacitor(char protocolo[], boolean radioParou){ 
  static unsigned long tempoDelay = millis();              
  int valor_capacitor, tensao_capacitor;

  valor_capacitor = analogRead(this->_adcChute);            // Valor atual da tensão no capacitor em unidades do ADC
  this->_carga_capacitor = (DEN_CHUTE * valor_capacitor);   // Valor atual da carga do capacitor em volts
  //Serial.println(this->_carga_capacitor);

  if(protocolo[1] & CHUTE_BIT) {                          // Verifica se é para chutar - AND bit a bit entre o byte de ações+id do protocolo recebido e o bit do chute
    tensao_capacitor = 130;                               // Valor pretendido para a carga do capacitor (CHUTE)
  }                           
  else if(protocolo[1] & PASSE_BIT){                      // Verifica se é para passar - AND bit a bit entre o byte de ações+id do protocolo recebido e o bit do passe
    tensao_capacitor = 130;                               // Valor pretendido para a carga do capacitor (DEFAULT)
  }  

  if(!this->_estaChutando && !radioParou){
    if(this->_carga_capacitor < tensao_capacitor){
      this->_capacitorCarregado = false;
      if(millis() - tempoDelay >= 10){
        tempoDelay = millis();
        return true;
        //?PWMWrite(this->_chutePWM, 255, DUTY, 3000);       //PWMWrite(pin, resolution, duty, frequency)
      }
    }
    else{
      this->_capacitorCarregado = true;                   // Capacitor já está carregado
    }
  }   
  
  return false;
}

void AcoesTiva2::chutar(char protocolo[], boolean radioParou){
  static unsigned long tempoDelay = millis();

  if((this->_capacitorCarregado && this->possedeBola() && ((protocolo[1] & CHUTE_BIT) || (protocolo[1] & PASSE_BIT))) || !(radioParou)){  // Se estiver carregado, com posse de bola e o comando for chute ou passe
    digitalWrite(this->_disparo, HIGH);           // Realiza o disparo
    this->_capacitorCarregado = false;            // Capacitor ficou descarregado
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





    
  
