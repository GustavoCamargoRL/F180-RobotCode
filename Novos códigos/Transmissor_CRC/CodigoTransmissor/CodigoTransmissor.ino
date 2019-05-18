#include <SerialCommunication.h>
#include <Radio.h>
#include <FastCRC.h>

#define SIZE_MSG 7
#define START 77
FastCRC16 CRC16;

SerialCommunication serial(38400);
Radio radio(0, 109, "TX");            //Módulo, canal, modo

bool crc_check(char msg[SIZE_MSG]);

void setup() {
  serial.begin();
  radio.setup();
}

void loop() {
  char msg[SIZE_MSG];

  if (serial.read(msg) && msg[0] == START && crc_check(msg)) {
    radio.send(true, msg);    // Envia mensagem recebida
  }
  else {
    radio.send(false, msg);
  }
}

// Checks if CRC is OK
bool crc_check(char msg[SIZE_MSG]) {
  uint8_t msg_crc[SIZE_MSG];
  for (int i = 0; i < SIZE_MSG; i++) {
    msg_crc[i] = msg[i];
  }

  return (CRC16.xmodem(msg_crc, sizeof(msg_crc)) == 0);
}
