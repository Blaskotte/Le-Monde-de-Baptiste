#include "mcc_generated_files/mcc.h"
#include "drivers/vl53l0x.h"
#include <stdio.h>
#include <string.h>

#include "drivers/LCD.h"
#include "mcc_generated_files/eusart1.h"
#include <math.h>
#include "commandes.h"

void main(void)//__at 0x00200
{
    SYSTEM_Initialize();
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    
    LED(0, ON);
    LED(1, OFF);
    
    Motors(0);
    Speed_Motors( 0, 0 );
    initLCD();
    clearLCD();
    retroLCD(ON);
    __delay_ms(50);
    gotoLCD(0,0); printLCD("     HELLO      ");
    gotoLCD(1,0); printLCD("PROGRAMME MOI !!");
    __delay_ms(3000);
    
    Ecrit_Valeur_selection(0);
    int state = 0;
    int counter = 0;
    int calibration_state = 0;
    
while (1)
    {
    switch(state)
        {
            case 0:
                Motors(0);
                if(BP_selection()==0)
                {
                    state=1;
                    counter = 0;
                }
                break;
                
            case 1:
                counter++;
                Motors(1);
                Speed_Motors(10, -10);
                LED(1, 1);
                if(counter>=10)
                {
                    state=2;
                    counter = 0;
                    LED(1, 0);
                }
                break;
            
            case 2:
                counter++;
                Motors(1);
                Speed_Motors(10,0);
                LED(0, 1);
                if(counter>=10){
                    state=1;
                    counter = 0;
                    LED(0, 0);
                }
                break;
        }
    __delay_ms(100);
    
    /*switch(calibration_state)
    {
        case 0:
        Motors(1);
        Speed_Motors(10, -10);
                if(counter>=10)
                {
                    state=2;
                    counter = 0;
                    LED(1, 0);
                }
            break;
        
        case 1:
    }*/
    
    }

}
    
    
