#include <Radio.h>
 
Radio radio(3, "RX"); //(Canal, Modo(TX/RX))

void setup() {
  Serial.begin(38400);

  radio.setup();
}

void loop() {
  char msg[MSG_SIZE];
  
  if(radio.receive(msg)){
    Serial.println(msg);
  }
}
