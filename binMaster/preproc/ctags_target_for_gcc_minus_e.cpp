# 1 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino"
# 2 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino" 2
//------------------------------pins--------------------------------------------------------------
int startstop = 5, pinControlPress =A0, pinControlAliv =A1, pinValvPress =12, pinValvAliv =13,
motorA = 7, motorB = 6;
//-------------------------------variables--------------------------------------------------------
int nPotPress = 0, nPotAliv = 0, difPotPress = 0, difPotAliv = 0, cacheTimePress = 0,
cacheTimeAliv = 0, statusStartStop, countTwoHundred, count=0;
//--------------------------------booleans--------------------------------------------------------
boolean checkB = false;
//--------------------------------display---------------------------------------------------------
LiquidCrystal lcd(3, 2, 8, 9, 10, 11);
//------------------------------------------------------------------------------------------------

// int timeCount(int x)
// {
//     int t, x1;
//     x1 = x;
//     t= (8 * x1)/1023 + 2;
//     return t;
// }

// void confFunc(int x, int y)
// {
//     if (y == 1)
//     {
//         cacheTimePress = timeCount(x);
//     }
//     else if (y == 0)
//     {
//         cacheTimeAliv = timeCount(x);
//     }
// }

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
Thread *ledPressThread, *ledAlivThread, *timeFiveHundredThread, *timeTwoHundredThreadA,
*timeTwoHundredThreadB, *confThread, *lcdThread;

void setup()
{
    lcd.begin(16, 2);
 lcd.print("Press:");
   lcd.setCursor(0, 1);
   lcd.print("Alivio:");

    pinMode(motorA, 0x1);
    pinMode(motorB, 0x1);
    pinMode(pinControlPress, 0x0);
    pinMode(pinControlAliv, 0x0);
    pinMode(pinValvPress, 0x1);
    pinMode(pinValvAliv, 0x1);
    pinMode(startstop, 0x2);
    // startMillis = millis();  //initial start time
    statusStartStop = 0x0;

    ledPressThread = new Thread(1000);
    ledAlivThread = new Thread(1000);
    timeFiveHundredThread = new Thread(250);
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
    timeFiveHundredThread = new Thread(250);
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
        digitalWrite(13, statusStartStop);
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

    // thread controler display
    // if (lcdThread ->posDelay())
    // {
    //     Serial.print("teste ");
    //     if (nPotPress != difPotPress)
    //     {
    //         Serial.print("teste1 ");
    //         lcdConf(cacheTimePress, 1);
    //     }
    //     else if (nPotAliv != difPotAliv)
    //     {
    //         Serial.print("teste2 ");
    //         lcdConf(cacheTimeAliv, 0);
    //     }    
    // }

    if (statusStartStop == 0x1 && cacheTimePress > 0 && cacheTimeAliv > 0)
    {

        // contador de pressurização
        if (ledPressThread->posDelay() && ledPressThread)
        {
            digitalWrite(pinValvPress, 0x1);//inicia pressurização
            count = count + 1;
        }
        else if (count == cacheTimePress && ledPressThread)
        {
            Serial.print(count);
            count =0;
            delete ledPressThread;
            ledPressThread =
# 216 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino" 3 4
                           __null
# 216 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino"
                               ;
        }

        // delay de 500ms
        if (timeFiveHundredThread->posDelay() && timeFiveHundredThread && !ledPressThread)
        {
            count = count + 1;
        }
        else if (count == 2 && timeFiveHundredThread && !ledPressThread)
        {
            digitalWrite(pinValvPress, 0x0); //fecha valvula de pressao
            Serial.print(count);
            count =0;
            delete timeFiveHundredThread;
            timeFiveHundredThread =
# 230 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino" 3 4
                                  __null
# 230 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino"
                                      ;
        }

        // contador de 200ms
        if (timeTwoHundredThreadA -> posDelay() && timeTwoHundredThreadA && !timeFiveHundredThread)
        {
            count = count + 1;
        }
        else if (count == 2 && timeTwoHundredThreadA && !timeFiveHundredThread)
        {
            Serial.print(count);
            count =0;
            delete timeTwoHundredThreadA;
            timeTwoHundredThreadA =
# 243 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino" 3 4
                                  __null
# 243 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino"
                                      ;
        }

        // contador de alivio
        if (ledAlivThread -> posDelay() && ledAlivThread && !timeTwoHundredThreadA)
        {
            digitalWrite(pinValvAliv, 0x1);//abrir valvula de alivio
            count = count + 1;
        }
        else if (count == cacheTimeAliv && ledAlivThread && !timeTwoHundredThreadA)
        {
            Serial.print(count);
            count =0;
            delete ledAlivThread;
            ledAlivThread =
# 257 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino" 3 4
                          __null
# 257 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino"
                              ;
        }

        // contador de 200ms
        if (timeTwoHundredThreadB -> posDelay() && timeTwoHundredThreadB && !ledAlivThread)
        {
            count = count + 1;
        }
        else if (count == 2 && timeTwoHundredThreadB && !ledAlivThread)
        {
            digitalWrite(pinValvAliv, 0x0);//fecha valvula de alivio
            Serial.print(count);
            count =0;
            delete timeTwoHundredThreadB;
            timeTwoHundredThreadB =
# 271 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino" 3 4
                                  __null
# 271 "c:\\Users\\fabiano\\Documents\\GitHub\\firmware_respirador_arduino\\appMaster.ino"
                                      ;
            resetCicle(); //reinicia o ciclo
        }
    }
///////////////////////////////////////////////////////////////////////////////////


}
