#include <Radio.h>
#include "wiring_analog.c"   // Necessário para o PWMWrite
#include <AcoesTiva2.h>
#include <Motor.h>
#include <Hall.h>
#include <FastCRC.h>

#define ID_ROBO 0x03
#define SIZE_MSG 7

//PC_4 - RX
//PC_5 - TX

bool crc_check(char msg[SIZE_MSG]);
void parar();
void contagemPulsos1();
void contagemPulsos2();
void contagemPulsos3();

FastCRC16 CRC16;
Motor *robo[3];
Hall *hallMotores[3];
Radio radio(109, "RX"); 
AcoesTiva2 tiva;

int qntPulsos[3] = {48, 48, 48};
boolean radioParou = true;

unsigned int tempoRadioParado = millis();
boolean flagTempo = true;
int qntPulsosTotal1 = 0, qntPulsosTotal2 = 0, qntPulsosTotal3 = 0;
unsigned int tempoTotal, tempoInicial, tempoPulso;

void setup() {  
  Serial.begin(38400);
  for(int i = 0; i < 3; i++){
    robo[i] = new Motor(i+1);
    robo[i]->configurar();
    hallMotores[i] = new Hall(robo[i]->_hall, qntPulsos[i]);
  }
  
  attachInterrupt(digitalPinToInterrupt(robo[0]->_hall), contagemPulsos1 , FALLING);
  attachInterrupt(digitalPinToInterrupt(robo[1]->_hall), contagemPulsos2 , FALLING);
  attachInterrupt(digitalPinToInterrupt(robo[2]->_hall), contagemPulsos3 , FALLING);

  tiva.configurarTiva();  
  
  radio.setup();

  parar();
}

void loop() {
  char msg[SIZE_MSG];

  if(radio.receive(msg, SIZE_MSG) && crc_check(msg) && ((msg[0] == 'M') && ((msg[1] & 0x07) == ID_ROBO))){
      //Serial.println("recebeu");
      if(flagTempo){
        tempoInicial = millis();
        qntPulsosTotal1 = 0, qntPulsosTotal2 = 0, qntPulsosTotal3 = 0;
        flagTempo = false;
      }
      else{
        tempoTotal = millis() - tempoInicial;
      }
      for(int j = 0; j < 3; j++){
         robo[j]->andar(msg);
         PWMWrite(robo[j]->_velocidade, 127, robo[j]->_output, 1000);          // PWMWrite(pin, resolution, duty, frequency);
      }
  
      Serial.print("tempo: ");
      Serial.print(tempoTotal);    
      Serial.print("  pul1: ");
      Serial.print(qntPulsosTotal1);
      Serial.print("  pul2: ");
      Serial.print(qntPulsosTotal2);
      Serial.print("  pul3: ");
      Serial.println(qntPulsosTotal3);

      
    
      tiva.driblar(msg);
      
      if(tiva.carregarCapacitor(msg, false)){
       PWMWrite(tiva._chutePWM, 255, DUTY, 3000);       //PWMWrite(pin, resolution, duty, frequency);
      }
      else{
        PWMWrite(tiva._chutePWM, 255, 0, 3000);       //PWMWrite(pin, resolution, duty, frequency);
      } 
      tiva.chutar(msg, true);
      tempoRadioParado = millis();
      radioParou = false;
    
  }
  else{
    delay(1);
    if(millis() - tempoRadioParado >= 500)
      parar();  
  }  
}

// Checks if CRC is OK
bool crc_check(char msg[SIZE_MSG]){
  uint8_t msg_crc[SIZE_MSG];
  for(int i=0; i < 7; i++){
    msg_crc[i] = msg[i];
  }

  return (CRC16.xmodem(msg_crc, sizeof(msg_crc)) == 0);
}

void contagemPulsos1(){
  qntPulsosTotal1++;
}

void contagemPulsos2(){
  qntPulsosTotal2++;
}

void contagemPulsos3(){
  qntPulsosTotal3++;
}

void parar(){
  char protocol[6] = {'M', ID_ROBO, 1, 1, 1, 1};
  for(int j = 0; j < 3; j++){
    robo[j]->andar(protocol);
    PWMWrite(robo[j]->_velocidade, 127, robo[j]->_output, 1000);          // PWMWrite(pin, resolution, duty, frequency);
  }
  if(millis() - tempoRadioParado >= 3000){
    PWMWrite(tiva._chutePWM, 255, 0, 3000);       //PWMWrite(pin, resolution, duty, frequency);
    tiva.chutar(protocol, radioParou);
    radioParou = true;
  }
  tiva.driblar(protocol);
}
