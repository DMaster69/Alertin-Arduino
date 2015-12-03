/*
GPRS Connect TCP
 
 This sketch is used to test seeeduino GPRS_Shield's send SMS func.
 to make it work, you should insert SIM card to Seeeduino GPRS 
 and replace the phoneNumber,enjoy it!
 
 create on 2013/12/5, version: 0.1
 by lawliet.zou(lawliet.zou@gmail.com)
 */
#include "Ultrasonic.h"
#include "InfraredReceiver.h"
#include <GPRS_Shield_Arduino.h>
#include <SoftwareSerial.h>

#include <Wire.h>
#include <Suli.h>

#define PIN_TX    7
#define PIN_RX    8
#define BAUDRATE  9600
#define PHONE_NUMBER "+569xxxxxxxx"
#define MESSAGE  "Alerta!! Este es un mensaje"

boolean swEnvio=false;
boolean swEncendido = false;
boolean swLimite = false;
Ultrasonic ultrasonic(10,11);//(Trig,Echo)
int distancia;
int limite;
int key;

InfraredReceiver infraredReceiver; 
GPRS gprsTest(PIN_TX,PIN_RX,BAUDRATE);//RX,TX,BaudRate

void setup() {
  Serial.begin(9600);
  infraredReceiver.begin();
  while(0 != gprsTest.init()) {
    delay(1000);
    Serial.print("init error\r\n");
  }
  Serial.println("inicializacion gprs satisfactoria");
  
}

void loop() {
  key = infraredReceiver.read();// get the pressed key information
   
  if(swEncendido){
    if(!swLimite){
      swLimite=true;
      limite  =  ultrasonic.Ranging(CM);
      Serial.print("Configurado limite automatico a ");
      Serial.print(limite);
      Serial.println(" cm de distancia");    
    }
    distancia  =  ultrasonic.Ranging(CM);
    Serial.print("Retorno a ");
    Serial.print(distancia);
    Serial.println(" cm ");
    if(distancia<limite-5 || distancia>limite+5){
      if(!swEnvio){
        Serial.println("Enviando mensaje de alerta ...");
        gprsTest.sendSMS(PHONE_NUMBER,MESSAGE);
        swEnvio=true;
        swEncendido = false;
        swLimite = false;
      }else{
        Serial.println("Alarma Activada - Mensaje ya enviado"); 
      }
    }
    if (key == 1 && swEncendido){
      swEncendido = false;
    }
    delay(500);
  }else{
    if (key == 1){
      swEncendido = true;
    }
  }
}
