#ifndef AcoesTiva2_h					   // Guarda de inclusão (evita problemas de compilação caso o usuário inclua a biblioteca mais de uma vez)
#define AcoesTiva2_h

#include "Arduino.h"

#define CHUTE_BIT 0x40                                  // Valor, em hexa, do bit de chute
#define PASSE_BIT 0x20                                  // Valor, em hexa, do bit de passe
#define DRIBLE_BIT 0x10                                 // Valor, em hexa, do bit de drible
#define DEN_BATERIA 0.00349206446208139819721378494034  //Define-se o inverso do denominador da regra de 3 do chute para evitar divisões no código visto que divisões em MCUS são lentas => den_chute=(200*3.3)/(4095*div tensao)
#define DUTY (int)(256 * 0.75)                          // 75% do máximo
#define DEN_CHUTE 0.08                                  // Define-se o inverso do denominador da regra de 3 do chute para evitar divisões no código visto que divisões em MCUS são lentas => den_chute=tensão maxima/resolução ADC

const uint8_t drible = PC_6;           // Controle do drible  (placa antiga PE_5/ nova PC_6)
const uint8_t infrared = PE_4;         // Leitura do led infravermelho - verificar posse de bola (placa antiga PA_6/ nova PE_4)
const uint8_t adcBateria = PE_2;       // Canal A0 do ADC (placa antiga PB_1/ PE_2)
const uint8_t adcChute = PE_3;         // Canal A0 do ADC(placa antiga PE_4/ PE_3)
const uint8_t chutePWM = PC_4;         // PWM do chute (placa antiga PC_4/ PF_1) ******* erro led vermelho
const uint8_t disparo = PA_7;          // Comando de chute (placa antiga PC_5/ PA_7)

class AcoesTiva2{
public:
  // Construtores
  AcoesTiva2();                                                                    // Inicialização dos atributos com os valores default
  AcoesTiva2(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);                // Inicialização dos pinos do infrared e do controle do drible manualmente

  // Métodos
  int possedeBola();			               // Verifica se o robô está ou não com a posse de bola
  void configurarTiva();			               // Realiza a inicialização dos pinos como entrada ou saída e da serial 
  void driblar(char []);			               // Comando para o drible
  void nivelBat();
  void horus(char [], char);
  boolean carregarCapacitor(char [], boolean);               // Realiza o carregamento do capacitor de acordo com o modo ('C' - chute, 'P' - passe, 'N' - nada)
  void chutar(char [], boolean);                          // Comando para o chute ou passe

public:
  // Atributos
  uint8_t _infrared, _drible, _adcBateria;		           // Pinos do LED infravermelho, do controle do drible e leitura do nível da bateria
  uint8_t _chutePWM, _disparo, _adcChute;                // Pinos de controle para o chute/passe
  boolean _capacitorCarregado, _estaChutando;            // Verifica se o capacitor está ou não carregado
  char _carga_bateria, _acao;
  int _carga_capacitor;
};
#endif
