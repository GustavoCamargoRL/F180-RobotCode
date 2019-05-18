#ifndef Horus_h
#define Horus_h

#include <Radio.h>

class Horus{
public:
    Horus(Radio *);
    void horus();

private:
	Radio *radio;
    byte id;
};

#endif