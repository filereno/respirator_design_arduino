# 1 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino"
# 2 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino" 2
# 3 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino" 2
//------------------------------pins--------------------------------------------------------------
int startstop = 5, pinControlPress =A0, pinControlAliv =A1, pinValvPress =12, pinValvAliv =13;
//-------------------------------variables--------------------------------------------------------
int nPotPress = 0, nPotAliv = 0, difPotPress = 0, difPotAliv = 0, cacheTimePress = 0,
cacheTimeAliv = 0, statusStartStop, countTwoHundred, count=0;
//--------------------------------booleans--------------------------------------------------------
boolean checkB = false;
//--------------------------------display---------------------------------------------------------
LiquidCrystal lcd(3, 2, 8, 9, 10, 11);
//----------------------------------pin com i2c---------------------------------------------------

//------------------------------------------------------------------------------------------------

// configuraçao do lcd
void lcdConf(int x, int y)
{
    if (y == 1)
    {
        if (x < 10)
        {
           lcd.setCursor(6, 0);
           lcd.print(x);
            lcd.setCursor(7, 0);
            lcd.print(" ");
        }
        else if (x >= 10)
        {
            lcd.setCursor(6, 0);
           lcd.print(x);
        }
    }
    else if (y == 0)
    {
        if (x < 10)
        {
           lcd.setCursor(7, 1);
           lcd.print(x);
            lcd.setCursor(8, 1);
            lcd.print(" ");
        }
        else if (x >= 10)
        {
            lcd.setCursor(7, 1);
           lcd.print(x);
        }
    }
}



//------------------------------------------------------------
class Thread
{
  private:
      long _delay;
      unsigned long time;
  public:
      Thread(long delay);
      boolean temp();
      boolean refresh();
      boolean posDelay();
};

Thread::Thread(long delay)
{
    _delay = delay;
    time = millis();
}

boolean Thread::temp()
{
    return (time < millis()) ? true : false;
}

boolean Thread::refresh()
{
    time = millis() + _delay;
    return true;
}

boolean Thread::posDelay()
{
    return (temp() == true) ? refresh(): false;
}
//-----------------------------------------------------
Thread *ledPressThread, *ledAlivThread, *timeFiveHundredThreadA, *timeFiveHundredThreadB, *timeTwoHundredThreadA,
*timeTwoHundredThreadB, *confThread, *lcdThread;

void setup()
{
    lcd.begin(16, 2);
 lcd.print("Press:");
   lcd.setCursor(0, 1);
   lcd.print("Alivio:");

    pinMode(pinControlPress, 0x0);
    pinMode(pinControlAliv, 0x0);
    pinMode(pinValvPress, 0x1);
    pinMode(pinValvAliv, 0x1);
    pinMode(startstop, 0x2);

    statusStartStop = 0x0;

    Wire.begin(); // ingressa ao barramento I2C

    ledPressThread = new Thread(1000);
    ledAlivThread = new Thread(1000);
    timeFiveHundredThreadA = new Thread(250);
    timeFiveHundredThreadB = new Thread(250);
    timeTwoHundredThreadA = new Thread(100);
    timeTwoHundredThreadB = new Thread(100);
    confThread = new Thread(10);
    lcdThread = new Thread(10);

   Serial.begin(9600);
}

void resetCicle()
{
    ledPressThread = new Thread(1000);
    ledAlivThread = new Thread(1000);
    timeFiveHundredThreadA = new Thread(250);
    timeFiveHundredThreadB = new Thread(250);
    timeTwoHundredThreadA = new Thread(100);
    timeTwoHundredThreadB = new Thread(100);
}

void loop()
{

    nPotPress= analogRead(pinControlPress);
    nPotAliv= analogRead(pinControlAliv);

    // control pussh button    
    if (digitalRead(startstop) == 0x0 && checkB == false)
    {
        statusStartStop = !statusStartStop;
        checkB = true;
    }
    else if (digitalRead(startstop) == 0x1 && checkB == true)
    {
        checkB = false;
    }

    // thread controler potentiometer
    if (confThread->posDelay())
    {

        if (nPotPress != difPotPress)
        {
            cacheTimePress = map(nPotPress,0,1023,2,10);
            // confFunc(nPotPress, 1);
            lcdConf(cacheTimePress, 1);
        }

        if (nPotAliv != difPotAliv)
        {
            cacheTimeAliv = map(nPotAliv,0,1023,2,10);
            // confFunc(nPotAliv, 0);
            lcdConf(cacheTimeAliv, 0);
        }
        difPotPress = nPotPress;
        difPotAliv = nPotAliv;
    }

    if (statusStartStop == 0x1 && cacheTimePress > 0 && cacheTimeAliv > 0)
    {

        if (timeFiveHundredThreadA)
        {
            Serial.print("abre ");
            digitalWrite(pinValvPress, 0x1);//Abre a valvula de pressurização ## 11111111
        }

        // delay de 500ms ## 222222222
        if (timeFiveHundredThreadA->posDelay() && timeFiveHundredThreadA)
        {
            count = count + 1;
        }
        else if (count == 2 && timeFiveHundredThreadA)
        {
            Wire.beginTransmission(0x08);// inicia a transmissao
            Wire.write(2); // envia um byte contendo o estado do MOTOR
            Wire.endTransmission(); // encerra a transmissao
            Serial.print("500 ");
            count =0;
            delete timeFiveHundredThreadA;
            timeFiveHundredThreadA =
# 190 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino" 3 4
                                   __null
# 190 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino"
                                       ;
        }

        // contador de pressurização ## 33333333333
        if (ledPressThread->posDelay() && ledPressThread && !timeFiveHundredThreadA)
        {
            count = count + 1;
        }
        else if (count == cacheTimePress && ledPressThread && !timeFiveHundredThreadA)
        {
            Wire.beginTransmission(0x08);// inicia a transmissao
            Wire.write(0); // envia um byte contendo o estado do MOTOR
            Wire.endTransmission(); // encerra a transmissao
            Serial.print("press: ");
            Serial.print(count);
            count =0;
            delete ledPressThread;
            ledPressThread =
# 207 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino" 3 4
                           __null
# 207 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino"
                               ;
        }

        // delay de 500ms ## 444444444444
        if (timeFiveHundredThreadB->posDelay() && timeFiveHundredThreadB && !ledPressThread)
        {
            count = count + 1;
        }
        else if (count == 2 && timeFiveHundredThreadB && !ledPressThread)
        {
            digitalWrite(pinValvPress, 0x0); //fecha valvula de pressao ## 5555555555
            Serial.print(" 500 ");
            Serial.print(" fecha ");
            count =0;
            delete timeFiveHundredThreadB;
            timeFiveHundredThreadB =
# 222 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino" 3 4
                                   __null
# 222 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino"
                                       ;
        }

        // contador de 200ms ## 66666666666
        if (timeTwoHundredThreadA -> posDelay() && timeTwoHundredThreadA && !timeFiveHundredThreadB)
        {
            count = count + 1;
        }
        else if (count == 2 && timeTwoHundredThreadA && !timeFiveHundredThreadB)
        {
            digitalWrite(pinValvAliv, 0x1);//abrir valvula de alivio ## 777777777 
            Serial.print(" 200 ");
            Serial.print("abre ");
            count =0;
            delete timeTwoHundredThreadA;
            timeTwoHundredThreadA =
# 237 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino" 3 4
                                  __null
# 237 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino"
                                      ;
        }

        // contador de alivio ## 8888888888
        if (ledAlivThread -> posDelay() && ledAlivThread && !timeTwoHundredThreadA)
        {
            count = count + 1;
        }
        else if (count == cacheTimeAliv && ledAlivThread && !timeTwoHundredThreadA)
        {
            digitalWrite(pinValvAliv, 0x0);//fecha valvula de alivio ## 999999999
            Serial.print("alivio: ");
            Serial.print(count);
            count =0;
            delete ledAlivThread;
            ledAlivThread =
# 252 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino" 3 4
                          __null
# 252 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino"
                              ;
        }

        // contador de 200ms ## 10 10 10 10 10
        if (timeTwoHundredThreadB -> posDelay() && timeTwoHundredThreadB && !ledAlivThread)
        {
            count = count + 1;
        }
        else if (count == 2 && timeTwoHundredThreadB && !ledAlivThread)
        {

            Serial.print(" 200 ");
            count =0;
            delete timeTwoHundredThreadB;
            timeTwoHundredThreadB =
# 266 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino" 3 4
                                  __null
# 266 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino"
                                      ;
            resetCicle(); //reinicia o processo ## 11 11 11 11 11 11
        }
    }
///////////////////////////////////////////////////////////////////////////////////


}
