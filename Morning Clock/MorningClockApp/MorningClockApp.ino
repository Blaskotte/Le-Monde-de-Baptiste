
#include <Arduino.h>
#include <U8g2lib.h>
#include <DS3231.h>
#include <Wire.h>
#include <SerialMP3Player.h>


U8G2_UC1701_MINI12864_2_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* cs=*/10, /* dc=*/9, /* reset=*/8);

//Defining pins
const int RotaryCLK = 2;  //CLK
const int RotaryDT = 4;   //DT
const int RotarySW = 3;   //SW (Button function)
SerialMP3Player mp3(/*RX*/ 6, /*TX*/ 7);

//Bitmaps
const unsigned char bigHappy_BM[] PROGMEM = {
  // 'bigHappy', 14x18px
  0xc3, 0x30, 0xc3, 0x30, 0x0c, 0x0c, 0x0c, 0x0c, 0xf0, 0x03, 0xf0, 0x03, 0xf0, 0x03, 0xf0, 0x03, 0xf0, 0x03, 0xf0,
  0x03, 0xf0, 0x03, 0xf0, 0x03, 0xf0, 0x03, 0xf0, 0x03, 0x30, 0x03, 0x30, 0x03, 0x30, 0x03, 0x30, 0x03
};
const unsigned char happyMenu_BM[] PROGMEM = {
  // 'happyMenu', 8x9px
  0x49, 0x22, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x14, 0x14
};
const unsigned char happyMenu_select_BM[] PROGMEM = {
  // 'happyMenu_select, 9x11px
  0xfe, 0x00, 0x6d, 0x01, 0xbb, 0x01, 0xc7, 0x01, 0xc7, 0x01, 0xc7, 0x01, 0xc7, 0x01, 0xc7, 0x01,
  0xd7, 0x01, 0xd7, 0x01, 0xfe, 0x00
};
const unsigned char alarmON_BM[] PROGMEM = {
  // 'alarmON, 8x9px
  0x42, 0xbd, 0x42, 0x89, 0x89, 0xb9, 0x81, 0x42, 0x3c
};
const unsigned char alarmON_select_BM[] PROGMEM = {
  // 'alarmON_select', 10x11px
  0xfe, 0x01, 0x7b, 0x03, 0x85, 0x02, 0x7b, 0x03, 0xed, 0x02, 0xed, 0x02, 0x8d, 0x02, 0xfd, 0x02,
  0x7b, 0x03, 0x87, 0x03, 0xfe, 0x01
};
const unsigned char chimeON_BM[] PROGMEM = {
  // 'chimeON, 8x9px
  0x18, 0x3c, 0x7e, 0x7e, 0x7e, 0x7e, 0xff, 0x00, 0x18
};
const unsigned char chimeON_select_BM[] PROGMEM = {
  // 'chimeON_select, 10x11px
  0xfe, 0x01, 0xcf, 0x03, 0x87, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x02,
  0xff, 0x03, 0xcf, 0x03, 0xfe, 0x01
};
const unsigned char alarmOFF_BM[] PROGMEM = {
  // 'alarmOFF, 8x11px
  0x01, 0x42, 0xbd, 0x46, 0x89, 0x99, 0xb9, 0xa1, 0x42, 0x7c, 0x80
};
const unsigned char alarmOFF_select_BM[] PROGMEM = {
  // 'alarmOFF_select', 10x11px
  0xfc, 0x01, 0x7b, 0x03, 0x85, 0x02, 0x73, 0x03, 0xed, 0x02, 0xcd, 0x02, 0x8d, 0x02, 0xbd, 0x02,
  0x7b, 0x03, 0x07, 0x03, 0xfe, 0x00
};
const unsigned char chimeOFF_BM[] PROGMEM = {
  // 'chimeOFF', 8x9px
  0x19, 0x3e, 0x7c, 0x7a, 0x76, 0x6e, 0xdf, 0x80, 0x18
};
const unsigned char chimeOFF_select_BM[] PROGMEM = {
  // 'chimeOFF_select', 10x11px
  0xfe, 0x01, 0xcd, 0x03, 0x83, 0x03, 0x07, 0x03, 0x0b, 0x03, 0x13, 0x03, 0x23, 0x03, 0x41, 0x02,
  0xff, 0x02, 0xcf, 0x03, 0xfe, 0x01
};
const unsigned char right_arrow_BM[] PROGMEM = {
  // 'right_arrow, 7x5px
  0x08, 0x10, 0x3f, 0x10, 0x08
};

// 'bigHappy', 14x18px

// 'happyMenu', 8x9px
// 'happyMenu_select, 9x11px

// 'alarmON, 8x9px
// 'alarmON_select', 10x11px

// 'chimeON, 8x9px
// 'chimeON_select, 10x11px

// 'alarmOFF, 8x11px
// 'alarmOFF_select', 10x11px

// 'chimeOFF', 8x9px
// 'chimeOFF_select', 10x11px

//Defining variables for rotary encoder and button
int ButtonCounter = 0;            //counts the button clicks
int RotateCounter = 0;            //counts the rotation clicks
bool buttonPressedState = false;  //info of the button

//Statuses (rotary encoder)
int CLKNow;
int CLKPrevious;
int DTNow;
int DTPrevious;

// Timers (rotary encoder)
float TimeNow1;
float TimeNow2;

//Defining variables for the state of the homePage
bool chime_is_activated = false;
bool alarm_is_activated = false;

bool chimeSelect = false;
bool alarmSelect = false;
bool happyMenuSelect = false;

bool homePage = true;
bool settingsPage = false;
bool alarmPage = false;
bool chimePage = false;
bool clockPage = false;
bool calendarPage = false;
bool volumePage = false;

unsigned int alarmHour = 0;
unsigned int alarmMinute = 0;

DS3231 rtc(SDA, SCL);
Time RTClock;

void setup() {
  Serial.begin(9600);  // start serial interface
  mp3.begin(9600);
  rtc.begin();
  u8g2.begin();
  u8g2.enableUTF8Print();
  mp3.sendCommand(CMD_SEL_DEV, 0, 2);  //select sd-card
  //setting up pins
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  //Store states
  CLKPrevious = digitalRead(RotaryCLK);
  DTPrevious = digitalRead(RotaryDT);

  attachInterrupt(digitalPinToInterrupt(RotaryCLK), wheel_is_rotated, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RotarySW), button_is_pressed, FALLING);

  int progress = 0;

  u8g2.firstPage();
  do {

    u8g2.firstPage();
    do {
      u8g2.drawXBMP(57, 14, 14, 18, bigHappy_BM);
      //u8g2.drawXBMP(61, 19, 7, 9, happyMenu_BM);
      u8g2.drawBox(23, 46, progress, 2);
      u8g2.drawFrame(21, 44, 84, 6);
    } while (u8g2.nextPage());

    delay(24);

    progress = progress + 1;

  } while (progress <= 80);

  mp3.setVol(30);
  mp3.play(1);
  delay(300);
  TimeNow1 = millis();  //Start timer 1
}

void loop() {

  RTClock = rtc.getTime();

  if (homePage == true && settingsPage == false) {
    updateHomePage();
    executeHomePage();
    printHomePage();
  }

  if (homePage == false && settingsPage == true) {
    updateSettingsPage();
    executeSettingsPage();
  }
}


void printHomePage() {

  u8g2.firstPage();
  do {

    u8g2.drawLine(0, 13, 128, 13);

    if (happyMenuSelect == true) {
      u8g2.drawXBMP(1, 1, 9, 11, happyMenu_select_BM);
    } else {
      u8g2.drawXBMP(2, 2, 7, 9, happyMenu_BM);
    }

    if (chime_is_activated == true) {
      if (chimeSelect == true) {
        u8g2.drawXBMP(27, 1, 10, 11, chimeON_select_BM);
      } else {
        u8g2.drawXBMP(28, 2, 8, 9, chimeON_BM);
      }

    } else {
      if (chimeSelect == true) {
        u8g2.drawXBMP(27, 1, 10, 11, chimeOFF_select_BM);
      } else {
        u8g2.drawXBMP(28, 2, 8, 9, chimeOFF_BM);
      }
    }

    if (alarm_is_activated == true) {
      if (alarmSelect == true) {
        u8g2.drawXBMP(16, 1, 10, 11, alarmON_select_BM);
      } else {
        u8g2.drawXBMP(17, 2, 8, 9, alarmON_BM);
      }

      // Display the time of the alarm
      u8g2.setFont(u8g2_font_profont11_tf);
      u8g2.setCursor(96, 10);
      u8g2.print(twoDigit(alarmHour));
      u8g2.print(':');
      u8g2.print(twoDigit(alarmMinute));

    } else {
      if (alarmSelect == true) {
        u8g2.drawXBMP(16, 1, 10, 11, alarmOFF_select_BM);
      } else {
        u8g2.drawXBMP(17, 1, 8, 11, alarmOFF_BM);
      }
    }

    u8g2.setFont(u8g2_font_timR24_tr);
    u8g2.setCursor(28, 51);
    u8g2.print(twoDigit(RTClock.hour));
    u8g2.print(':');
    u8g2.print(twoDigit(RTClock.min));

  } while (u8g2.nextPage());
}

void updateHomePage() {
  switch (RotateCounter) {
    case 1:
      RotateCounter = -3;
      break;
    case 0:
      chimeSelect = false;
      alarmSelect = false;
      happyMenuSelect = false;
      break;

    case -1:
      chimeSelect = true;
      alarmSelect = false;
      happyMenuSelect = false;
      break;

    case -2:
      chimeSelect = false;
      alarmSelect = true;
      happyMenuSelect = false;
      break;

    case -3:
      chimeSelect = false;
      alarmSelect = false;
      happyMenuSelect = true;
      break;

    case -4:
      RotateCounter = 0;
      break;

    default:
      RotateCounter = 0;
  }
}

void executeHomePage() {

  if (buttonPressedState == true) {
    switch (RotateCounter) {
      case 0:
        break;
      case -1:
        if (chime_is_activated == true) {
          chime_is_activated = false;
        } else {
          chime_is_activated = true;
        }
        break;

      case -2:
        if (alarm_is_activated == true) {
          alarm_is_activated = false;
        } else {
          alarm_is_activated = true;
        }
        break;

      case -3:
        homePage = false;
        settingsPage = true;
        RotateCounter = 1;
        break;
    }
  }
  buttonPressedState = false;  //reset this variable
}



void updateSettingsPage() {
  switch (RotateCounter) {
    case -10:
      RotateCounter = 1;
      break;
    case -2:
      RotateCounter = 1;
      break;
    case -1:
      RotateCounter = 1;
      break;
    case 0:
      RotateCounter = 1;
      break;

    case 1:
      u8g2.firstPage();
      do {
        u8g2.drawXBMP(2, 2, 7, 9, happyMenu_BM);
        u8g2.drawLine(0, 13, 127, 13);
        u8g2.drawXBMP(5, 22, 7, 5, right_arrow_BM);
        u8g2.setFont(u8g2_font_profont11_tf);
        u8g2.drawUTF8(40, 10, "Réglages");
        u8g2.drawUTF8(18, 28, "Réveil");
        u8g2.drawStr(15, 43, "Carillon");
        u8g2.drawStr(15, 58, "Horloge");
      } while (u8g2.nextPage());
      break;

    case 2:
      u8g2.firstPage();
      do {
        u8g2.drawXBMP(2, 2, 7, 9, happyMenu_BM);
        u8g2.drawLine(0, 13, 127, 13);
        u8g2.drawXBMP(5, 37, 7, 5, right_arrow_BM);
        u8g2.setFont(u8g2_font_profont11_tf);
        u8g2.drawUTF8(40, 10, "Réglages");
        u8g2.drawUTF8(15, 28, "Réveil");
        u8g2.drawStr(18, 43, "Carillon");
        u8g2.drawStr(15, 58, "Horloge");
      } while (u8g2.nextPage());
      break;

    case 3:
      u8g2.firstPage();
      do {
        u8g2.drawXBMP(2, 2, 7, 9, happyMenu_BM);
        u8g2.drawLine(0, 13, 127, 13);
        u8g2.drawXBMP(5, 37, 7, 5, right_arrow_BM);
        u8g2.setFont(u8g2_font_profont11_tf);
        u8g2.drawUTF8(40, 10, "Réglages");
        u8g2.drawStr(15, 28, "Carillon");
        u8g2.drawStr(18, 43, "Horloge");
        u8g2.drawStr(15, 58, "Calendrier");
      } while (u8g2.nextPage());
      break;

    case 4:
      u8g2.firstPage();
      do {
        u8g2.drawXBMP(2, 2, 7, 9, happyMenu_BM);
        u8g2.drawLine(0, 13, 127, 13);
        u8g2.drawXBMP(5, 37, 7, 5, right_arrow_BM);
        u8g2.setFont(u8g2_font_profont11_tf);
        u8g2.drawUTF8(40, 10, "Réglages");
        u8g2.drawStr(15, 28, "Horloge");
        u8g2.drawStr(18, 43, "Calendrier");
        u8g2.drawStr(15, 58, "Volume");
      } while (u8g2.nextPage());
      break;

    case 5:
      u8g2.firstPage();
      do {
        u8g2.drawXBMP(2, 2, 7, 9, happyMenu_BM);
        u8g2.drawLine(0, 13, 127, 13);
        u8g2.drawXBMP(5, 37, 7, 5, right_arrow_BM);
        u8g2.setFont(u8g2_font_profont11_tf);
        u8g2.drawUTF8(40, 10, "Réglages");
        u8g2.drawStr(15, 28, "Calendrier");
        u8g2.drawStr(18, 43, "Volume");
        u8g2.drawStr(15, 58, "Retour");
      } while (u8g2.nextPage());
      break;

    case 6:
      u8g2.firstPage();
      do {
        u8g2.drawXBMP(2, 2, 7, 9, happyMenu_BM);
        u8g2.drawLine(0, 13, 127, 13);
        u8g2.drawXBMP(5, 52, 7, 5, right_arrow_BM);
        u8g2.setFont(u8g2_font_profont11_tf);
        u8g2.drawUTF8(40, 10, "Réglages");
        u8g2.drawStr(15, 28, "Calendrier");
        u8g2.drawStr(15, 43, "Volume");
        u8g2.drawStr(18, 58, "Retour");
      } while (u8g2.nextPage());
      break;

    case 7:
      RotateCounter = 6;
      break;
    case 8:
      RotateCounter = 6;
      break;
    case 9:
      RotateCounter = 6;
      break;
    case 16:
      RotateCounter = 6;
      break;
  }
}

void executeSettingsPage() {

  if (buttonPressedState == true) {
    switch (RotateCounter) {
    case 1:

      break;

    case 2:

      break;

    case 3:

      break;

    case 4:

      break;

    case 5:

      break;

    case 6:
      settingsPage = false;
      homePage = true;
      RotateCounter = 0;
      break;
    }
  }
  buttonPressedState = false;  //reset this variable
}


void button_is_pressed() {
  TimeNow2 = millis();
  if (TimeNow2 - TimeNow1 > 200) {
    buttonPressedState = true;
  }
  TimeNow1 = millis();  //"reset" timer; the next 500 ms is counted from this moment
}

void wheel_is_rotated() {

  CLKNow = digitalRead(RotaryCLK);  //Read the state of the CLK pin

  // If last and current state of CLK are different, then a pulse occurred
  if (CLKNow != CLKPrevious && CLKNow == 1) {
    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so increase
    if (digitalRead(RotaryDT) != CLKNow) {
      RotateCounter++;
      delay(200);
    } else {
      RotateCounter--;
      delay(200);
    }
  }

  CLKPrevious = CLKNow;  // Store last CLK state
}


String twoDigit(int number) {
  if (number < 10) {              // If it's a single-digit number
    return "0" + String(number);  // Add a leading zero
  } else {
    return String(number);  // Otherwise, just return the number as is
  }
}