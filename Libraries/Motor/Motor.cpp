/*********************************************************************************************************************************
 * Biblioteca a ser utilizada na TIVA TM4C123 para o controle dos motoreses maxons dos robôs da equipe Maracatronics - Versão 1.0
 *
 * por Natália Souza Soares e Victor Lyra - <maracatronics@gmail.com> 
**********************************************************************************************************************************/
#include "Motor.h"
//#include "wiring_analog.c"    // Necessário para o PWMWrite

// Construtores 
Motor::Motor(int id){ // Básico
  this->_id = id;
  this->_velocidade = motores_pins[0][id-1];
  this->_sentido = motores_pins[1][id-1];
  this->_hall = motores_pins[2][id-1];
  this->_kp = 1;       
  this->_ki = 0;
  this->_kd = 0;
  this->_input = 0;
  this->_setpoint = 0;
  this->_output = 0;
  this->_erro = 0;
  this->_status = false;
  //this->_tacometro = tacometro_pins[id - 1];
  this->motorPID = new PID(&(_input), &(_output), &(_setpoint), _kp, _ki, _kd, 0);         // Inicializa o controlador PID do motor
}

// Métodos 
void Motor::configurar(){                          
  pinMode(_velocidade,OUTPUT);                  
  pinMode(_sentido,OUTPUT);
  pinMode(_hall,INPUT);
  //pinMode(_tacometro, INPUT);
  //Serial.begin(9600);
 
  if(_id == 1){
    _kp = MOTOR1_KP;
    _ki = MOTOR1_KI;
    _kd = MOTOR1_KD;
  }
  else if(_id == 2){
    _kp = MOTOR2_KP;
    _ki = MOTOR2_KI;
    _kd = MOTOR2_KD;
  }
  motorPID->SetMode(AUTOMATIC);
  motorPID->SetSampleTime(20);
  motorPID->SetOutputLimits(0, 255);
  motorPID->SetTunings(_kp, _ki, _kd);
}

double Motor::PWMtoSpeed(byte setpoint_pwm){
  // TODO
}

void Motor::andarPID(double setpoint_speed, double current_speed){
  //Serial.println("anda BB");
  // TODO
  if(_erro == _setpoint || current_speed != 0){
    _input = current_speed;
  }  
  _setpoint = setpoint_speed;


  /*_erro = abs(_setpoint - _input); //distance away from setpoint
  if (_erro <= 200)
    motorPID->SetTunings(_kp, _ki, _kd);
  else
   motorPID->SetTunings(_akp, _aki, _akd); */

  motorPID->Compute();  
}

void Motor::andar(char protocolo[]){                              // Protocolo = {'M', byteAção+ID, byteVelocMotor1, byteVelocMotor2, byteVelocMotor3, 1}
  if(protocolo[_id + 1] <= ANTI_HORARIO){                 // Se o pwm do motor <= 127 
    digitalWrite(_sentido, LOW);                            // Rotação no sentido horário 
    _output = protocolo[_id + 1];                   // Output recebe o pwm contido no byte de velocidade do motor específico (1,2 ou 3)
  }
  else{                                                           // Se o pwm > 127 - rotação no sentido anti-horário
    digitalWrite(_sentido, HIGH);
    _output = (protocolo[_id + 1] & ANTI_HORARIO);  // Módulo 127
  }
 }


