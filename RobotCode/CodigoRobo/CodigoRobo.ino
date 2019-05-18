#include <Radio.h>
#include "wiring_analog.c"    // Necessário para o PWMWrite
#include <AcoesTiva2.h>
#include <Motor.h>
#include <Hall.h>

#define ID_ROBO 0x03

//DECLARAÇÃO DAS FUNÇÕES
void parar();
bool isChecksumOk(char);
void contagemPulsos1();
void contagemPulsos2();
void contagemPulsos3();

//DECLARAÇÃO DAS VARIÁVEIS GLOBAIS
int qntPulsosTotal[3] = {0};
unsigned long tempoRadioParado = millis(), tempoPulso1 = 0, tempoPulso2 = 0, tempoPulso3 = 0;
boolean radioParou = true, interruptRoda1 = true, interruptRoda2 = true, interruptRoda3 = true;
Motor *robo[3];
Hall *hallMotores[3];
Radio radio(109, "RX");
AcoesTiva2 tiva;

//************************************************************CONFIGURAÇÕES*****************************************************************************
void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 3; i++) {
    robo[i] = new Motor(i + 1);
    robo[i]->configurar();
    hallMotores[i] = new Hall(robo[i]->_hall, 24);
  }

  attachInterrupt(digitalPinToInterrupt(robo[0]->_hall), contagemPulsos1 , CHANGE);
  attachInterrupt(digitalPinToInterrupt(robo[1]->_hall), contagemPulsos2 , CHANGE);
  attachInterrupt(digitalPinToInterrupt(robo[2]->_hall), contagemPulsos3 , CHANGE);

  tiva.configurarTiva();
  radio.setup();

  parar();
}

//************************************************************FUÇÃO PRINCIPAL*****************************************************************************
void loop() {
  char msg[7];
  char sendFrame[6];
  static unsigned int tempoHorus, tempoAtual, tempoPulso;
  static boolean flagHorus = false, inicio = true;
  static double setpoint_speed =1300;
  double setpoint_speedR1 = 0;

  int velocidade1 = 100, velocidade2 = 100, velocidade3 = 100;
  int DRIBLE = 16, CHUTE = 64, PASSE = 32;
  
//  msg[0] = (byte) 'M';   
//  msg[1] = (byte) (ID_ROBO + DRIBLE + CHUTE);
//  msg[2] = (byte) velocidade1;
//  msg[3] = (byte) velocidade2;  
//  msg[4] = (byte) velocidade3;
//  msg[5] = (byte) 'F';
//  msg[6] = (byte) (msg[0] ^ msg[1] ^ msg[2] ^ msg[3] ^ msg[4] ^ msg[5]);

  if (radio.receive(msg, 7) && isChecksumOk(msg) && ((msg[0] == 'M') && ((msg[1] & 0x07) == ID_ROBO))) {
  //if (msg[0] == 'M' && (msg[1] & 0x07) == ID_ROBO) {
    tempoAtual = millis();
    if (inicio) {
      tempoRadioParado = tempoAtual;
      tempoPulso1 = tempoAtual;
      tempoPulso2 = tempoAtual;
      tempoPulso3 = tempoAtual;
      for (int k = 0; k < 3; k++) {
        qntPulsosTotal[k] = 0;
        hallMotores[k]->iniciar(tempoAtual);
      }
      inicio = false;
    }
    else {      
      for(int k = 0; k < 3; k++){
        hallMotores[k]->atualizaTempo(tempoAtual);
        if(hallMotores[k]->_tempoContagem >= INTERVALO_MAXIMO){
          qntPulsosTotal[k] = 0;
        }
      }
    }

    for (int j = 0; j < 3; j++) {
      hallMotores[j]->calcularVelocidade(qntPulsosTotal[j]);
      setpoint_speedR1 = hallMotores[1]->_rpm;
      if(j==0){
        robo[j]->andarPID(setpoint_speedR1, hallMotores[j]->_rpm);
        robo[j]->andar(msg);
        PWMWrite(robo[j]->_velocidade, 127, robo[j]->_output, 1000); 
      } else if(j == 1){
        robo[j]->andarPID(setpoint_speed, hallMotores[j]->_rpm);
        robo[j]->andar(msg);
        PWMWrite(robo[j]->_velocidade, 127, robo[j]->_output, 1000);          // PWMWrite(pin, resolution, duty, frequency);
      }
    }
    

    //PRINTS PARA A DOCUMENTAÇÃO DO PID   
//    if(msg[3] == 1 || hallMotores[0]->_rpm != 0 || hallMotores[0]->_rpm != 0 || hallMotores[0]->_rpm != 0){
//      Serial.print((int) msg[3]);
//      for(int l = 0; l < 3; l++){
//      Serial.print("    ");
//      if(l == 2)
//        Serial.println(hallMotores[l]->_rpm);
//      else
//        Serial.print(hallMotores[l]->_rpm);
//      } 
//    }


    tiva.driblar(msg);
    if (tiva.carregarCapacitor(msg, false))
      PWMWrite(tiva._chutePWM, 255, DUTY, 3000);       //PWMWrite(pin, resolution, duty, frequency);
    else
      PWMWrite(tiva._chutePWM, 255, 0, 3000);       //PWMWrite(pin, resolution, duty, frequency);
    tiva.chutar(msg, true);

    tempoRadioParado = tempoAtual;
    radioParou = false;

    //HORUS
    /* if(msg[1] & 0x80 && !flagHorus){
       tempoHorus = millis();
       flagHorus = true;
      }
      if(flagHorus && millis() - tempoHorus >= ID_ROBO * 100){
       PWMWrite(tiva._chutePWM, 255, 0, 3000);       //PWMWrite(pin, resolution, duty, frequency);
       tiva.horus(sendFrame, msg[1]);
       radio.sendHorus(sendFrame);
       flagHorus = false;
      }*/
  }
  else {
    delay(1);
    if (millis() - tempoRadioParado >= 500)
      parar();
  }
}



//************************************************************FUÇÕES AUXILIARES*****************************************************************************
void parar() {
  char protocol[6] = {'M', ID_ROBO, 1, 1, 1, 1};
  for (int j = 0; j < 3; j++) {
    robo[j]->andar(protocol);
    PWMWrite(robo[j]->_velocidade, 127, robo[j]->_output, 1000);          // PWMWrite(pin, resolution, duty, frequency);
  }
  if (millis() - tempoRadioParado >= 3000) {
    PWMWrite(tiva._chutePWM, 255, 0, 3000);       //PWMWrite(pin, resolution, duty, frequency);
    tiva.chutar(protocol, radioParou);
    radioParou = true;
  }
  tiva.driblar(protocol);
}

bool isChecksumOk(char msg[]) {
  return (msg[6] == (msg[0] ^ msg[1] ^ msg[2] ^ msg[3] ^ msg[4] ^ msg[5]));
}

void contagemPulsos1() {
  if (interruptRoda1) {
    tempoPulso1 = micros();
    interruptRoda1 = false;
  } else {
    tempoPulso1 = micros() - tempoPulso1;
    if (tempoPulso1 >= INTERVALO_MINIMO){
      qntPulsosTotal[0]++;
      interruptRoda1 = true;
    } else {
       interruptRoda1 = true;
    }
  }
}

void contagemPulsos2() {
  if (interruptRoda2) {
    tempoPulso2 = micros();
    interruptRoda2 = false;
  } else {
    tempoPulso2 = micros() - tempoPulso2;
    if (tempoPulso2 >= INTERVALO_MINIMO){
      qntPulsosTotal[1]++;
      interruptRoda2 = true;
    } else {
       interruptRoda2 = true;
    }
  }
}

void contagemPulsos3() {
  if (interruptRoda3) {
    tempoPulso3 = micros();
    interruptRoda3 = false;
  } else {
    tempoPulso3 = micros() - tempoPulso3;
    if (tempoPulso3 >= INTERVALO_MINIMO){
      qntPulsosTotal[2]++;
      interruptRoda3 = true;
    } else {
       interruptRoda3 = true;
    }
  }
}
