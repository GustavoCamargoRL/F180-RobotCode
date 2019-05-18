#ifndef AcoesMSP_h                    // Guarda de inclusão (evita problemas de compilação caso o usuário inclua a biblioteca mais de uma vez)
#define AcoesMSP_h

#include "Arduino.h"


#define MSG_SIZE 3
#define START 0x4D                                         // Valor, em hexa, para 'M'
#define DRIBLE_BIT 0x10                                    // Valor, em hexa, do bit de drible
#define CHUTE_BIT 0x40                                     // Valor, em hexa, do bit de chute
#define PASSE_BIT 0x20                                     // Valor, em hexa, do bit de passe
#define CHUTAR 67                                          // Valor, inteiro, do bit de chutar
#define PASSAR 80                                          // Valor, inteiro, do bit de passe
#define DUTY (int)(256 * 0.75)                             // 75% do máximo
#define DEN_CHUTE 0.25                                     // Define-se o inverso do denominador da regra de 3 do chute para evitar divisões no código visto que divisões em MCUS são lentas => den_chute=tensão maxima/resolução ADC
#define DEN_BATERIA 0.00349206446208139819721378494034     // Mesmo motivo do chute        


const uint8_t chute_PWM = P2_1;        // PWM do chute 
const uint8_t disparo = P1_5;          // Comando de chute
const uint8_t adc_chute = P1_4;        // Canal A0 do ADC


class AcoesMSP{
public:
  // Construtores
  AcoesMSP();                             // Inicialização dos atributos com os valores default
  AcoesMSP(uint8_t, uint8_t,uint8_t);     // Inicialização dos pinos de controle de chute/passe manualmente

  // Métodos
  void carregarCapacitor();               // Realiza o carregamento do capacitor de acordo com o modo ('C' - chute, 'P' - passe, 'N' - nada)
  void receberComando();                  // Recebe o comando de ação da tiva (chutar, passar ou nada)
  void ajustarArray();                    // Ajusta o recebimento do protocolo (garante que o protocolo seja recebido do jeito certo)
  void configurarMSP();                   // Realiza a inicialização dos pinos como entrada ou saída e da serial
  void chutar();                          // Comando para o chute ou passe
  void enviarInfo();                      // Envia a informação do capacitor para a tiva

public:
  // Atributos 
  char _mensagemRecebida[3];                  // Protocolo de comunicação recebido da TIVA (byte de start, ação, posse de bola)
  boolean _capacitorCarregado, _estaChutando, _descarregouCap;                // Verifica se o capacitor está ou não carregado
  uint8_t _chutePWM, _disparo, _adcChute;     // Pinos de controle para o chute/passe
  double _carga_capacitor;                    // Carga do capacitor

};
#endif
