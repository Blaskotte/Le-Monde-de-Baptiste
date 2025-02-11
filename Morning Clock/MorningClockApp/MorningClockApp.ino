
#include <Arduino.h>
#include <U8g2lib.h>
#include <DS3231.h>
#include <Wire.h>


U8G2_UC1701_MINI12864_2_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/12, /* cs=*/11, /* dc=*/10, /* reset=*/9);

// 'happyMenu', 8x9px
const unsigned char happyMenu [] PROGMEM = {
	0x49, 0x22, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x14, 0x14
};
const unsigned char happyMenu_select [] PROGMEM = {
	// 'happyMenu_select, 9x11px
	0xfe, 0x00, 0x6d, 0x01, 0xbb, 0x01, 0xc7, 0x01, 0xc7, 0x01, 0xc7, 0x01, 0xc7, 0x01, 0xc7, 0x01, 
	0xd7, 0x01, 0xd7, 0x01, 0xfe, 0x00
};
const unsigned char alarmON [] PROGMEM = {
	// 'alarmON, 8x9px
	0x18, 0x3c, 0x7e, 0x7e, 0x7e, 0x7e, 0xff, 0x00, 0x18
};
const unsigned char alarmON_select [] PROGMEM = {
	// 'alarmON_select, 10x11px
	0xfe, 0x01, 0xcf, 0x03, 0x87, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x02, 
	0xff, 0x03, 0xce, 0x03, 0xfe, 0x01
};
const unsigned char chimeON [] PROGMEM = {
	// 'chimeON, 8x9px
	0xe0, 0xfc, 0x9c, 0x84, 0x84, 0xc4, 0xe6, 0x67, 0x03
};
const unsigned char chimeON_select [] PROGMEM = {
	// 'chimeON_select, 10x11px
	0xfe, 0x01, 0x3f, 0x02, 0x07, 0x02, 0xc7, 0x02, 0xf7, 0x02, 0xf7, 0x02, 0x77, 0x02, 0x33, 0x02, 
	0x31, 0x03, 0xf9, 0x03, 0xfe, 0x01
};
const unsigned char clockICON [] PROGMEM = {
	// 'clockICON, 8x9px
	0x42, 0xbd, 0x42, 0x89, 0x89, 0xb9, 0x81, 0x42, 0x3c
};


// 'alarmOFF', 10x11px
const unsigned char alarmOFF [] PROGMEM = {
	0x19, 0x00, 0x3e, 0x00, 0x7c, 0x00, 0x7a, 0x00, 0x76, 0x00, 0x6e, 0x00, 0xdf, 0x00, 0x80, 0x00, 
	0x18, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'alarmOFF_select', 10x11px
const unsigned char alarmOFF_select [] PROGMEM = {
	0xfe, 0x01, 0xcd, 0x03, 0x83, 0x03, 0x07, 0x03, 0x0b, 0x03, 0x13, 0x03, 0x23, 0x03, 0x41, 0x02, 
	0xff, 0x02, 0xcf, 0x03, 0xfe, 0x01
};
// 'chimeOFF', 10x11px
const unsigned char chimeOFF [] PROGMEM = {
	0xe1, 0x00, 0xfe, 0x00, 0x9c, 0x00, 0x8c, 0x00, 0x94, 0x00, 0xe4, 0x00, 0xe6, 0x00, 0x67, 0x00, 
	0x83, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'chimeOFF_select', 10x11px
const unsigned char chimeOFF_select [] PROGMEM = {
	0xfe, 0x01, 0x3d, 0x02, 0x03, 0x02, 0xc7, 0x02, 0xe7, 0x02, 0xd7, 0x02, 0x37, 0x02, 0x33, 0x02, 
	0x31, 0x03, 0xf9, 0x02, 0xfe, 0x01
};



int alarmHour = 12;
int alarmMinute = 6;

DS3231  rtc(SDA, SCL);
Time  RTClock;

void setup() {
  rtc.begin();
  u8g2.begin();
  u8g2.enableUTF8Print();
}

void loop() {

  u8g2.firstPage();
  do {

    // Page principale (alarmON/chimeON)

    RTClock = rtc.getTime();

    //affichage de la barre des réglages
    u8g2.drawLine(0, 13, 128, 13);
    u8g2.drawXBMP(2, 2, 7, 9, happyMenu);
    u8g2.drawXBMP(17, 2, 8, 9, alarmON);
    u8g2.drawXBMP(28, 2, 8, 9, chimeON);

    //affichage de l'heure d'activation de l'alarme si alarme est activée
    u8g2.setFont(u8g2_font_profont11_mn);
    u8g2.setCursor(86, 10);
    u8g2.print(twoDigit(alarmHour));
    u8g2.print(':');
    u8g2.print(twoDigit(alarmMinute));
    u8g2.drawXBMP(118, 2, 8, 9, clockICON);


    //u8g2.drawLine(63, 0, 63, 64);
    //u8g2.drawLine(64, 0, 64, 64);
    //affichage de l'heure
    u8g2.setFont(u8g2_font_osr29_tn);
    u8g2.setCursor(10, 53);
    u8g2.print(twoDigit(RTClock.hour));
    u8g2.print(':');
    u8g2.print(twoDigit(RTClock.min));

  } while (u8g2.nextPage());


    delay(2000);

  u8g2.firstPage();
  do {

    // Page principale (alarmOFF/chimeOFF)

    RTClock = rtc.getTime();

    //affichage de la barre des réglages
    u8g2.drawLine(0, 13, 128, 13);
    u8g2.drawXBMP(2, 2, 7, 9, happyMenu);
    u8g2.drawXBMP(17, 2, 10, 11, alarmOFF);
    u8g2.drawXBMP(28, 2, 10, 11, chimeOFF);

    //affichage de l'heure
    u8g2.setFont(u8g2_font_osr29_tn);
    u8g2.setCursor(10, 53);
    u8g2.print(twoDigit(RTClock.hour));
    u8g2.print(':');
    u8g2.print(twoDigit(RTClock.min));

  } while (u8g2.nextPage());
  delay(2000);

    u8g2.firstPage();
  do {

    // Page principale (Select)

    RTClock = rtc.getTime();

    //affichage de la barre des réglages
    u8g2.drawLine(0, 13, 128, 13);
    u8g2.drawXBMP(1, 1, 9, 11, happyMenu_select);
    u8g2.drawXBMP(16, 1, 10, 11, alarmOFF_select);
    u8g2.drawXBMP(27, 1, 10, 11, chimeOFF_select);

    //affichage de l'heure
    u8g2.setFont(u8g2_font_osr29_tn);
    u8g2.setCursor(10, 53);
    u8g2.print(twoDigit(RTClock.hour));
    u8g2.print(':');
    u8g2.print(twoDigit(RTClock.min));

  } while (u8g2.nextPage());

  delay(2000);

      u8g2.firstPage();
  do {

    // Page principale (Select)

    RTClock = rtc.getTime();

    //affichage de la barre des réglages
    u8g2.drawLine(0, 13, 128, 13);
    u8g2.drawXBMP(1, 1, 9, 11, happyMenu_select);
    u8g2.drawXBMP(16, 1, 10, 11, alarmON_select);
    u8g2.drawXBMP(27, 1, 10, 11, chimeON_select);

    //affichage de l'heure d'activation de l'alarme si alarme est activée
    u8g2.setFont(u8g2_font_profont11_mn);
    u8g2.setCursor(86, 10);
    u8g2.print(twoDigit(alarmHour));
    u8g2.print(':');
    u8g2.print(twoDigit(alarmMinute));
    u8g2.drawXBMP(118, 2, 8, 9, clockICON);

    //affichage de l'heure
    u8g2.setFont(u8g2_font_osr29_tn);
    u8g2.setCursor(10, 53);
    u8g2.print(twoDigit(RTClock.hour));
    u8g2.print(':');
    u8g2.print(twoDigit(RTClock.min));

  } while (u8g2.nextPage());

  delay(2000);

}

String twoDigit(int number) {
  if (number < 10) {  // If it's a single-digit number
    return "0" + String(number);  // Add a leading zero
  } else {
    return String(number);  // Otherwise, just return the number as is
  }
}
