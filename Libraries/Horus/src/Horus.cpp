#include "Horus.h"
#include "Arduino.h"

Horus::Horus(Radio *radio, byte id){
    this->radio = radio;
    this->id = id;
}

void Horus::horus(){
    radio->disableRX();
    
    char sendFrame[7];
    sendFrame[0] = 'H';
    sendFrame[1] = (char) id;
    sendFrame[2] = 10*1;//Baterial()
    sendFrame[3] = (char) 1;//Carga do capacitor
    sendFrame[4] = digitalRead(2)//Infrared
    sendFrame[5] = digitalRead(2);//drible
    sendFrame[6] = 'M';

    radio->send(true, sendFrame);

    radio->enableRX();
}