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

// INSTANCIANDO OBJETOS
A2a arduinoMaster;

void setup() {
  // INICIA A COMUNICAÇÃO ENTRE ARDUINOS COMO SLAVE NO ENDEREÇO DEFINIDO
  arduinoMaster.begin(endereco);

  // FUNÇÕES PARA COMUNICAÇÃO
  arduinoMaster.onReceive(receberDados);
  arduinoMaster.onRequest(enviarDados);
}

void loop() {

}

void receberDados() {
  arduinoMaster.receiveData(); 
}

void enviarDados() {
  arduinoMaster.sendData(); 
}
