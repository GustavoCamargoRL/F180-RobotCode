#ifndef SerialCommunication_h
#define SerialCommunication_h

#define MSG_SIZE 7
#define START 0x4D      // Byte de start 'M'
#define FINAL 0x46      // Byte de final 'F'

class SerialCommunication{
public:
	SerialCommunication(int);
	bool read(char []);
	void begin();
	void ajustArray(char []);

private:
	int _baud;
};

#endif