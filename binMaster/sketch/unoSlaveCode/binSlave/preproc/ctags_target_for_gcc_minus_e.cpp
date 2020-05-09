# 1 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\unoSlaveCode\\appSlave.ino"
# 2 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\unoSlaveCode\\appSlave.ino" 2
# 3 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\unoSlaveCode\\appSlave.ino" 2


Servo meuservo; // Cria o objeto servo para programação 
int pos = 0; // Ajusta o ângulo inicial do Servo 

int pinValvPress = 6;
void setup()
{
    Wire.begin(0x08);
    Wire.onReceive(receiveEvent);
    meuservo.attach(4);
    meuservo.write(0); // Inicia motor posição zero
    pinMode(pinValvPress, 0x1);
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
            digitalWrite(pinValvPress, 0x1);
            meuservo.write(180);
        }

        //se o byte recebido for igual a 1 acende o LED
        if (received == 0)
        {
            digitalWrite(pinValvPress, 0x0);
            meuservo.write(0);
        }
    }
}
