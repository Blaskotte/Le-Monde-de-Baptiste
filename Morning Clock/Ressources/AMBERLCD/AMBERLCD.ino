
#include <Arduino.h>
#include <U8g2lib.h>


//U8G2_SSD1306_128X64_NONAME_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
U8G2_UC1701_MINI12864_2_4W_SW_SPI u8g2(U8G2_R2, 13, 11, 10, 9 , 8);

void setup() {

    u8g2.begin();

    u8g2.enableUTF8Print(); 
}

void loop() {



    u8g2.firstPage();
    do {
  
        
    } while ( u8g2.nextPage() );


    delay(10000);
}