#ifndef AcoesTiva_h					   // Guarda de inclusão (evita problemas de compilação caso o usuário inclua a biblioteca mais de uma vez)
#define AcoesTiva_h

#include "Arduino.h"

#define MSG_SIZE 3                                      // Tamanho da mensagem enviada da MSP
#define MSG_SIZE2 2                                     // Tamanho da mensagem recebida da MSP
#define CHUTE_BIT 0x40                                  // Valor, em hexa, do bit de chute
#define PASSE_BIT 0x20                                  // Valor, em hexa, do bit de passe
#define DRIBLE_BIT 0x10                                 // Valor, em hexa, do bit de drible
#define DEN_BATERIA 0.00349206446208139819721378494034  //Define-se o inverso do denominador da regra de 3 do chute para evitar divisões no código visto que divisões em MCUS são lentas => den_chute=(200*3.3)/(4095*div tensao)

const uint8_t infrared = PE_4;         // Leitura do led infravermelho - verificar posse de bola
const uint8_t drible = PE_5;           // Controle do drible  
const uint8_t resetMSP = PA_5;         // Pino de reset da MSP
const uint8_t adcBateria = PB_1;       // Canal A0 do ADC

class AcoesTiva{
public:
  // Construtores
  AcoesTiva();                                         // Inicialização dos atributos com os valores default
  AcoesTiva(uint8_t, uint8_t, uint8_t, uint8_t);       // Inicialização dos pinos do infrared e do controle do drible manualmente

  // Métodos
  boolean possedeBola();			               // Verifica se o robô está ou não com a posse de bola
  void enviarComando(char [], boolean);		   // Envia o comando de chutar, passar ou nada para a MSP
  void configurarTiva();			               // Realiza a inicialização dos pinos como entrada ou saída e da serial 
  void driblar(char []);			               // Comando para o drible
  void nivelBat();
  void horus(char [], char);
  void receberComando();

private:
  // Atributos
  uint8_t _infrared, _drible, _resetMSP, _adcBateria;		     // Pinos do LED infravermelho e do controle do drible
  uint8_t _chutePWM, _disparo, _adcChute;                    // Pinos de controle para o chute/passe
  boolean _capacitorCarregado;                               // Verifica se o capacitor está ou não carregado
  char _carga_bateria;
  int _carga_capacitor;
};
#endif
