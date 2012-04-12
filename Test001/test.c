#include "asuro.h"

void delayms(int t)
{
 int n;
 
 for (n=1; n<=t; n++)
 {
  Sleep(72);            /* Sleep 0.001s (1ms)    */
 }
}


/* LED Test */
void ledtest(void)
{
    StatusLED(RED);
    delayms(300);
        
    StatusLED(GREEN);
    delayms(100);
        
    StatusLED(YELLOW);
    delayms(50);
        
    StatusLED(OFF);
    delayms(1000);
        
    FrontLED(ON);
    
    delayms(200);
    
    BackLED(ON, ON);
    delayms(500);
    
    FrontLED(OFF);
    BackLED(OFF, OFF);
    delayms(500);
}

int main(void)
{
    Init();
    
    while(TRUE)
    {
        StatusLED(GREEN);
        BackLED(ON, OFF);
        MotorDir(FWD, FWD);
        MotorSpeed(100, 100);
        delayms(500);
        

        StatusLED(RED);
        BackLED(OFF, ON);
        MotorDir(RWD, RWD);
        MotorSpeed(255, 255);
        delayms(2000);
    };
    return 0;
}

