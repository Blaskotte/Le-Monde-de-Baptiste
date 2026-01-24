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
    
    Motors(0);
    Speed_Motors( 0, 0 );
    VL53L0X_InitDevices();
    Buzzer(1);
    __delay_ms(100);
    Buzzer(0);
    
    //demarrage LCD
    initLCD();
    retroLCD(ON);
    clearLCD();
    gotoLCD(0,0); printLCD("-- Welcome :) --");
    for(int i=0;i<10;i++){
        __delay_ms(100);
    }
    clearLCD();
    Buzzer(1);
    __delay_ms(100);
    Buzzer(0);

    
    //variabes personnelles
    int mainMenuState=0;
    int sensorsOverview=0;
    int sensorsOverviewState=0;

    
    
while (1)
    {
    
    //-----Menu principal début-----
    if(Valeur_selection() < 0 || Valeur_selection() > 1000){
        Ecrit_Valeur_selection(0);
    }
    if(Valeur_selection() > 3){
        Ecrit_Valeur_selection(3);
    }
    mainMenuState=Valeur_selection();
    
    
    switch(mainMenuState){
            case 0:
                gotoLCD(0,0); printLCD(">Follow a Line  ");
                gotoLCD(1,0); printLCD(" Sensors Review ");
                break;
                
            case 1:
                gotoLCD(0,0); printLCD(" Follow a Line  ");
                gotoLCD(1,0); printLCD(">Sensors Review ");
                
                if(!BP_selection()){
                    sensorsOverview=1;
                    while(!BP_selection()){
                        __delay_ms(100);
                    }
                Ecrit_Valeur_selection(0);
                clearLCD();
                }
                
                break;
            
            case 2:
                gotoLCD(0,0); printLCD(">Calib. Mode    ");
                gotoLCD(1,0); printLCD(" 4              ");
                break;
            case 3:
                gotoLCD(0,0); printLCD(" Calib. Mode    ");
                gotoLCD(1,0); printLCD(">4              ");
                break;
        }
    //-----Menu principal fin-----
    
    //-----Aperçu des capteurs-----
    while(sensorsOverview==1){
        
        if(Valeur_selection() < 0 || Valeur_selection() > 1000){
        Ecrit_Valeur_selection(0);
        }
        if(Valeur_selection() > 3){
        Ecrit_Valeur_selection(3);
        }
        if(sensorsOverviewState!=Valeur_selection()){
            sensorsOverviewState=Valeur_selection();
            clearLCD();
        }
                
        switch(sensorsOverviewState){
            case 0:
                //Capteurs CNY
                gotoLCD(0,0); printLCD("1- CNY Senso. -4");
                gotoLCD(1,3); LCDvalue16fp(ADCC_GetSingleConversion(31), 0);
                gotoLCD(1,9); LCDvalue16fp(ADCC_GetSingleConversion(30), 0);
                gotoLCD(1,15); LCDvalue16fp(ADCC_GetSingleConversion(29), 0);
                gotoLCD(1,11); printLCD(" ");
                gotoLCD(1,5); printLCD(" ");
                break;
            
            case 1:
                //Capteurs CNY Booléens
                gotoLCD(0,0); printLCD("2- CNY  Bool. -4");
                gotoLCD(1,0); printLCD("left:");
                gotoLCD(1,5); LCDvalue_Boolean(Digital_read(1));
                gotoLCD(1,9); printLCD("right:");
                gotoLCD(1,15); LCDvalue_Boolean(Digital_read(0));
                break;
                
            case 2:
                //Capteurs laser
                gotoLCD(0,0); printLCD("3-Laser Senso.-4");
                gotoLCD(1,2); LCDvalue_laser_Range(VL53L0X_Read_Distance_No_Wait(0));
                __delay_ms(50);
                gotoLCD(1,7); LCDvalue_laser_Range(VL53L0X_Read_Distance_No_Wait(1));
                __delay_ms(50);
                gotoLCD(1,12); LCDvalue_laser_Range(VL53L0X_Read_Distance_No_Wait(2));
                gotoLCD(1,14); printLCD("mm");
                break;
                
            case 3:
                //Microphone
                gotoLCD(0,0); printLCD("4- Microphone -4");
                
                if(Digital_read(2)){
                    gotoLCD(1,4); printLCD("True  :)");
                    Buzzer(1);
                    __delay_ms(100);
                    Buzzer(0);

                } else {
                    gotoLCD(1,4); printLCD("False :(");
                }
                break;
            case 4:
                break;
        }
        
        if(!BP_selection()){
            sensorsOverview=0;
                while(!BP_selection()){
                        __delay_ms(100);
                }
            Ecrit_Valeur_selection(mainMenuState);
            clearLCD();
        }
        __delay_ms(100);

        /*if(!BP_selection()){
            sensorsOverview=0;
            while(!BP_selection()){
                        __delay_ms(100);
                    }
        }*/
    }
    //-----Aperçu des capteurs fin-----
    
    //__delay_ms(100);
    
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
    
    
