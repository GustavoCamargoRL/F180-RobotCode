#include <SerialCommunication.h>
#include <Radio.h>

SerialCommunication serial(38400);
Radio radio(0, 0, "TX");

void setup(){
  serial.begin();
  radio.setup();
}

void loop(){
  char msg[MSG_SIZE];
    
  radio.send(serial.read(msg), msg);
}
