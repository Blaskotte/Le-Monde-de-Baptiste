
#include <Arduino.h>
#include <U8g2lib.h>
#include <RtcDS1302.h>
#include <ThreeWire.h>

ThreeWire myWire(A0, A1, 3);       // DAT, CLK, RST (fils pour le RTC)
RtcDS1302<ThreeWire> Rtc(myWire);  // RTC Object

U8G2_UC1701_MINI12864_2_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* cs=*/10, /* dc=*/9, /* reset=*/8);

void setup() {

  u8g2.begin();

  u8g2.enableUTF8Print();
}

void loop() {

  RtcDateTime now = Rtc.GetDateTime();

  u8g2.firstPage();
  do {
    //u8g2.drawRFrame(0, 0, 128, 64, 2);
    u8g2.setFontMode(1);  /* activate transparent font mode */
    u8g2.setDrawColor(1); /* color 1 for the box */
    u8g2.setFont(u8g2_font_profont12_mn);
    u8g2.setFontDirection(0);
    u8g2.drawStr(96, 10, "08:15");
    u8g2.drawLine(0, 11, 128, 11);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(1, 10, 0xE04F); /* dec 9731/hex 2603 Snowman */
    u8g2.drawRFrame(6, 48, 116, 40, 4);

    u8g2.drawGlyph(9, 60, 0xE0B2); /* dec 9731/hex 2603 Snowman */
    u8g2.drawRFrame(9, 50, 12, 12, 2);
    u8g2.drawGlyph(24, 60, 0xE272); /* dec 9731/hex 2603 Snowman */
    u8g2.drawRFrame(23, 50, 12, 12, 2);
    u8g2.drawRFrame(37, 50, 12, 12, 2);
    u8g2.drawRFrame(51, 50, 12, 12, 2);
    u8g2.drawRFrame(65, 50, 12, 12, 2);
    u8g2.drawRFrame(79, 50, 12, 12, 2);
    u8g2.drawRFrame(93, 50, 12, 12, 2);
    u8g2.drawRFrame(107, 50, 12, 12, 2);


    u8g2.setFont(u8g2_font_chargen_92_mn);
    u8g2.setCursor(34, 37);
    if (now.Hour() >= 0 && now.Hour() < 10) {
      u8g2.print('0');
    }
    u8g2.print(now.Hour(), DEC);
    u8g2.print(' ');
    if (now.Minute() >= 0 && now.Minute() < 10) {
      u8g2.print('0');
    }
    u8g2.print(now.Minute(), DEC);
    u8g2.drawLine(0, 11, 128, 11);


  } while (u8g2.nextPage());

  delay(1000);

  u8g2.firstPage();
  do {
    //u8g2.drawRFrame(0, 0, 128, 64, 2);
    u8g2.setFontMode(1);  /* activate transparent font mode */
    u8g2.setDrawColor(1); /* color 1 for the box */
    u8g2.setFont(u8g2_font_profont12_mn);
    u8g2.setFontDirection(0);
    u8g2.drawStr(96, 10, "08:15");
    u8g2.drawLine(0, 11, 128, 11);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(1, 10, 0xE04F); /* dec 9731/hex 2603 Snowman */
    u8g2.drawRFrame(6, 48, 116, 40, 4);

    u8g2.drawGlyph(9, 60, 0xE0B2); /* dec 9731/hex 2603 Snowman */
    u8g2.drawRFrame(9, 50, 12, 12, 2);
    u8g2.drawGlyph(24, 60, 0xE272); /* dec 9731/hex 2603 Snowman */
    u8g2.drawRFrame(23, 50, 12, 12, 2);
    u8g2.drawRFrame(37, 50, 12, 12, 2);
    u8g2.drawRFrame(51, 50, 12, 12, 2);
    u8g2.drawRFrame(65, 50, 12, 12, 2);
    u8g2.drawRFrame(79, 50, 12, 12, 2);
    u8g2.drawRFrame(93, 50, 12, 12, 2);
    u8g2.drawRFrame(107, 50, 12, 12, 2);


    u8g2.setFont(u8g2_font_chargen_92_mn);
    u8g2.setCursor(34, 37);
    if (now.Hour() >= 0 && now.Hour() < 10) {
      u8g2.print('0');
    }
    u8g2.print(now.Hour(), DEC);
    u8g2.print(':');
    if (now.Minute() >= 0 && now.Minute() < 10) {
      u8g2.print('0');
    }
    u8g2.print(now.Minute(), DEC);
    u8g2.drawLine(0, 11, 128, 11);


  } while (u8g2.nextPage());

  delay(1000);
}
