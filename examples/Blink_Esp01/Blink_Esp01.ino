/*
     AUTOR:   BrincandoComIdeias
     YOUTUBE: https://www.youtube.com/brincandocomideias 
     CURSOS:  https://cursodearduino.net/ ; https://cursoderobotica.net
     LIBRARY: https://github.com/canalBrincandoComIdeias/A2a
*/

// INCLUSÃO DE BIBLIOTECAS
#include <A2a.h>

// DEFINIÇÃO DO ENDEREÇO DO SLAVE
#define endereco 8

// DEFINIÇÃO DAS PORTAS I2C
#define SDA 0
#define SCL 2

// DEFINIÇÕES DE PINOS DO SLAVE
#define pinBotao 3
#define pinLED   6
#define pinPot   A0

// INSTANCIANDO OBJETOS
A2a arduinoSlave;

void setup() {
  // INICIA A COMUNICAÇÃO ENTRE ESP01 E ARDUINO
  arduinoSlave.begin(SDA,SCL); 

  // ENVIA O pinMode PARA O SLAVE
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
