#include <LiquidCrystal.h>
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
  
    pinMode(motorA, OUTPUT);
    pinMode(motorB, OUTPUT);
    pinMode(pinControlPress, INPUT);
    pinMode(pinControlAliv, INPUT);
    pinMode(pinValvPress, OUTPUT);
    pinMode(pinValvAliv, OUTPUT);
    pinMode(startstop, INPUT_PULLUP);
    // startMillis = millis();  //initial start time
    statusStartStop = LOW;

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
    
    nPotPress=  analogRead(pinControlPress);
    nPotAliv=  analogRead(pinControlAliv);

    // control pussh button    
    if (digitalRead(startstop) == LOW && checkB == false)
    {
        
        statusStartStop = !statusStartStop;
        checkB = true;
        digitalWrite(13, statusStartStop);
    }
    else if (digitalRead(startstop) == HIGH && checkB == true)
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

    if (statusStartStop == HIGH && cacheTimePress > 0 && cacheTimeAliv > 0)
    {

        // contador de pressurização
        if (ledPressThread->posDelay() && ledPressThread)
        {  
            digitalWrite(pinValvPress, HIGH);//inicia pressurização
            count = count +  1; 
        }        
        else if (count == cacheTimePress && ledPressThread)
        {
            Serial.print(count);
            count =0;
            delete ledPressThread;
            ledPressThread =NULL;
        }

        // delay de 500ms
        if (timeFiveHundredThread->posDelay() && timeFiveHundredThread && !ledPressThread)
        {   
            count = count +  1;
        }        
        else if (count == 2 && timeFiveHundredThread && !ledPressThread)
        {
            digitalWrite(pinValvPress, LOW); //fecha valvula de pressao
            Serial.print(count);
            count =0;
            delete timeFiveHundredThread;
            timeFiveHundredThread =NULL;
        }

        // contador de 200ms
        if (timeTwoHundredThreadA -> posDelay() && timeTwoHundredThreadA && !timeFiveHundredThread)
        {   
            count = count +  1;
        }        
        else if (count == 2 && timeTwoHundredThreadA && !timeFiveHundredThread)
        {
            Serial.print(count);
            count =0;
            delete timeTwoHundredThreadA;
            timeTwoHundredThreadA =NULL;
        }

        // contador de alivio
        if (ledAlivThread -> posDelay() && ledAlivThread && !timeTwoHundredThreadA)
        {   
            digitalWrite(pinValvAliv, HIGH);//abrir valvula de alivio
            count = count +  1;
        }        
        else if (count == cacheTimeAliv && ledAlivThread && !timeTwoHundredThreadA)
        {
            Serial.print(count);
            count =0;
            delete ledAlivThread;
            ledAlivThread =NULL;
        }

        // contador de 200ms
        if (timeTwoHundredThreadB -> posDelay() && timeTwoHundredThreadB && !ledAlivThread)
        {   
            count = count +  1;
        }        
        else if (count == 2 && timeTwoHundredThreadB && !ledAlivThread)
        {
            digitalWrite(pinValvAliv, LOW);//fecha valvula de alivio
            Serial.print(count);
            count =0;
            delete timeTwoHundredThreadB;
            timeTwoHundredThreadB =NULL;
            resetCicle(); //reinicia o ciclo
        }    
    }
///////////////////////////////////////////////////////////////////////////////////


}