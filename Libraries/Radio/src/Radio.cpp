#include "Radio.h"

//Construtor para a base station
Radio::Radio(int radio_module, int canal, String modo){
	this->_radio_module = 	radio_module;
	this->_spi_module 	=   radio_module;
	this->_canal		=	canal;
	this->_modo 		= 	modo;
	this->_fail_safe	=	millis();
	this->_led_state	=	false;
}

//Construtor para o robo
Radio::Radio(int canal, String modo){
	this->_radio_module = 	4;
	this->_spi_module 	= 	3;
	this->_canal		=	canal;
	this->_modo 		= 	modo;
	this->_fail_safe	=	millis();
	this->_led_state	=	false;
}

void Radio::setup(){
	//Setup SPI comunitaction
	SPI.begin();
  	SPI.setDataMode(SPI_MODE0);
  	SPI.setBitOrder(MSBFIRST);
  	SPI.setModule(this->_spi_module);
	
  	//Setup radio
  	Enrf24 radio(radio_pins[this->_radio_module][0], radio_pins[this->_radio_module][1], radio_pins[this->_radio_module][2]);
  	this->_radio = radio;
  	this->_radio.begin();
	
	//Setup modo de execução: RX/TX
  	this->_radio.setChannel(this->_canal);
    if(this->_modo == "TX") this->_radio.setTXaddress((void*)txaddr);
    else if(this->_modo=="RX"){
     	this->_radio.setRXaddress((void*)rxaddr);
      	this->_radio.enableRX();
    }
	
	//Setup state led pins
    pinMode(led_pins[this->_radio_module][0], OUTPUT);
    pinMode(led_pins[this->_radio_module][1], OUTPUT);
    pinMode(led_pins[this->_radio_module][2], OUTPUT);
}

//Enviar mensagem
void Radio::send(boolean msg_received, char msg[]){

	//Se o rádio está presente
	if(!this->_radio.radioState() == ENRF24_STATE_NOTPRESENT){

		//Se alguma  mensagem foi recebida pela serial
		if(msg_received){

			//Envia a mensagem
			this->_radio.print(msg);
	        this->_radio.flush();

			//Feedback com leds
			digitalWrite(led_pins[this->_radio_module][0], LOW);
			digitalWrite(led_pins[this->_radio_module][1], this->_led_state);
			digitalWrite(led_pins[this->_radio_module][2], LOW);
			this->_led_state = !this->_led_state;	        
		}
		else{

			//Esperando mensagem

			//Feedbak com led
			digitalWrite(led_pins[this->_radio_module][0], LOW);
			digitalWrite(led_pins[this->_radio_module][1], LOW);
			digitalWrite(led_pins[this->_radio_module][2], HIGH);
		}
	}
	else{

		//Erro

		//Feedbak com leds
		digitalWrite(led_pins[this->_radio_module][0], HIGH);
		digitalWrite(led_pins[this->_radio_module][1], LOW);
		digitalWrite(led_pins[this->_radio_module][2], LOW);
	}
}

//Receber mensagem
boolean Radio::receive(char msg[], int tamProtocolo){

	//Se o rádio está presente
	if(!this->_radio.radioState() == ENRF24_STATE_NOTPRESENT){

		//Se alguma mensagem foi recebida pelo rádio
		if(this->_radio.available(true)){

			//Armazena a mensagem no vetor msg
			if(this->_radio.read(msg, tamProtocolo)){

				//Se o armazenamento foi correto

				//Feedack com leds
				digitalWrite(led_pins[this->_radio_module][0], LOW);
				digitalWrite(led_pins[this->_radio_module][1], this->_led_state);
				digitalWrite(led_pins[this->_radio_module][2], LOW);
				this->_led_state = !this->_led_state;

				//Reinicia o _fail_safe
				this->_fail_safe = millis();

				return true;
			}
		}
		
		//Se passou 500 milisegundos sem receber mensagem
		if(millis() - this->_fail_safe > 500){

			//Feedback com leds
			digitalWrite(led_pins[this->_radio_module][0], LOW);
			digitalWrite(led_pins[this->_radio_module][1], HIGH);
			digitalWrite(led_pins[this->_radio_module][2], HIGH);

			setup();

			//Reinicia o _fail_safe
			this->_fail_safe = millis();
		}
	}
	else{

		//Erro

		//Feedback com leds
		digitalWrite(led_pins[this->_radio_module][0], HIGH);
		digitalWrite(led_pins[this->_radio_module][1], LOW);
		digitalWrite(led_pins[this->_radio_module][2], LOW);

		setup();
	}

	return false;
}

void Radio::sendHorus(char sendFrame[]){
	this->_radio.setChannel(CANALHORUS);
	this->_radio.setTXaddress((void*)txaddr);
	this->_radio.disableRX();
    this->send(true, sendFrame);
	this->_radio.setChannel(this->_canal);
    this->_radio.setRXaddress((void*)rxaddr);
	this->_radio.enableRX();
}