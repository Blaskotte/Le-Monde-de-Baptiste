
#include <Arduino.h>
#include <U8g2lib.h>


//U8G2_SSD1306_128X64_NONAME_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R2, 13, 11, 10, 9);

void setup() {

    u8g2.begin();

    u8g2.enableUTF8Print(); 
}

void loop() {



    u8g2.firstPage();
    do {
u8g2.setFontMode(1);  /* activate transparent font mode */
u8g2.setDrawColor(1); /* color 1 for the box */
u8g2.setFont(u8g2_font_bauhaus2015_tr);
u8g2.setFontDirection(0);
u8g2.drawStr(90, 14, "10:12");
u8g2.drawLine(0, 16, 128, 16);
    u8g2.setFont(u8g2_font_unifont_h_symbols);
    u8g2.drawGlyph(1, 14, 0x23f0);	/* dec 9731/hex 2603 Snowman */
        
    } while ( u8g2.nextPage() );


    delay(10000);
}