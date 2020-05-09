#include <Arduino.h>
#line 1 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\unoSlaveCode\\appSlave.ino"
#include <Servo.h>
#include "Wire.h"

#define myAdress 0x08
Servo meuservo; // Cria o objeto servo para programação 
int pos = 0; // Ajusta o ângulo inicial do Servo 

int pinValvPress = 6;
#line 9 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\unoSlaveCode\\appSlave.ino"
void setup();
#line 19 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\unoSlaveCode\\appSlave.ino"
void loop();
#line 24 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\unoSlaveCode\\appSlave.ino"
void receiveEvent(int howMany);
#line 9 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\unoSlaveCode\\appSlave.ino"
void setup()
{
    Wire.begin(myAdress);
    Wire.onReceive(receiveEvent);
    meuservo.attach(4);
    meuservo.write(0); // Inicia motor posição zero
    pinMode(pinValvPress, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
  //faz nada
}

void receiveEvent(int howMany) 
{
    // verifica se existem dados para serem lidos no barramento I2C
    if (Wire.available())
    {
        // le o byte recebido
        char received = Wire.read();

        // se o byte recebido for igual a 0, apaga o LED
        if (received == 1) 
        {
            digitalWrite(pinValvPress, HIGH);
            meuservo.write(180);
        }

        //se o byte recebido for igual a 1 acende o LED
        if (received == 0) 
        {
            digitalWrite(pinValvPress, LOW);
            meuservo.write(0);
        }
    }
}  
