#include <A2a.h>

#define endereco 8
#define pinBotao 3
#define pinLED   6
#define pinPot   A0

A2a arduinoSlave;

void setup() {
  arduinoSlave.begin();

  arduinoSlave.pinWireMode(endereco, pinBotao, INPUT_PULLUP);
  arduinoSlave.pinWireMode(endereco, pinLED, OUTPUT);
}

void loop() {
  if (arduinoSlave.digitalWireRead(endereco, pinBotao)) {
    arduinoSlave.digitalWireWrite(endereco, pinLED, HIGH);
    delay(250);
    arduinoSlave.analogWireWrite(endereco, pinLED, map(arduinoSlave.analogWireRead(endereco, pinPot),0,1023,0,255));
    delay(250);
  } else {
    arduinoSlave.digitalWireWrite(endereco, pinLED, LOW);
  }
}
