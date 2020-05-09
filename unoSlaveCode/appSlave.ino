#include <Servo.h>
#include "Wire.h"

#define myAdress 0x08
Servo meuservo; // Cria o objeto servo para programação 
int pos = 0; // Ajusta o ângulo inicial do Servo 

int pinValvPress = 6;
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