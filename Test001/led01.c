#include "asuro.h"

void delayms(int t);    /* Prototype */

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
