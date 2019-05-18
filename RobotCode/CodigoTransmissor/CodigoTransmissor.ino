#include <SerialCommunication.h>
#include <Radio.h>

#define tempoDelay 1000
#define tempoDelay2 1090

SerialCommunication serial(38400);
Radio radio(0, 109, "TX");            //Módulo, canal, modo

void setup(){
  serial.begin();
  radio.setup();
}

bool isChecksumOk(char msg[]) {
  return (msg[6] == (msg[0] ^ msg[1] ^ msg[2] ^ msg[3] ^ msg[4] ^ msg[5]));
}

void loop(){
  static unsigned long tempo = millis();
  char msg[7]; // message size  + 1 (checksum)      

  if(serial.read(msg)){
    //if(millis() - tempo >= tempoDelay && millis() - tempo <= tempoDelay2)
    //  msg[1] |= 0x80;
    //else if(millis() - tempo > tempoDelay2)
    //  tempo = millis();
    //else
    //  msg[1] &= 0x7F;
    if(msg[0] == START && msg[5] == FINAL && isChecksumOk(msg)) {
      radio.send(true, msg);
    }      
  }

  else{
    radio.send(false, msg); 
  }
}
