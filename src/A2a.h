#ifndef A2a_h
#define A2a_h

#include <inttypes.h>
#include "Arduino.h"

#define qtdVar 10       //quantidade de variaveis para integração

class A2a {
  public:
    A2a(void);
	void begin(void);
	void begin(uint8_t);
	void begin(int);
    void pinWireMode(uint8_t, uint8_t, uint8_t);
	void digitalWireWrite(uint8_t, uint8_t, bool);
	bool digitalWireRead(uint8_t, uint8_t);
	void analogWireWrite(uint8_t, uint8_t, uint8_t);
	unsigned int analogWireRead(uint8_t, uint8_t);
	void varWireWrite(uint8_t, uint8_t, uint8_t);
	uint8_t varWireRead(uint8_t, uint8_t);
	void receiveData(void);
	void sendData(void);
	void onReceive( void (*)(int) );
    void onRequest( void (*)(void) );
  private:
	uint8_t porta;
	uint8_t comando;
	uint8_t varIntegr[qtdVar];
};

#endif