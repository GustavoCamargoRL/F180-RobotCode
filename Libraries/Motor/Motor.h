#ifndef motores_h  				// Guarda de inclusão (evita problemas de compilação caso o usuário inclua a biblioteca mais de uma vez)
#define motores_h

// Inclusão de bibliotecas necessárias
#include "Hall.h"
#include "PID_v1.h"

#define ANTI_HORARIO 0x7F					// 127 em hexa
#define MOTOR1_KP 0.0169674600619376 
#define MOTOR1_KI 0.345553536364746 
#define MOTOR1_KD 3.65844139495533e-05 
#define MOTOR2_KP 0.0148149151695593 
#define MOTOR2_KI 0.740745758477963  
#define MOTOR2_KD 7.40745758477962e-05 

// Pinos do microcontrolador que estão sendo utilizados para as rodas 1, 2 e 3
const uint8_t motores_pins[3][3] = {
  {PB_4, PF_4, PC_7},     						// Velocidade roda 1,2 e 3 respectivamente {PB_5, PC_6, PC_7}  {PB_4, PF_4, PB_6} **** motor 3 erro radio
  {PE_5, PD_7, PA_4},     						// Sentido da rotação do Motor 1,2 e 3 respectivamente (ROBÔ 1: Antigo - PB0, Novo - PA7) {PB_0, PA_4, PA_3} {PE_5, PD_7, PA_4}
  {PB_5, PD_6, PB_7}      						// Sensor Hall Motor 1,2 e 3 respectivamente {PB_2, PE_0, PF_0} {PB_5, PD_6, PB_7}
};

// Pinos Tacômetro Motores 1, 2 e 3
//const uint8_t tacometro_pins[3] = {
//	PE_3, PE_2, PB_4
//};
//const uint8_t tacometro1 PE_3 // Motor 1
//const uint8_t tacometro2 PE_2 // Motor 2
//const uint8_t tacometro3 PB_4 // Motor 3

class Motor
{
	public:
	  	// Construtores 
	  	Motor(int); 							// Recebe o id do Motor e inicializa o resto com valores default
	  
	  	// Métodos Implementados 
	  	void configurar();						// Realiza as inicializações dos pinos como entrada ou saída
	  	void andar(char []);					// Recebe o protocolo de comunicação e realiza a ação de andar
	  
	  	// Métodos ainda não Implementados 
	  	void andarPID(double, double);			// Recebe um setpoint e realiza a ação de andar com o controle PID da velocidade dos motores
	  	double PWMtoSpeed(byte);				// Recebe o valor de pwm pretendido e transforma num setpoint da velocidade do Motor

	public:
	  	// Atributos
	  	int _id;												// id = 1, 2 ou 3 (o id do Motor deve ser igual ao da roda que ele está acoplado) 
	  	double _kp, _ki, _kd, _akp, _aki, _akd;					// kp = constante proporcional; ki = constante integrativa; kd = constante derivativa (constantes do PID)
	  	double _setpoint, _input, _output, _erro;				// setpoint = velocidade pretendida do Motor; input = velocidade atual do Motor; output = valor do pwm de saída (PID)
	  	uint8_t _velocidade, _sentido, _hall, _tacometro;		// Pinos para saída do pwm da roda (velocidade), para o sentido da rotação do Motor e para a leitura da velocidade atual do Motor (hall)
	  	boolean _status;										// Se o controlador e o sensor já foram atualizados (status = 1) ou não (status = 0)	
	  	//Hall(int pin, int qtdPulsos); 	
	  	PID *motorPID;         // Inicializa o controlador PID do motor
};
#endif
