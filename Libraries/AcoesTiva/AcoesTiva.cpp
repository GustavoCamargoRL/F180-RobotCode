/************************************************************************************************************************
 * Biblioteca a ser utilizada na TIVA TM4C123 para o controle das ações dos robôs da equipe Maracatronics - Versão 1.5
 *
 * por Natália Souza Soares e Victor Lyra - <maracatronics@gmail.com> 
*************************************************************************************************************************/
#include "AcoesTiva.h"
//#include "wiring_analog.c"

// Construtores para a Tiva 
AcoesTiva::AcoesTiva(){                   
  this->_infrared = infrared;            
  this->_drible = drible;  
  this->_resetMSP = resetMSP;
  this->_adcBateria = adcBateria;              
}

// Construtores para a Tiva
AcoesTiva::AcoesTiva(uint8_t i, uint8_t dri, uint8_t reset, uint8_t adcBat){
  this->_infrared = i;
  this->_drible = dri;
  this->_resetMSP = reset;
  this->_adcBateria = adcBat;
}

void AcoesTiva::configurarTiva(){
  pinMode(this->_resetMSP, OUTPUT);
  digitalWrite(this->_resetMSP, HIGH);                   // Reset na MSP quando for 0

  Serial.begin(9600);
  Serial4.begin(9600);

  pinMode(this->_infrared,INPUT);
  pinMode(this->_drible,OUTPUT);
  pinMode(this->_adcBateria, INPUT);
}

void AcoesTiva::enviarComando(char protocolo[], boolean radioParou){          // Protocolo = {'M', byteAção+ID, byteVelocMotor1, byteVelocMotor2, byteVelocMotor3, 1}
  char protocolo2[MSG_SIZE];                              // Cria um vetor de tam 3 para ser enviado para MSP
  protocolo2[0] = protocolo[0];                           // O primeiro elemento enviado será o byte de start do protocolo de comunicação ('M')
  if(!radioParou){
    if(protocolo[1] & CHUTE_BIT)                            // Verifica se é para chutar - AND bit a bit entre o byte de ações+id do protocolo recebido e o bit do chute
      protocolo2[1] = 'C';                                  // O segundo elemento enviado será que é para chutar
    else if(protocolo[1] & PASSE_BIT)                       // Verifica se é para passar - AND bit a bit entre o byte de ações+id do protocolo recebido e o bit do passe
      protocolo2[1] = 'P';                                  // O segundo elemento enviado será que é para passar
    else                                                    // Caso nenhum dos casos dê certo, não é para ele fazer nada
      protocolo2[1] = 'N';                                  // O segundo elemento enviado será que é para fazer nada

    if(this->possedeBola())                                 // Verifica se o robô está com posse de bola
      protocolo2[2] = '1';                                  // O último elemento enviado será que o robô está com a bola
    else                                                    // Caso contrário
      protocolo2[2] = '0';                                  // O último elemento enviado será que o robô não está com a bola

  }
  else{
    protocolo2[1] = 'S';
    protocolo2[2] = '1';
  }
  
  Serial4.write(protocolo2);
}

void AcoesTiva::driblar(char protocolo[]){            // Protocolo = {'M', byteAção+ID, byteVelocMotor1, byteVelocMotor2, byteVelocMotor3, 1}
  if(protocolo[1] & DRIBLE_BIT)                       // Verifica se é para driblar - AND bit a bit entre o byte de ações+id do protocolo recebido e o bit do drible
    digitalWrite(this->_drible,HIGH);                 // Realiza o drible
  else                                                // Caso contrário
    digitalWrite(this->_drible,LOW);                  // Não dribla
}

boolean AcoesTiva::possedeBola(){
  if(digitalRead(this->_infrared) == HIGH)                // Se o sensor infrared detectar a bola
    return true;                                          // O robô está com a pose de bola
  return false;                                           // Caso contrário, ele não está
}

void AcoesTiva::nivelBat(){                                     // Faz a medição da tensão da bateria
  int valor_bateria;                                         
  valor_bateria = analogRead(this->_adcBateria);                // Faz a medição do ADC
  this->_carga_bateria = (char) (valor_bateria * DEN_BATERIA);  // Valor em Volts da bateria
}

void AcoesTiva::horus(char sendFrame[], char id){
    
    sendFrame[0] = 'H';
    sendFrame[1] = (char) id & 0x07;                          // ID & 0b00000111
    sendFrame[2] = (char) 85; //10 * this->_carga_bateria;          // Para pegar a carga da bateria (valor int)
    sendFrame[3] = (char) 10; //this->_carga_capacitor;             // Carga do capacitor
    sendFrame[4] = (char) 0; //digitalRead(this->_infrared);              // Posse de bola
    sendFrame[5] = (char) 1; //digitalRead(this->_drible);                // Status do drible
}

void AcoesTiva::receberComando(){
  byte aux[2];
  static boolean resetou = true; 
  static unsigned long int tempo = millis();
  static unsigned long int tempo2 = millis();
  static boolean mspAlive = false;
  if(Serial4.available()){
    mspAlive = true;
    if(Serial4.available() >= 2){
      for(int n = 0; n < 2; n++){       
           aux[n] = Serial4.read();                              // Lê o conteúdo da serial até o tamanho da mensagem esperada
       }
       if (aux[0] == 'M'){
         this->_carga_capacitor = aux[1];
         tempo = millis();
         //Serial.println(aux[0]);
         Serial.println(this->_carga_capacitor);
       }
    }
  }
  else{
    if (millis() - tempo >= 1000 && mspAlive){
      digitalWrite(this->_resetMSP, LOW);
      if(resetou){
        tempo2 = millis();
        resetou = false;
      }

      if(millis() - tempo2 >= 2){
        digitalWrite(this->_resetMSP, HIGH);
        resetou = true;
        mspAlive = false;
        tempo = millis();
      }
    }
  }
}
