#include <Motor.h>

Motor *robo[3];

void setup() {
  
  for(int i = 0; i < 3; i++){
    robo[i] = new Motor(i+1);
    robo[i]->configurar();
  }
   
}

void loop() {
  char msg[6] = {'M', 3, 100, 100, 100, 1};

  for(int j = 0; j < 3; j++)
    robo[j]->andar(msg);
  
}
