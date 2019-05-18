#include <SerialCommunication.h>

SerialCommunication serial(38400);

void setup(){
  
}

void loop(){
  char msg[MSG_SIZE];
  
  serial.read(msg);

  Serial.println(msg);
}
