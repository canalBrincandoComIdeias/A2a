#include "A2a.h"

#include <inttypes.h>
#include "Arduino.h"
#include <Wire.h>

#if defined(__AVR__)
   #include <EEPROM.h>
#endif



A2a::A2a(void) {
}

void A2a::begin(void) {
	Wire.begin();
}

void A2a::begin(uint8_t address) {
	Wire.begin(address);

    #if defined(__AVR__)		
		for (int nL=0; nL < NUM_DIGITAL_PINS + NUM_ANALOG_INPUTS; nL++) {
		  pinMode(nL, EEPROM.read(nL));
		}
	#endif
}

void A2a::begin(int address) {
	Wire.begin(address);
	
	#if defined(__AVR__)		
		for (int nL=0; nL < NUM_DIGITAL_PINS + NUM_ANALOG_INPUTS; nL++) {
		  pinMode(nL, EEPROM.read(nL));
		}
	
	#endif
}

#if defined(ESP8266)
	void A2a::begin(int sda, int scl) {
		Wire.begin(sda, scl);
	}

	void A2a::begin(int sda, int scl, uint8_t address) {
		Wire.begin(sda, scl, address);	
	
        #if defined(__AVR__)	
			for (int nL=0; nL < NUM_DIGITAL_PINS + NUM_ANALOG_INPUTS; nL++) {
				pinMode(nL, EEPROM.read(nL));
			}
		#endif
	}
#endif

void A2a::pinWireMode(uint8_t address, uint8_t porta, uint8_t valor) {
  uint8_t comando = 0;        //comando 0=pinMode
  comando = (comando << 4);
  valor = comando | valor; //junta comando e valor no mesmo byte

  Wire.beginTransmission(address); 
  Wire.write(porta); //porta
  Wire.write(valor); //comando + valor
  Wire.endTransmission();  
  delay(1);  
}

void A2a::digitalWireWrite(uint8_t address, uint8_t porta, bool valor) {
  uint8_t comando = 1;            //comando 1=digitalWrite
  comando = (comando << 4);
  bitWrite(comando, 0, valor); //junta comando e valor no mesmo byte
    
  Wire.beginTransmission(address); 
  Wire.write(porta);    //porta
  Wire.write(comando);  //comando + valor
  Wire.endTransmission();    
}

void A2a::analogWireWrite(uint8_t address, uint8_t porta, uint8_t valor) {
  uint8_t comando = 2;        //comando 2=analogWrite
  comando = (comando << 4);

  Wire.beginTransmission(address); 
  Wire.write(porta);   //porta
  Wire.write(comando); //comando
  Wire.write(valor);   //valor
  Wire.endTransmission();    
}

bool A2a::digitalWireRead(uint8_t address, uint8_t porta) {
  uint8_t comando = 3;        //comando 3=digitalRead
  comando = (comando << 4);
  static bool lastRet;
    
  Wire.beginTransmission(address); 
  Wire.write(porta);   //porta
  Wire.write(comando); //comando
  Wire.endTransmission(); 
  
  delay(1); //DELAY PARA ESP01
  
  Wire.requestFrom(address, 1, false);    
  uint8_t rec;
  
  unsigned long waitAvailableTimeOut = millis();
  while (!Wire.available()) { 
     if ((millis() - waitAvailableTimeOut) > waitWireTimeout) {
		 break;
     }		 
  }
  
  if (Wire.available()) { 
     rec = Wire.read();    
	 lastRet = rec;
  } else {
	 rec = lastRet;
  }
  return rec;  
}

unsigned int A2a::analogWireRead(uint8_t address, uint8_t porta) {
  uint8_t comando = 4;        //comando 4=analogRead
  comando = (comando << 4);
  static unsigned int lastRet;

  Wire.beginTransmission(address); 
  Wire.write(porta); //porta
  Wire.write(comando); //valor
  Wire.endTransmission();
  
  delay(1); //DELAY PARA ESP01
  
  Wire.requestFrom(address, 2, false);    

  unsigned int rec;
  
  unsigned long waitAvailableTimeOut = millis();
  while (!Wire.available()) { 
     if ((millis() - waitAvailableTimeOut) > waitWireTimeout) {
		 break;
     }		 
  }
  
  if (Wire.available()) { 
     uint8_t rec1 = Wire.read();          
     uint8_t rec2 = Wire.read();     
     rec = ((rec2 << 8) | rec1);   
	 lastRet = rec;	 
  } else {
	 rec = lastRet;
  }
  
  return rec;
}

void A2a::varWireWrite(uint8_t address, uint8_t variavel, uint8_t valor) {
  uint8_t comando = 5;        //comando 5=varWrite
  comando = (comando << 4);
   
  Wire.beginTransmission(address); 
  Wire.write(variavel); //variavel
  Wire.write(comando);  //comando
  Wire.write(valor);    //valor
  Wire.endTransmission();    
}

void A2a::varWireWrite(uint8_t variavel, uint8_t valor) {
  varIntegr[variavel] = valor;  
}


uint8_t A2a::varWireRead(uint8_t address, uint8_t variavel) {
  uint8_t comando = 6;        //comando 6=varRead
  comando = (comando << 4);
  static uint8_t lastRet;

  Wire.beginTransmission(address); 
  Wire.write(variavel); //porta
  Wire.write(comando);  //comando
  Wire.endTransmission();
  
  Wire.requestFrom(address, 2, false);    

  unsigned int rec;
  
  unsigned long waitAvailableTimeOut = millis();
  while (!Wire.available()) { 
     if ((millis() - waitAvailableTimeOut) > waitWireTimeout) {
		 break;
     }		 
  }
  
  if (Wire.available()) { 
     rec = Wire.read();          
     lastRet = rec;	 
  } else {
	 rec = lastRet;
  }
    
  return rec;
}

uint8_t A2a::varWireRead(uint8_t variavel) {
  return varIntegr[variavel];
}

void A2a::receiveData() {
  uint8_t rec2;
  uint8_t valor;
  if (Wire.available()) { 
     porta = Wire.read();  //porta ou variavel
     rec2  = Wire.read();  //segundo byte
     
     comando = rec2;
     comando = (comando >> 4);

     valor = rec2;
     valor = (valor << 4);
     valor = (valor >> 4);

     if ((comando == 2) || (comando == 5)) {    
        valor = Wire.read();  //terceiro byte
     }
  }
 
  switch (comando) {
     case 0: 
        pinMode(porta, valor);
		#if defined(__AVR__)
			EEPROM.write(porta, valor);
		#endif
        break;
     case 1:
        digitalWrite(porta, valor);
        break;
     case 2:
	    #ifndef ESP32
           analogWrite(porta, valor);
		#endif
        break;
     case 5:
        varIntegr[porta] = valor;
        break;
  } 
}

void A2a::sendData() {
  if (comando == 3) {
     Wire.write(digitalRead(porta));
  } 
  
  if (comando == 4) {
     unsigned int vAnalogValue = analogRead(porta);
     Wire.write(lowByte(vAnalogValue));
     Wire.write(highByte(vAnalogValue));
  } 
  
  if (comando == 6) {
     Wire.write(varIntegr[porta]);
  }
}

// sets function called on slave write
void A2a::onReceive( void (*function)(int) )
{
  Wire.onReceive(function);
}

// sets function called on slave read
void A2a::onRequest( void (*function)(void) )
{
  Wire.onRequest(function);
}

void A2a::scan() {
byte resultCode = 0;
byte devices = 0;
byte address;
	
   for (address=0; address<128; address++){
		Wire.beginTransmission(address);
		resultCode = Wire.endTransmission();
    
		if (resultCode == 0){
			Serial.println();
			Serial.println(address,HEX);
			devices++;
			delay(3000);
		} else {
		    Serial.print(".");	
		}
		delay(100);
   }
   Serial.println();
    
   if (devices == 0){
       Serial.println("No devices found.");
   } else if (devices == 1) {
	   Serial.println("Only one device found.");
   } else {
	   Serial.print(devices);
	   Serial.println(" devices found"); 
   }	   
}

uint8_t A2a::lastWireError() {
	#ifdef ESP32
		return Wire.lastError();
	#endif ESP32
}