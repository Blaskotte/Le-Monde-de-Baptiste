
#include <Arduino.h>
#include <U8g2lib.h>
#include <DS3231.h>
#include <Wire.h>
#include <SerialMP3Player.h>

//prototype display
U8G2_ST7571_128X128_2_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* cs=*/10, /* dc=*/9, /* reset=*/8);

//final display
//U8G2_UC1701_MINI12864_2_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* cs=*/10, /* dc=*/9, /* reset=*/8);

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
const unsigned char clock_BM[] PROGMEM = {
  // 'clock, 8x9px
  0x3c, 0x42, 0x89, 0x89, 0xb9, 0x81, 0x42, 0x3c, 0x3c
};
const unsigned char clock_select_BM[] PROGMEM = {
  // 'clock_select, 10x11px
  0xfe, 0x01, 0x87, 0x03, 0x7b, 0x03, 0xed, 0x02, 0xed, 0x02, 0x8d, 0x02, 0xfd, 0x02, 0x7b, 0x03,
  0x87, 0x03, 0x87, 0x03, 0xfe, 0x01
};
const unsigned char calendar_BM[] PROGMEM = {
  // 'calendar_BM, 8x9px
  0x42, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff
};
const unsigned char calendar_select_BM[] PROGMEM = {
  // 'calendar_select_BM, 10x11px
  0xfe, 0x01, 0x7b, 0x03, 0x01, 0x02, 0x01, 0x02, 0xfd, 0x02, 0xfd, 0x02, 0xfd, 0x02, 0xfd, 0x02,
  0xfd, 0x02, 0x01, 0x02, 0xfe, 0x01
};
const unsigned char volume_BM[] PROGMEM = {
  // 'volume_BM, 8x7px
  0x08, 0x4c, 0x8f, 0xaf, 0x8f, 0x4c, 0x08
};
const unsigned char volume_select_BM[] PROGMEM = {
  // 'volume_select_BM, 10x11px
  0xfe, 0x01, 0xff, 0x03, 0xef, 0x03, 0x67, 0x03, 0xe1, 0x02, 0xa1, 0x02, 0xe1, 0x02, 0x67, 0x03,
  0xef, 0x03, 0xff, 0x03, 0xfe, 0x01
};
const unsigned char return_BM[] PROGMEM = {
  // 'return_BM, 8x7px
  0x08, 0x0c, 0x7e, 0xff, 0xfe, 0xcc, 0x88
};
const unsigned char return_select_BM[] PROGMEM = {
  // 'return_select_BM, 10x11px
  0xfe, 0x01, 0xff, 0x03, 0xef, 0x03, 0xe7, 0x03, 0x03, 0x03, 0x01, 0x02, 0x03, 0x02, 0x67, 0x02,
  0xef, 0x02, 0xff, 0x03, 0xfe, 0x01
};

// 'bigHappy_BM', 14x18px

// 'happyMenu_BM', 8x9px
// 'happyMenu_select_BM, 9x11px

// 'alarmON_BM, 8x9px
// 'alarmON_select_BM', 10x11px

// 'chimeON_BM, 8x9px
// 'chimeON_select_BM, 10x11px

// 'alarmOFF_BM, 8x11px
// 'alarmOFF_select_BM', 10x11px

// 'chimeOFF_BM', 8x9px
// 'chimeOFF_select_BM', 10x11px

// 'clock_BM, 8x9px
// 'clock_select_BM, 10x11px

// 'calendar_BM, 8x9px
// 'calendar_select_BM, 10x11px

// 'volume_BM, 8x7px
// 'volume_select_BM, 10x11px

// 'return_BM, 8x7px
// 'return_select_BM, 10x11px

// 'right_arrow_BM, 7x5px

//Defining variables for rotary encoder and button
int rotateCounter = 0;            //counts the rotation clicks
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
bool alarmSetPage = false;
bool chimeSetPage = false;
bool calendarSetPage = false;

int alarmMusic = 2;
int chimeMusic = 4;

unsigned int alarmHour = 0;
unsigned int alarmMinute = 0;

unsigned int temporaryHour = 0;
unsigned int temporaryMinute = 0;
unsigned int temporarySecond = 0;

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
      u8g2.drawLine(0, 64, 127, 64);
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

  testAlarmChime();

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


//main page
void printHomePage() {

  u8g2.firstPage();
  do {
    drawMenuLine();

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
  switch (rotateCounter) {
    case 1:
      rotateCounter = -3;
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
      rotateCounter = 0;
      break;

    default:
      rotateCounter = 0;
  }
}
void executeHomePage() {

  if (buttonPressedState == true) {
    switch (rotateCounter) {
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
        alarmSet_state();
        break;

      case -3:
        homePage = false;
        settingsPage = true;
        rotateCounter = 1;
        break;
    }
  }
  buttonPressedState = false;  //reset this variable
}


//main settings page
void updateSettingsPage() {
  testAlarmChime();
  switch (rotateCounter) {
    case -10:
      rotateCounter = 1;
      break;
    case -2:
      rotateCounter = 1;
      break;
    case -1:
      rotateCounter = 1;
      break;
    case 0:
      rotateCounter = 1;
      break;

    case 1:
      u8g2.firstPage();
      do {
        drawSettingsBar();
        u8g2.drawXBMP(5, 22, 7, 5, right_arrow_BM);
        u8g2.drawUTF8(18, 28, "Réveil");
        u8g2.drawXBMP(113, 19, 10, 11, alarmON_select_BM);
        u8g2.drawStr(15, 43, "Carillon");
        u8g2.drawXBMP(114, 35, 8, 9, chimeON_BM);
        u8g2.drawStr(15, 58, "Horloge");
        u8g2.drawXBMP(114, 50, 8, 9, clock_BM);
      } while (u8g2.nextPage());
      break;

    case 2:
      u8g2.firstPage();
      do {
        drawSettingsBar();
        u8g2.drawXBMP(5, 37, 7, 5, right_arrow_BM);
        u8g2.drawUTF8(15, 28, "Réveil");
        u8g2.drawXBMP(114, 20, 8, 9, alarmON_BM);
        u8g2.drawStr(18, 43, "Carillon");
        u8g2.drawXBMP(113, 34, 10, 11, chimeON_select_BM);
        u8g2.drawStr(15, 58, "Horloge");
        u8g2.drawXBMP(114, 50, 8, 9, clock_BM);
      } while (u8g2.nextPage());
      break;

    case 3:
      u8g2.firstPage();
      do {
        drawSettingsBar();
        u8g2.drawXBMP(5, 37, 7, 5, right_arrow_BM);
        u8g2.drawStr(15, 28, "Carillon");
        u8g2.drawXBMP(114, 20, 8, 9, chimeON_BM);
        u8g2.drawStr(18, 43, "Horloge");
        u8g2.drawXBMP(113, 34, 10, 11, clock_select_BM);
        u8g2.drawStr(15, 58, "Calendrier");
        u8g2.drawXBMP(114, 50, 8, 9, calendar_BM);
      } while (u8g2.nextPage());
      break;

    case 4:
      u8g2.firstPage();
      do {
        drawSettingsBar();
        u8g2.drawXBMP(5, 37, 7, 5, right_arrow_BM);
        u8g2.drawStr(15, 28, "Horloge");
        u8g2.drawXBMP(114, 20, 8, 9, clock_BM);
        u8g2.drawStr(18, 43, "Calendrier");
        u8g2.drawXBMP(113, 34, 10, 11, calendar_select_BM);
        u8g2.drawStr(15, 58, "Volume");
        u8g2.drawXBMP(114, 51, 8, 7, volume_BM);
      } while (u8g2.nextPage());
      break;

    case 5:
      u8g2.firstPage();
      do {
        drawSettingsBar();
        u8g2.drawXBMP(5, 37, 7, 5, right_arrow_BM);
        u8g2.drawStr(15, 28, "Calendrier");
        u8g2.drawXBMP(114, 20, 8, 9, calendar_BM);
        u8g2.drawStr(18, 43, "Volume");
        u8g2.drawXBMP(113, 34, 10, 11, volume_select_BM);
        u8g2.drawStr(15, 58, "Retour");
        u8g2.drawXBMP(114, 51, 8, 7, return_BM);
      } while (u8g2.nextPage());
      break;

    case 6:
      u8g2.firstPage();
      do {
        drawSettingsBar();
        u8g2.drawXBMP(5, 52, 7, 5, right_arrow_BM);
        u8g2.drawStr(15, 28, "Calendrier");
        u8g2.drawXBMP(114, 20, 8, 9, calendar_BM);
        u8g2.drawStr(15, 43, "Volume");
        u8g2.drawXBMP(114, 36, 8, 7, volume_BM);
        u8g2.drawStr(18, 58, "Retour");
        u8g2.drawXBMP(113, 49, 10, 11, return_select_BM);
      } while (u8g2.nextPage());
      break;

    case 7:
      rotateCounter = 6;
      break;
    case 8:
      rotateCounter = 6;
      break;
    case 9:
      rotateCounter = 6;
      break;
    case 16:
      rotateCounter = 6;
      break;
  }
}
void executeSettingsPage() {

  if (buttonPressedState == true) {
    switch (rotateCounter) {
      case 1:
        updateAlarmPage();
        rotateCounter = 1;
        break;

      case 2:
        break;

      case 3:
        updateClockPage();
        rotateCounter = 3;
        break;

      case 4:
        break;

      case 5:
        updateClockPage();
        break;

      case 6:
        settingsPage = false;
        homePage = true;
        rotateCounter = 0;
        break;
    }
  }
  buttonPressedState = false;  //reset this variable
}

//alarm page
void updateAlarmPage() {
  alarmSetPage = true;
  rotateCounter = 1;
  buttonPressedState = false;
  do {
    switch (rotateCounter) {
      case -10:
        rotateCounter = 1;
        break;
      case -2:
        rotateCounter = 1;
        break;
      case -1:
        rotateCounter = 1;
        break;
      case 0:
        rotateCounter = 1;
        break;

      case 1:

        u8g2.firstPage();
        do {
          drawAlarmMenu();
          if (alarm_is_activated == true) {
            u8g2.drawUTF8(18, 28, "Désactiver");
          } else {
            u8g2.drawStr(18, 28, "Activer");
          }
          u8g2.drawStr(15, 43, "Heure");
          u8g2.drawStr(15, 58, "Musique");
          u8g2.drawXBMP(5, 22, 7, 5, right_arrow_BM);
        } while (u8g2.nextPage());
        break;

      case 2:
        u8g2.firstPage();
        do {
          drawAlarmMenu();
          if (alarm_is_activated == true) {
            u8g2.drawUTF8(15, 28, "Désactiver");
          } else {
            u8g2.drawStr(15, 28, "Activer");
          }
          u8g2.drawStr(18, 43, "Heure");
          u8g2.drawStr(15, 58, "Musique");
          u8g2.drawXBMP(5, 37, 7, 5, right_arrow_BM);
        } while (u8g2.nextPage());
        break;

      case 3:
        u8g2.firstPage();
        do {
          drawAlarmMenu();
          u8g2.drawStr(15, 28, "Heure");
          u8g2.drawStr(18, 43, "Musique");
          u8g2.drawStr(15, 58, "Retour");
          u8g2.drawXBMP(5, 37, 7, 5, right_arrow_BM);
        } while (u8g2.nextPage());
        break;

      case 4:
        u8g2.firstPage();
        do {
          drawAlarmMenu();
          u8g2.drawStr(15, 28, "Heure");
          u8g2.drawStr(15, 43, "Musique");
          u8g2.drawStr(18, 58, "Retour");
          u8g2.drawXBMP(5, 52, 7, 5, right_arrow_BM);
        } while (u8g2.nextPage());
        break;

      case 5:
        rotateCounter = 4;
        break;
      case 6:
        rotateCounter = 4;
        break;
      case 7:
        rotateCounter = 4;
        break;
      case 14:
        rotateCounter = 4;
        break;
    }
    executeAlarmSetPage();
  } while (alarmSetPage == true);
}
void executeAlarmSetPage() {
  if (buttonPressedState == true) {
    switch (rotateCounter) {
      case 1:
        alarmSet_state();
        break;

      case 2:
        alarmSet_hour();
        alarmSetPage = false;
        break;

      case 3:
        alarmSet_music();
        alarmSetPage = false;
        break;

      case 4:
        alarmSetPage = false;
        rotateCounter = 1;
        break;
    }
    buttonPressedState = false;  //reset this variable
  }
}

//alarm page submenus
void alarmSet_state() {
  if (alarm_is_activated == true) {
    alarm_is_activated = false;
    mp3.stop();
  } else {
    alarm_is_activated = true;
  }
  buttonPressedState = false;
}
void alarmSet_hour() {
  bool alarmSetHour_page = true;
  int alarmSetStep = 1;
  rotateCounter = 0;
  buttonPressedState = false;
  alarmHour = 0;
  alarmMinute = 0;

  do {
    u8g2.firstPage();
    do {
      drawAlarmMenu();
      u8g2.setFont(u8g2_font_timR24_tr);
      u8g2.setCursor(28, 44);
      u8g2.print(twoDigit(alarmHour));
      u8g2.print(':');
      u8g2.print(twoDigit(alarmMinute));
      switch (alarmSetStep) {
        case 1:
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawStr(34, 57, "Heure(s) ?");
          break;

        case 2:
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawStr(31, 57, "Minute(s) ?");
          break;

        case 3:
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawUTF8(25, 57, "Enregistrée !");
          break;
      }

    } while (u8g2.nextPage());

    switch (alarmSetStep) {

      case 1:  //Setting hours
        if (rotateCounter >= 24) {
          rotateCounter = 0;
        }
        if (rotateCounter < 0) {
          rotateCounter = 23;
        }
        alarmHour = rotateCounter;
        if (buttonPressedState == true) {
          rotateCounter = 0;
          buttonPressedState = false;
          alarmSetStep = 2;
        }
        break;

      case 2:  //Setting minutes
        if (rotateCounter >= 60) {
          rotateCounter = 0;
        }
        if (rotateCounter < 0) {
          rotateCounter = 59;
        }
        alarmMinute = rotateCounter;
        if (buttonPressedState == true) {
          rotateCounter = 0;
          buttonPressedState = false;
          alarmSetStep = 3;
        }
        break;

      case 3:
        mp3.play(5);
        alarmSetHour_page = false;
        delay(500);
        buttonPressedState = false;
        rotateCounter = 2;
        break;
    }
  } while (alarmSetHour_page == true);
}
void alarmSet_music() {
  bool alarmSetMusic_page = true;
  int alarmSetStep = 1;
  rotateCounter = 2;
  buttonPressedState = false;

  do {
    u8g2.firstPage();
    do {
      drawAlarmMenu();
      switch (alarmSetStep) {
        case 1:
          if (alarmMusic == 2) {
            u8g2.setFont(u8g2_font_profont11_tf);
            u8g2.drawStr(18, 34, "Grieg : Peer Gynt");
            u8g2.drawStr(15, 52, "Haydn : Concerto");
            u8g2.drawXBMP(5, 28, 7, 5, right_arrow_BM);
          }
          if (alarmMusic == 3) {
            u8g2.setFont(u8g2_font_profont11_tf);
            u8g2.drawStr(15, 34, "Grieg : Peer Gynt");
            u8g2.drawStr(18, 52, "Haydn : Concerto");
            u8g2.drawXBMP(5, 46, 7, 5, right_arrow_BM);
          }
          break;

        case 2:
          if (rotateCounter == 1) {
            u8g2.drawStr(25, 29, "Ce choix vous");
            u8g2.drawStr(25, 41, "convient-il ?");
            u8g2.drawStr(34, 56, "Oui");
            u8g2.drawStr(77, 56, "Non");
            u8g2.setDrawColor(2);
            u8g2.drawBox(31, 46, 22, 13);
            u8g2.setDrawColor(1);
          }
          if (rotateCounter == 2) {
            u8g2.drawStr(25, 29, "Ce choix vous");
            u8g2.drawStr(25, 41, "convient-il ?");
            u8g2.drawStr(34, 56, "Oui");
            u8g2.drawStr(77, 56, "Non");
            u8g2.setDrawColor(2);
            u8g2.drawBox(74, 46, 23, 13);
            u8g2.setDrawColor(1);
          }
          break;

        case 3:
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawStr(16, 35, "Choix de musique");
          u8g2.drawUTF8(28, 48, "enregistré !");
          break;
      }

    } while (u8g2.nextPage());

    switch (alarmSetStep) {

      case 1:  //Selection of the music
        if (rotateCounter < 2) {
          rotateCounter = 2;
        }
        if (rotateCounter > 3) {
          rotateCounter = 3;
        }
        alarmMusic = rotateCounter;
        if (buttonPressedState == true) {
          buttonPressedState = false;
          alarmSetStep = 2;
          rotateCounter = 1;
          mp3.play(alarmMusic);
        }
        break;

      case 2:  //Validate the choice
        if (rotateCounter < 1) {
          rotateCounter = 1;
        }
        if (rotateCounter > 2) {
          rotateCounter = 2;
        }
        if (buttonPressedState == true && rotateCounter == 1) {
          buttonPressedState = false;
          alarmSetStep = 3;
        }
        if (buttonPressedState == true && rotateCounter == 2) {
          buttonPressedState = false;
          alarmSetStep = 1;
          mp3.stop();
        }
        break;

      case 3:
        mp3.play(5);
        alarmSetMusic_page = false;
        delay(500);
        buttonPressedState = false;
        rotateCounter = 1;
        break;
    }
  } while (alarmSetMusic_page == true);
}










//chime page
/*void updateChimePage() {
  chimeSetPage = true;
  rotateCounter = 1;
  buttonPressedState = false;
  do {
    switch (rotateCounter) {
      case -10:
        rotateCounter = 1;
        break;
      case -2:
        rotateCounter = 1;
        break;
      case -1:
        rotateCounter = 1;
        break;
      case 0:
        rotateCounter = 1;
        break;

      case 1:

        u8g2.firstPage();
        do {
          drawAlarmMenu();
          if (chime_is_activated == true) {
            u8g2.drawUTF8(18, 28, "Désactiver");
          } else {
            u8g2.drawStr(18, 28, "Activer");
          }
          u8g2.drawStr(15, 43, "Heure");
          u8g2.drawStr(15, 58, "Musique");
          u8g2.drawXBMP(5, 22, 7, 5, right_arrow_BM);
        } while (u8g2.nextPage());
        break;

      case 2:
        u8g2.firstPage();
        do {
          drawAlarmMenu();
          if (alarm_is_activated == true) {
            u8g2.drawUTF8(15, 28, "Désactiver");
          } else {
            u8g2.drawStr(15, 28, "Activer");
          }
          u8g2.drawStr(18, 43, "Heure");
          u8g2.drawStr(15, 58, "Musique");
          u8g2.drawXBMP(5, 37, 7, 5, right_arrow_BM);
        } while (u8g2.nextPage());
        break;

      case 3:
        u8g2.firstPage();
        do {
          drawAlarmMenu();
          u8g2.drawStr(15, 28, "Heure");
          u8g2.drawStr(18, 43, "Musique");
          u8g2.drawStr(15, 58, "Retour");
          u8g2.drawXBMP(5, 37, 7, 5, right_arrow_BM);
        } while (u8g2.nextPage());
        break;

      case 4:
        u8g2.firstPage();
        do {
          drawAlarmMenu();
          u8g2.drawStr(15, 28, "Heure");
          u8g2.drawStr(15, 43, "Musique");
          u8g2.drawStr(18, 58, "Retour");
          u8g2.drawXBMP(5, 52, 7, 5, right_arrow_BM);
        } while (u8g2.nextPage());
        break;

      case 5:
        rotateCounter = 4;
        break;
      case 6:
        rotateCounter = 4;
        break;
      case 7:
        rotateCounter = 4;
        break;
      case 14:
        rotateCounter = 4;
        break;
    }
    executeAlarmSetPage();
  } while (alarmSetPage == true);
}
void executeAlarmSetPage() {
  if (buttonPressedState == true) {
    switch (rotateCounter) {
      case 1:
        alarmSet_state();
        break;

      case 2:
        alarmSet_hour();
        alarmSetPage = false;
        break;

      case 3:
        alarmSet_music();
        alarmSetPage = false;
        break;

      case 4:
        alarmSetPage = false;
        rotateCounter = 1;
        break;
    }
    buttonPressedState = false;  //reset this variable
  }
}

//alarm page submenus
void alarmSet_state() {
  if (alarm_is_activated == true) {
    alarm_is_activated = false;
    mp3.stop();
  } else {
    alarm_is_activated = true;
  }
  buttonPressedState = false;
}
void alarmSet_hour() {
  bool alarmSetHour_page = true;
  int alarmSetStep = 1;
  rotateCounter = 0;
  buttonPressedState = false;
  alarmHour = 0;
  alarmMinute = 0;

  do {
    u8g2.firstPage();
    do {
      drawAlarmMenu();
      u8g2.setFont(u8g2_font_timR24_tr);
      u8g2.setCursor(28, 44);
      u8g2.print(twoDigit(alarmHour));
      u8g2.print(':');
      u8g2.print(twoDigit(alarmMinute));
      switch (alarmSetStep) {
        case 1:
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawStr(34, 57, "Heure(s) ?");
          break;

        case 2:
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawStr(31, 57, "Minute(s) ?");
          break;

        case 3:
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawUTF8(28, 57, "Enregistré !");
          break;
      }

    } while (u8g2.nextPage());

    switch (alarmSetStep) {

      case 1:  //Setting hours
        if (rotateCounter >= 24) {
          rotateCounter = 0;
        }
        if (rotateCounter < 0) {
          rotateCounter = 23;
        }
        alarmHour = rotateCounter;
        if (buttonPressedState == true) {
          rotateCounter = 0;
          buttonPressedState = false;
          alarmSetStep = 2;
        }
        break;

      case 2:  //Setting minutes
        if (rotateCounter >= 60) {
          rotateCounter = 0;
        }
        if (rotateCounter < 0) {
          rotateCounter = 59;
        }
        alarmMinute = rotateCounter;
        if (buttonPressedState == true) {
          rotateCounter = 0;
          buttonPressedState = false;
          alarmSetStep = 3;
        }
        break;

      case 3:
        mp3.play(5);
        alarmSetHour_page = false;
        delay(500);
        buttonPressedState = false;
        rotateCounter = 2;
        break;
    }
  } while (alarmSetHour_page == true);
}
void alarmSet_music() {
  bool alarmSetMusic_page = true;
  int alarmSetStep = 1;
  rotateCounter = 2;
  buttonPressedState = false;

  do {
    u8g2.firstPage();
    do {
      drawAlarmMenu();
      switch (alarmSetStep) {
        case 1:
          if (alarmMusic == 2) {
            u8g2.setFont(u8g2_font_profont11_tf);
            u8g2.drawStr(18, 34, "Grieg : Peer Gynt");
            u8g2.drawStr(15, 52, "Haydn : Concerto");
            u8g2.drawXBMP(5, 28, 7, 5, right_arrow_BM);
          }
          if (alarmMusic == 3) {
            u8g2.setFont(u8g2_font_profont11_tf);
            u8g2.drawStr(15, 34, "Grieg : Peer Gynt");
            u8g2.drawStr(18, 52, "Haydn : Concerto");
            u8g2.drawXBMP(5, 46, 7, 5, right_arrow_BM);
          }
          break;

        case 2:
          if (rotateCounter == 1) {
            u8g2.drawStr(25, 29, "Ce choix vous");
            u8g2.drawStr(25, 41, "convient-il ?");
            u8g2.drawStr(34, 56, "Oui");
            u8g2.drawStr(77, 56, "Non");
            u8g2.setDrawColor(2);
            u8g2.drawBox(31, 46, 22, 13, 1);
            u8g2.setDrawColor(1);
          }
          if (rotateCounter == 2) {
            u8g2.drawStr(25, 29, "Ce choix vous");
            u8g2.drawStr(25, 41, "convient-il ?");
            u8g2.drawStr(34, 56, "Oui");
            u8g2.drawStr(77, 56, "Non");
            u8g2.setDrawColor(2);
            u8g2.drawBox(74, 46, 23, 13, 1);
            u8g2.setDrawColor(1);
          }
          break;

        case 3:
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawStr(16, 35, "Choix de musique");
          u8g2.drawUTF8(28, 48, "enregistré !");
          break;
      }

    } while (u8g2.nextPage());

    switch (alarmSetStep) {

      case 1:  //Selection of the music
        if (rotateCounter < 2) {
          rotateCounter = 2;
        }
        if (rotateCounter > 3) {
          rotateCounter = 3;
        }
        alarmMusic = rotateCounter;
        if (buttonPressedState == true) {
          buttonPressedState = false;
          alarmSetStep = 2;
          rotateCounter = 1;
        }
        break;

      case 2:  //Validate the choice
        if (rotateCounter < 1) {
          rotateCounter = 1;
        }
        if (rotateCounter > 2) {
          rotateCounter = 2;
        }
        if (buttonPressedState == true && rotateCounter == 1) {
          buttonPressedState = false;
          alarmSetStep = 3;
        }
        if (buttonPressedState == true && rotateCounter == 2) {
          buttonPressedState = false;
          alarmSetStep = 1;
        }
        break;

      case 3:
        mp3.play(5);
        alarmSetMusic_page = false;
        delay(500);
        buttonPressedState = false;
        rotateCounter = 1;
        break;
    }
  } while (alarmSetMusic_page == true);
}*/










//clock page
void updateClockPage() {
  bool clockSet_page = true;
  int clockSetStep = 1;
  temporaryHour = 0;
  temporaryMinute = 0;
  temporarySecond = 0;
  rotateCounter = 0;
  buttonPressedState = false;

  do {
    u8g2.firstPage();
    do {
      u8g2.drawXBMP(2, 2, 8, 9, clock_BM);
      drawMenuLine();
      u8g2.setFont(u8g2_font_profont11_tf);
      u8g2.drawUTF8(43, 10, "Horloge");
      u8g2.setFont(u8g2_font_timR24_tr);
      u8g2.setCursor(7, 44);
      u8g2.print(twoDigit(temporaryHour));
      u8g2.print(':');
      u8g2.print(twoDigit(temporaryMinute));
      u8g2.print(':');
      u8g2.print(twoDigit(temporarySecond));
      switch (clockSetStep) {
        case 1:
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawStr(34, 57, "Heure(s) ?");
          break;

        case 2:
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawStr(31, 57, "Minute(s) ?");
          break;

        case 3:
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawStr(28, 57, "Seconde(s) ?");
          break;
        case 4:
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawUTF8(25, 57, "Enregistrée !");
          break;
      }

    } while (u8g2.nextPage());

    switch (clockSetStep) {

      case 1:  //Setting hours
        if (rotateCounter >= 24) {
          rotateCounter = 0;
        }
        if (rotateCounter < 0) {
          rotateCounter = 23;
        }
        temporaryHour = rotateCounter;
        if (buttonPressedState == true) {
          rotateCounter = 0;
          buttonPressedState = false;
          clockSetStep = 2;
        }
        break;

      case 2:  //Setting minutes
        if (rotateCounter >= 60) {
          rotateCounter = 0;
        }
        if (rotateCounter < 0) {
          rotateCounter = 59;
        }
        temporaryMinute = rotateCounter;
        if (buttonPressedState == true) {
          rotateCounter = 0;
          buttonPressedState = false;
          clockSetStep = 3;
        }
        break;

      case 3:  //Setting seconds
        if (rotateCounter >= 60) {
          rotateCounter = 0;
        }
        if (rotateCounter < 0) {
          rotateCounter = 59;
        }
        temporarySecond = rotateCounter;
        if (buttonPressedState == true) {
          rotateCounter = 0;
          buttonPressedState = false;
          clockSetStep = 4;
        }
        break;

      case 4:
        mp3.play(5);
        rtc.setTime(temporaryHour, temporaryMinute, temporarySecond);
        clockSet_page = false;
        delay(500);
        break;
    }
  } while (clockSet_page == true);
}

//volume page
void updateVolumePage() {
  bool volumeSetPage = true;
  do {
    volumeSetPage = false;
  } while (volumeSetPage == true);
}



void drawSettingsBar() {
  drawMenuLine();
  u8g2.drawXBMP(2, 2, 7, 9, happyMenu_BM);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawUTF8(40, 10, "Réglages");
}

void drawMenuLine() {
  u8g2.drawLine(0, 13, 128, 13);
  u8g2.drawLine(0, 64, 127, 64);
}

void drawAlarmMenu() {
  drawMenuLine();
  u8g2.drawUTF8(46, 10, "Réveil");
  u8g2.drawXBMP(2, 2, 8, 9, alarmON_BM);
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
      rotateCounter++;
      delay(200);
    } else {
      rotateCounter--;
      delay(200);
    }
  }

  CLKPrevious = CLKNow;  // Store last CLK state
}

void testAlarmChime() {

  if (alarm_is_activated == true && RTClock.hour == alarmHour && RTClock.min == alarmMinute && RTClock.sec == 1) {
    mp3.play(alarmMusic);
  }

  if (chime_is_activated == true && RTClock.min == 0 && RTClock.sec == 0) {
    mp3.play(4);
  }
}

String twoDigit(int number) {
  if (number < 10) {              // If it's a single-digit number
    return "0" + String(number);  // Add a leading zero
  } else {
    return String(number);  // Otherwise, just return the number as is
  }
}