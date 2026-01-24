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
    char mainMenuState=0;
    
    char sensorsOverviewMode=0;
    char sensorsOverviewState=0;
    
    char calibMode =0;
    char calibModeState =0;
    
    char followLineMode = 0;
    char followLineState =0;
    
    int tempMillis=0;
    int startMillis=0;
    
    //variables calibrages
    int CNY_black_left_max = 0;
    int CNY_black_middle_max = 0;
    int CNY_black_right_max = 0;
    
    int CNY_black_left;
    CNY_black_left = DATAEE_ReadByte(0x10);
    __delay_ms(100);
    int CNY_black_middle = DATAEE_ReadByte(0x20);
    __delay_ms(100);
    int CNY_black_right = DATAEE_ReadByte(0x30);
    __delay_ms(100);
    int substractNumber = 180; //valeur à retirer à la valeur maximale pour avoir une valeur de switch d'état logique.
    
    
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
                
                if(!BP_selection()){
                    followLineMode=1;
                    while(!BP_selection()){
                        __delay_ms(100);
                    }
                followLineState=0;
                clearLCD();
                }
                break;
                
            case 1:
                gotoLCD(0,0); printLCD(" Follow a Line  ");
                gotoLCD(1,0); printLCD(">Sensors Review ");
                
                if(!BP_selection()){
                    sensorsOverviewMode=1;
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
                if(!BP_selection()){
                    calibModeState=0;
                    while(!BP_selection()){
                        __delay_ms(100);
                    }
                    calibMode=1;
                    calibModeState=0;
                    clearLCD();
                }
                break;
            case 3:
                gotoLCD(0,0); printLCD(" Calib. Mode    ");
                gotoLCD(1,0); printLCD(">4              ");
                break;
        }
    //-----Menu principal fin-----
    
    
    //-----Mode suivi de lignes-----
    while(followLineMode==1){
        
        /*switch(followLineState){
            case 0: //robot arreté et lancé si BP pressé
                
                break;
                
            case 1:
                
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
        }*/
        
        
        
        
        
        
        
        
        
        
        
        
        
        gotoLCD(1,3); LCDvalue16fp(CNY_black_left_max, 0);
        gotoLCD(1,9); LCDvalue16fp(CNY_black_middle_max, 0);
        gotoLCD(1,5); printLCD(" ");
        gotoLCD(1,15); LCDvalue16fp(CNY_black_right_max, 0);
        gotoLCD(1,11); printLCD(" ");
        if(ADCC_GetSingleConversion(31)>CNY_black_left){
             LED(0, 1);
        } else {
            LED(0, 0);
        }
        if(ADCC_GetSingleConversion(29)>CNY_black_right){
            LED(1, 1);
        }else {
            LED(1, 0);
        }
        if(!BP_selection()){
            followLineMode=0;
            while(!BP_selection()){
                __delay_ms(100);
            }
            LED(0, 0);
            LED(1, 0);
            Ecrit_Valeur_selection(mainMenuState);
        }
    }
    
    //-----Mode suivi de lignes fin-----
    
    
    //-----Aperçu des capteurs-----
    while(sensorsOverviewMode==1){
        
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
                gotoLCD(0,0); printLCD("CNY Sensors  1-4");
                gotoLCD(1,3); LCDvalue16fp(ADCC_GetSingleConversion(31), 0);
                gotoLCD(1,9); LCDvalue16fp(ADCC_GetSingleConversion(30), 0);
                gotoLCD(1,15); LCDvalue16fp(ADCC_GetSingleConversion(29), 0);
                gotoLCD(1,11); printLCD(" ");
                gotoLCD(1,5); printLCD(" ");
                break;
            
            case 1:
                //Capteurs CNY Booléens
                gotoLCD(0,0); printLCD("CNY Boolean  2-4");
                gotoLCD(1,0); printLCD("left:");
                gotoLCD(1,5); LCDvalue_Boolean(Digital_read(1));
                gotoLCD(1,9); printLCD("right:");
                gotoLCD(1,15); LCDvalue_Boolean(Digital_read(0));
                break;
                
            case 2:
                //Capteurs laser
                gotoLCD(0,0); printLCD("Laser Sensor 3-4");
                gotoLCD(1,2); LCDvalue_laser_Range(VL53L0X_Read_Distance_No_Wait(0));
                __delay_ms(50);
                gotoLCD(1,7); LCDvalue_laser_Range(VL53L0X_Read_Distance_No_Wait(1));
                __delay_ms(50);
                gotoLCD(1,12); LCDvalue_laser_Range(VL53L0X_Read_Distance_No_Wait(2));
                gotoLCD(1,14); printLCD("mm");
                break;
                
            case 3:
                //Microphone
                gotoLCD(0,0); printLCD("Microphone   4-4");
                
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
            sensorsOverviewMode=0;
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
    
    //-----Mode de calibration-----
    
    while(calibMode==1){
        switch(calibModeState){
            case 0:
                Motors(0);
                gotoLCD(0,0); printLCD("Set the robot on");
                gotoLCD(1,0); printLCD("a white surface ");
                if(!BP_selection()){
                    calibModeState=1;
                    while(!BP_selection()){
                        __delay_ms(100);
                    }
                    Buzzer(1);
                    __delay_ms(100);
                    Buzzer(0);
                    clearLCD();
                    CNY_black_left_max = 0;
                    CNY_black_middle_max = 0;
                    CNY_black_right_max = 0;
                    startMillis = millis();
                    
                }
                break;
                
            case 1:
                gotoLCD(0,0); printLCD(" Calibrating... ");
                gotoLCD(1,3); LCDvalue16fp(CNY_black_left_max, 0);
                gotoLCD(1,9); LCDvalue16fp(CNY_black_middle_max, 0);
                gotoLCD(1,5); printLCD(" ");
                gotoLCD(1,15); LCDvalue16fp(CNY_black_right_max, 0);
                gotoLCD(1,11); printLCD(" ");
                Motors(1);
                Speed_Motors(2, -2);
                
                if(CNY_black_left_max < ADCC_GetSingleConversion(31)){
                    CNY_black_left_max = ADCC_GetSingleConversion(31);
                }
                if(CNY_black_middle_max < ADCC_GetSingleConversion(30)){
                    CNY_black_middle_max = ADCC_GetSingleConversion(30);
                }
                if(CNY_black_right_max < ADCC_GetSingleConversion(29)){
                    CNY_black_right_max = ADCC_GetSingleConversion(29);
                }
                
                tempMillis = millis();
                
                if(tempMillis>(startMillis+2000)){
                    calibModeState=2;
                    Speed_Motors(0, 0);
                    Motors(0);

                    Buzzer(1);
                    __delay_ms(100);
                    Buzzer(0);
                    clearLCD();
                }
                __delay_ms(50);
                break;
            case 2:
                gotoLCD(0,0); printLCD("  Calib. done!  ");
                gotoLCD(1,3); LCDvalue16fp(CNY_black_left_max, 0);
                gotoLCD(1,9); LCDvalue16fp(CNY_black_middle_max, 0);
                gotoLCD(1,5); printLCD(" ");
                gotoLCD(1,15); LCDvalue16fp(CNY_black_right_max, 0);
                gotoLCD(1,11); printLCD(" ");
                if(!BP_selection()){
                    calibModeState=0;
                    while(!BP_selection()){
                        __delay_ms(100);
                    }
                    CNY_black_left = CNY_black_left_max - substractNumber;
                    CNY_black_middle = CNY_black_middle_max - substractNumber;
                    CNY_black_right = CNY_black_right_max - substractNumber;
                    
                    DATAEE_WriteByte(0x10, CNY_black_left);
                    __delay_ms(100);
                    DATAEE_WriteByte(0x20, CNY_black_middle);
                    __delay_ms(100);
                    DATAEE_WriteByte(0x30, CNY_black_right);
                    __delay_ms(100);
                    Buzzer(1);
                    gotoLCD(0,0); printLCD("Values saved in");
                    gotoLCD(1,0); printLCD("the EEPROM ! :) ");
                    for(int i=0;i<10;i++){
                        __delay_ms(100);
                    }
                    Buzzer(0);
                    calibMode=0;
                    clearLCD();    
                    Ecrit_Valeur_selection(mainMenuState);
                }
                Speed_Motors(0, 0);
                Motors(0);
                __delay_ms(100);
                break;
        }
    }
    
    //-----Mode de calibration fin-----
    
    }

}
    
    
