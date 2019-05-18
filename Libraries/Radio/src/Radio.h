#ifndef Radio_h
#define Radio_h

#include <Enrf24.h>
#include <SPI.h>

#define CANALHORUS 2

//Testando
const uint8_t radio_pins[5][3] = {
	{PA_3, PE_0, PB_3},
	{PF_3, PC_6, PC_7},
	{PB_5, PC_4, PC_5},
	{PD_1, PB_2, PC_5},
	{PD_1, PE_1, PB_3}		//Modulo da placa de comunicação nova (CS/CE (PD_1), CSN, IRQ) - novo PD_1, antigo PE_2    PF_4 - novo / PB_3 - antigo
};

const uint8_t led_pins[5][3] = {
	// R           G            B
	{ PB_0,       PE_4,        PB_1  },
	{ PE_5,       PA_7,        PA_6  },
	{ PD_6,       PF_4,        PD_7  },
	{ PE_1,       PE_3,        PE_2  },
	{RED_LED,   GREEN_LED,   BLUE_LED}
};
const uint8_t txaddr[5] = {0x11, 0x22, 0x33, 0x44, 0x55};
const uint8_t rxaddr[5] = {0x11, 0x22, 0x33, 0x44, 0x55};

class Radio{
public:
	Radio(int, int, String);
	Radio(int, String);
	void send(boolean, char []);
	void setup();
	boolean receive(char [], int);
	void sendHorus(char []);
	boolean _led_state;

private:
	Enrf24 _radio;
	int _radio_module, _spi_module, _canal;
	String _modo;
	unsigned int _fail_safe;
	void (* resetFunc)(void) = 0; //Tentativa de dar reset por software
};
#endif
