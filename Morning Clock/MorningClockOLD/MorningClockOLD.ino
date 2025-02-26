
#include <Arduino.h>
#include <U8g2lib.h>
#include <DS3231.h>
#include <Wire.h>
#include <SerialMP3Player.h>
#include <EEPROM.h>

//prototype display
//U8G2_ST7571_128X128_2_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* cs=*/10, /* dc=*/9, /* reset=*/8);

//final display
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

bool alarmSetMusic_page = false;
bool chimeSetMusic_page = false;
bool volumeSet_page = false;

int alarmMusic;               //EEPROM adress 1
int chimeMusic;               //EEPROM adress 2
unsigned int alarmHour;       //EEPROM adress 3
unsigned int alarmMinute;     //EEPROM adress 4
unsigned int chimeStartHour;  //EEPROM adress 5
unsigned int chimeLastHour;   //EEPROM adress 6

unsigned int temporaryHour;
unsigned int temporaryMinute;
unsigned int temporarySecond;

unsigned int volume = 15;
bool page1_2bool = true;

DS3231 rtc(SDA, SCL);

Time RTClock;



void setup() {
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.firstPage();

  do {
    u8g2.drawXBMP(57, 23, 14, 18, bigHappy_BM);
    //u8g2.drawXBMP(61, 19, 7, 9, happyMenu_BM);
    //u8g2.drawBox(23, 46, progress, 2);
    //u8g2.drawFrame(21, 44, 84, 6);
    //u8g2.drawLine(0, 64, 127, 64);
  } while (u8g2.nextPage());

  Serial.begin(9600);  // start serial interface
  mp3.begin(9600);
  rtc.begin();
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

  alarmMusic = EEPROM.read(1);      //EEPROM adress 1
  chimeMusic = EEPROM.read(2);      //EEPROM adress 2
  alarmHour = EEPROM.read(3);       //EEPROM adress 3
  alarmMinute = EEPROM.read(4);     //EEPROM adress 4
  chimeStartHour = EEPROM.read(5);  //EEPROM adress 5
  chimeLastHour = EEPROM.read(6);   //EEPROM adress 6
  mp3.setVol(30);
  mp3.play(1);
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

    u8g2.setFont(u8g2_font_timR24_tn);
    u8g2.setCursor(28, 44);
    u8g2.print(twoDigit(RTClock.hour));
    u8g2.print(':');
    u8g2.print(twoDigit(RTClock.min));

    // print the date
    printDate();


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
        chimeSet_state();
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
        alarmMenuTransition();
        updateAlarmPage();
        rotateCounter = 1;
        break;

      case 2:
        chimeMenuTransition();
        updateChimePage();
        rotateCounter = 2;
        break;

      case 3:
        updateClockPage();
        rotateCounter = 3;
        break;

      case 4:
        updateCalendarPage();
        break;

      case 5:
        updateVolumePage();
        rotateCounter = 5;
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
        alarmToSettingsTransition();
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
      u8g2.setFont(u8g2_font_timR24_tn);
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
        EEPROM.update(3, alarmHour);
        EEPROM.update(4, alarmMinute);
        mp3.play(2);
        alarmSetHour_page = false;
        delay(500);
        buttonPressedState = false;
        rotateCounter = 2;
        break;
    }
  } while (alarmSetHour_page == true);
}
void alarmSet_music() {
  alarmSetMusic_page = true;
  int alarmSetStep = 1;
  rotateCounter = 2;
  buttonPressedState = false;

  do {
    u8g2.firstPage();
    do {
      drawAlarmMenu();
      switch (alarmSetStep) {
        case 1:
          if (alarmMusic == 3) {
            u8g2.setFont(u8g2_font_profont11_tf);
            u8g2.drawXBMP(5, 22, 7, 5, right_arrow_BM);
            u8g2.drawUTF8(18, 28, "Haydn : Concerto");
            u8g2.drawStr(15, 43, "Frutiger Aero");
            u8g2.drawStr(15, 58, "Grieg : Peer Gynt");
          }
          if (alarmMusic == 4) {
            u8g2.drawXBMP(5, 37, 7, 5, right_arrow_BM);
            u8g2.setFont(u8g2_font_profont11_tf);
            u8g2.drawUTF8(15, 28, "Haydn : Concerto");
            u8g2.drawStr(18, 43, "Frutiger Aero");
            u8g2.drawStr(15, 58, "Grieg : Peer Gynt");
          }
          if (alarmMusic == 5) {
            u8g2.setFont(u8g2_font_profont11_tf);
            u8g2.drawXBMP(5, 37, 7, 5, right_arrow_BM);
            u8g2.drawUTF8(15, 28, "Frutiger Aero");
            u8g2.drawStr(18, 43, "Grieg : Peer Gynt");
            u8g2.drawUTF8(15, 58, "Vivaldi : Été");
          }
          if (alarmMusic == 6) {
            u8g2.setFont(u8g2_font_profont11_tf);
            u8g2.drawXBMP(5, 52, 7, 5, right_arrow_BM);
            u8g2.drawUTF8(15, 28, "Frutiger Aero");
            u8g2.drawStr(15, 43, "Grieg : Peer Gynt");
            u8g2.drawUTF8(18, 58, "Vivaldi : Été");
          }
          break;

        case 2:
          choice_is_OK();
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
        if (rotateCounter < 3) {
          rotateCounter = 3;
        }
        if (rotateCounter > 6) {
          rotateCounter = 6;
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
        mp3.play(2);
        EEPROM.update(1, alarmMusic);
        alarmSetMusic_page = false;
        delay(500);
        buttonPressedState = false;
        rotateCounter = 1;
        break;
    }
  } while (alarmSetMusic_page == true);
}
void alarmMenuTransition() {
  int speed = 26;

  int pageTransition = 128;

  int arrowSet = 5;
  int menuSet1 = 18;
  int menuSet2 = 15;
  int icon1 = 113;
  int icon2 = 114;

  int menuAlarm1 = 146;
  int menuAlarm2 = 143;
  int arrowAlarm = 133;

  do {

    u8g2.firstPage();
    do {
      u8g2.drawXBMP(arrowSet, 22, 7, 5, right_arrow_BM);
      u8g2.drawUTF8(menuSet1, 28, "Réveil");
      u8g2.drawXBMP(icon1, 19, 10, 11, alarmON_select_BM);
      u8g2.drawStr(menuSet2, 43, "Carillon");
      u8g2.drawXBMP(icon2, 35, 8, 9, chimeON_BM);
      u8g2.drawStr(menuSet2, 58, "Horloge");
      u8g2.drawXBMP(icon2, 50, 8, 9, clock_BM);
      drawAlarmMenu();

      if (alarm_is_activated == true) {
        u8g2.drawUTF8(menuAlarm1, 28, "Désactiver");
      } else {
        u8g2.drawStr(menuAlarm1, 28, "Activer");
      }
      u8g2.drawStr(menuAlarm2, 43, "Heure");
      u8g2.drawStr(menuAlarm2, 58, "Musique");
      u8g2.drawXBMP(arrowAlarm, 22, 7, 5, right_arrow_BM);


    } while (u8g2.nextPage());

    u8g2.clearBuffer();

    arrowSet = arrowSet - speed;
    menuSet1 = menuSet1 - speed;
    menuSet2 = menuSet2 - speed;
    icon1 = icon1 - speed;
    icon2 = icon2 - speed;

    pageTransition = pageTransition - speed;
    menuAlarm1 = menuAlarm1 - speed;
    menuAlarm2 = menuAlarm2 - speed;
    arrowAlarm = arrowAlarm - speed;
  } while (pageTransition > 0);
}
void alarmToSettingsTransition() {
  int speed = 26;
  int pageTransition = -128;
  int arrowSet = -123;
  int menuSet1 = -110;
  int menuSet2 = -113;
  int icon1 = -15;
  int icon2 = -14;
  int menuAlarm1 = 15;
  int menuAlarm2 = 18;
  int arrowAlarm = 5;

  do {

    u8g2.firstPage();
    do {

      drawSettingsBar();
      u8g2.drawXBMP(arrowSet, 22, 7, 5, right_arrow_BM);
      u8g2.drawUTF8(menuSet1, 28, "Réveil");
      u8g2.drawXBMP(icon1, 19, 10, 11, alarmON_select_BM);
      u8g2.drawStr(menuSet2, 43, "Carillon");
      u8g2.drawXBMP(icon2, 35, 8, 9, chimeON_BM);
      u8g2.drawStr(menuSet2, 58, "Horloge");
      u8g2.drawXBMP(icon2, 50, 8, 9, clock_BM);

      u8g2.drawStr(menuAlarm1, 28, "Heure");
      u8g2.drawStr(menuAlarm1, 43, "Musique");
      u8g2.drawStr(menuAlarm2, 58, "Retour");
      u8g2.drawXBMP(arrowAlarm, 52, 7, 5, right_arrow_BM);

    } while (u8g2.nextPage());

    arrowSet = arrowSet + speed;
    menuSet1 = menuSet1 + speed;
    menuSet2 = menuSet2 + speed;
    icon1 = icon1 + speed;
    icon2 = icon2 + speed;

    pageTransition = pageTransition + speed;
    menuAlarm1 = menuAlarm1 + speed;
    menuAlarm2 = menuAlarm2 + speed;
    arrowAlarm = arrowAlarm + speed;
  } while (pageTransition < 0);
}


//chime page
void updateChimePage() {
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
          drawChimeMenu();
          if (chime_is_activated == true) {
            u8g2.drawUTF8(18, 28, "Désactiver");
          } else {
            u8g2.drawStr(18, 28, "Activer");
          }
          u8g2.drawUTF8(15, 43, "Heure de début/fin");
          u8g2.drawStr(15, 58, "Musique");
          u8g2.drawXBMP(5, 22, 7, 5, right_arrow_BM);
        } while (u8g2.nextPage());
        break;

      case 2:
        u8g2.firstPage();
        do {
          drawChimeMenu();
          if (chime_is_activated == true) {
            u8g2.drawUTF8(15, 28, "Désactiver");
          } else {
            u8g2.drawStr(15, 28, "Activer");
          }
          u8g2.drawUTF8(18, 43, "Heure de début/fin");
          u8g2.drawStr(15, 58, "Musique");
          u8g2.drawXBMP(5, 37, 7, 5, right_arrow_BM);
        } while (u8g2.nextPage());
        break;

      case 3:
        u8g2.firstPage();
        do {
          drawChimeMenu();
          u8g2.drawUTF8(15, 28, "Heure de début/fin");
          u8g2.drawStr(18, 43, "Musique");
          u8g2.drawStr(15, 58, "Retour");
          u8g2.drawXBMP(5, 37, 7, 5, right_arrow_BM);
        } while (u8g2.nextPage());
        break;

      case 4:
        u8g2.firstPage();
        do {
          drawChimeMenu();
          u8g2.drawUTF8(15, 28, "Heure de début/fin");
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
    executeChimeSetPage();
  } while (chimeSetPage == true);
}
void executeChimeSetPage() {
  if (buttonPressedState == true) {
    switch (rotateCounter) {
      case 1:
        chimeSet_state();
        break;

      case 2:
        chimeSet_hour();
        chimeSetPage = false;
        rotateCounter = 2;
        break;

      case 3:
        chimeSet_music();
        chimeSetPage = false;
        rotateCounter = 2;
        break;

      case 4:
        chimeToSettingsTransition();
        chimeSetPage = false;
        rotateCounter = 2;
        break;
    }
    buttonPressedState = false;  //reset this variable
  }
}

//chime page submenus
void chimeSet_state() {
  if (chime_is_activated == true) {
    chime_is_activated = false;
  } else {
    chime_is_activated = true;
  }
  buttonPressedState = false;
}
void chimeSet_hour() {
  bool chimeSetHour_page = true;
  int chimeSetStep = 1;
  rotateCounter = 0;
  buttonPressedState = false;
  chimeStartHour = 0;
  chimeLastHour = 0;

  do {
    u8g2.firstPage();
    do {
      drawChimeMenu();
      switch (chimeSetStep) {
        case 1:
          u8g2.setFont(u8g2_font_timR24_tn);
          u8g2.setCursor(19, 44);
          u8g2.print(twoDigit(chimeStartHour));
          u8g2.print(" - ");
          u8g2.print(twoDigit(chimeLastHour));
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawUTF8(13, 57, "Première sonnerie");
          break;
        case 2:
          u8g2.setFont(u8g2_font_timR24_tn);
          u8g2.setCursor(19, 44);
          u8g2.print(twoDigit(chimeStartHour));
          u8g2.print(" - ");
          u8g2.print(twoDigit(chimeLastHour));
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawUTF8(13, 57, "Dernière sonnerie");
          break;
        case 3:
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawUTF8(28, 41, "Enregistrées !");
          break;
      }

    } while (u8g2.nextPage());

    switch (chimeSetStep) {

      case 1:  //Setting first hour
        if (rotateCounter >= 24) {
          rotateCounter = 0;
        }
        if (rotateCounter < 0) {
          rotateCounter = 23;
        }
        chimeStartHour = rotateCounter;
        if (buttonPressedState == true) {
          rotateCounter = 0;
          buttonPressedState = false;
          chimeSetStep = 2;
        }
        break;

      case 2:  //Setting last hour
        if (rotateCounter >= 24) {
          rotateCounter = 0;
        }
        if (rotateCounter < 0) {
          rotateCounter = 23;
        }
        chimeLastHour = rotateCounter;
        if (buttonPressedState == true) {
          rotateCounter = 0;
          buttonPressedState = false;
          chimeSetStep = 3;
        }
        break;

      case 3:
        mp3.play(2);
        EEPROM.update(5, chimeStartHour);
        EEPROM.update(6, chimeLastHour);
        chimeSetHour_page = false;
        delay(500);
        buttonPressedState = false;
        rotateCounter = 2;
        break;
    }
  } while (chimeSetHour_page == true);
}
void chimeSet_music() {
  chimeSetMusic_page = true;
  int chimeSetStep = 1;
  rotateCounter = 2;
  buttonPressedState = false;

  do {
    u8g2.firstPage();
    do {
      drawAlarmMenu();
      switch (chimeSetStep) {
        case 1:
          if (chimeMusic == 7) {
            u8g2.setFont(u8g2_font_profont11_tf);
            u8g2.drawXBMP(5, 22, 7, 5, right_arrow_BM);
            u8g2.drawUTF8(18, 28, "West. moderne");
            u8g2.drawStr(15, 43, "West. ancienne");
            u8g2.drawStr(15, 58, "Campagne chinoise");
          }
          if (chimeMusic == 8) {
            u8g2.drawXBMP(5, 37, 7, 5, right_arrow_BM);
            u8g2.setFont(u8g2_font_profont11_tf);
            u8g2.drawUTF8(15, 28, "West. moderne");
            u8g2.drawStr(18, 43, "West. ancienne");
            u8g2.drawStr(15, 58, "Campagne chinoise");
          }
          if (chimeMusic == 9) {
            u8g2.setFont(u8g2_font_profont11_tf);
            u8g2.drawXBMP(5, 52, 7, 5, right_arrow_BM);
            u8g2.drawUTF8(15, 28, "West. moderne");
            u8g2.drawStr(15, 43, "West. ancienne");
            u8g2.drawStr(18, 58, "Campagne chinoise");
          }
          break;

        case 2:
          choice_is_OK();
          break;

        case 3:
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawStr(16, 35, "Choix de musique");
          u8g2.drawUTF8(28, 48, "enregistré !");
          break;
      }

    } while (u8g2.nextPage());

    switch (chimeSetStep) {

      case 1:  //Selection of the music
        if (rotateCounter < 7) {
          rotateCounter = 7;
        }
        if (rotateCounter > 9) {
          rotateCounter = 9;
        }
        chimeMusic = rotateCounter;
        if (buttonPressedState == true) {
          buttonPressedState = false;
          chimeSetStep = 2;
          rotateCounter = 1;
          mp3.play(chimeMusic);
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
          chimeSetStep = 3;
        }
        if (buttonPressedState == true && rotateCounter == 2) {
          buttonPressedState = false;
          chimeSetStep = 1;
          mp3.stop();
        }
        break;

      case 3:
        mp3.play(2);
        EEPROM.update(2, chimeMusic);
        chimeSetMusic_page = false;
        delay(500);
        buttonPressedState = false;
        break;
    }
  } while (chimeSetMusic_page == true);
}
void chimeMenuTransition() {
  int speed = 26;

  int pageTransition = 128;

  int arrowSet = 5;
  int menuSet1 = 18;
  int menuSet2 = 15;
  int icon1 = 113;
  int icon2 = 114;

  int menuChime1 = 146;
  int menuChime2 = 143;
  int arrowChime = 133;

  do {

    u8g2.firstPage();
    do {
      u8g2.drawXBMP(arrowSet, 37, 7, 5, right_arrow_BM);
      u8g2.drawUTF8(menuSet2, 28, "Réveil");
      u8g2.drawXBMP(icon2, 20, 8, 9, alarmON_BM);
      u8g2.drawStr(menuSet1, 43, "Carillon");
      u8g2.drawXBMP(icon1, 34, 10, 11, chimeON_select_BM);
      u8g2.drawStr(menuSet2, 58, "Horloge");
      u8g2.drawXBMP(icon2, 50, 8, 9, clock_BM);
      drawChimeMenu();

      if (chime_is_activated == true) {
        u8g2.drawUTF8(menuChime1, 28, "Désactiver");
      } else {
        u8g2.drawStr(menuChime1, 28, "Activer");
      }
      u8g2.drawUTF8(menuChime2, 43, "Heure de début/fin");
      u8g2.drawStr(menuChime2, 58, "Musique");
      u8g2.drawXBMP(arrowChime, 22, 7, 5, right_arrow_BM);

    } while (u8g2.nextPage());

    u8g2.clearBuffer();

    arrowSet = arrowSet - speed;
    menuSet1 = menuSet1 - speed;
    menuSet2 = menuSet2 - speed;
    icon1 = icon1 - speed;
    icon2 = icon2 - speed;

    pageTransition = pageTransition - speed;
    menuChime1 = menuChime1 - speed;
    menuChime2 = menuChime2 - speed;
    arrowChime = arrowChime - speed;
  } while (pageTransition > 0);
}
void chimeToSettingsTransition() {
  int speed = 26;
  int pageTransition = -128;
  int arrowSet = -123;
  int menuSet1 = -110;
  int menuSet2 = -113;
  int icon1 = -15;
  int icon2 = -14;
  int menuChime1 = 15;
  int menuChime2 = 18;
  int arrowChime = 5;

  do {

    u8g2.firstPage();
    do {

      drawSettingsBar();
      u8g2.drawXBMP(arrowSet, 37, 7, 5, right_arrow_BM);
      u8g2.drawUTF8(menuSet2, 28, "Réveil");
      u8g2.drawXBMP(icon2, 20, 8, 9, alarmON_BM);
      u8g2.drawStr(menuSet1, 43, "Carillon");
      u8g2.drawXBMP(icon1, 34, 10, 11, chimeON_select_BM);
      u8g2.drawStr(menuSet2, 58, "Horloge");
      u8g2.drawXBMP(icon2, 50, 8, 9, clock_BM);

      u8g2.drawUTF8(menuChime1, 28, "Heure de début/fin");
      u8g2.drawStr(menuChime1, 43, "Musique");
      u8g2.drawStr(menuChime2, 58, "Retour");
      u8g2.drawXBMP(arrowChime, 52, 7, 5, right_arrow_BM);

    } while (u8g2.nextPage());

    arrowSet = arrowSet + speed;
    menuSet1 = menuSet1 + speed;
    menuSet2 = menuSet2 + speed;
    icon1 = icon1 + speed;
    icon2 = icon2 + speed;

    pageTransition = pageTransition + speed;
    menuChime1 = menuChime1 + speed;
    menuChime2 = menuChime2 + speed;
    arrowChime = arrowChime + speed;
  } while (pageTransition < 0);
}


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
      u8g2.setFont(u8g2_font_timR24_tn);
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
        mp3.play(2);
        rtc.setTime(temporaryHour, temporaryMinute, temporarySecond);
        clockSet_page = false;
        delay(500);
        break;
    }
  } while (clockSet_page == true);
}


//calendar page
void updateCalendarPage() {
  bool calendarSet_page = true;
  int calendarSetStep = 1;
  unsigned int temporaryDate = 1;
  unsigned int temporaryMon = 1;
  unsigned int temporaryYear = 2025;
  rotateCounter = 1;
  buttonPressedState = false;

  do {
    u8g2.firstPage();
    do {
      u8g2.drawXBMP(2, 2, 8, 9, calendar_BM);
      drawMenuLine();
      u8g2.setFont(u8g2_font_profont11_tf);
      u8g2.drawUTF8(34, 10, "Calendrier");
      u8g2.setFont(u8g2_font_timR18_tn);
      u8g2.setCursor(9, 41);
      u8g2.setFontMode(1);
      u8g2.print(twoDigit(temporaryDate));
      u8g2.print("/");
      u8g2.print(twoDigit(temporaryMon));
      u8g2.print("/");
      u8g2.print(twoDigit(temporaryYear));
      switch (calendarSetStep) {
        case 1:
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawStr(46, 57, "Jour ?");
          break;

        case 2:
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawStr(46, 57, "Mois ?");
          break;

        case 3:
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawUTF8(43, 57, "Année ?");
          break;
        case 4:
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawUTF8(25, 57, "Enregistrée !");
          break;
      }

    } while (u8g2.nextPage());

    switch (calendarSetStep) {

      case 1:  //Setting day
        if (rotateCounter > 31) {
          rotateCounter = 1;
        }
        if (rotateCounter < 1) {
          rotateCounter = 31;
        }
        temporaryDate = rotateCounter;
        if (buttonPressedState == true) {
          rotateCounter = 1;
          buttonPressedState = false;
          calendarSetStep = 2;
        }
        break;

      case 2:  //Setting month
        if (rotateCounter > 12) {
          rotateCounter = 1;
        }
        if (rotateCounter < 1) {
          rotateCounter = 12;
        }
        temporaryMon = rotateCounter;
        if (buttonPressedState == true) {
          rotateCounter = 2025;
          buttonPressedState = false;
          calendarSetStep = 3;
        }
        break;

      case 3:  //Setting year
        if (rotateCounter > 2100) {
          rotateCounter = 2025;
        }
        if (rotateCounter < 2025) {
          rotateCounter = 2100;
        }
        temporaryYear = rotateCounter;
        if (buttonPressedState == true) {
          rotateCounter = 0;
          buttonPressedState = false;
          calendarSetStep = 4;
        }
        break;

      case 4:
        mp3.play(2);
        rtc.setDate(temporaryDate, temporaryMon, temporaryYear);
        calendarSet_page = false;
        delay(500);
        break;
    }
  } while (calendarSet_page == true);
}

//volume page
void updateVolumePage() {
  volumeSet_page = true;
  int volumeSetStep = 1;
  rotateCounter = volume;
  buttonPressedState = false;

  do {
    u8g2.firstPage();
    do {
      switch (volumeSetStep) {
        case 1:
          drawVolumeMenu();
          u8g2.drawFrame(16, 28, 96, 21);
          u8g2.drawBox(19, 31, volume * 3, 15);
          break;

        case 2:
          drawVolumeMenu();
          choice_is_OK();
          break;

        case 3:
          drawVolumeMenu();
          u8g2.setFont(u8g2_font_profont11_tf);
          u8g2.drawStr(16, 35, "Niveau du volume");
          u8g2.drawUTF8(28, 48, "enregistré !");
          break;
      }

    } while (u8g2.nextPage());

    switch (volumeSetStep) {

      case 1:  //Setting volume

        if (rotateCounter >= 15) {
          rotateCounter = 15;
        }
        if (rotateCounter < 0) {
          rotateCounter = 0;
        }
        volume = rotateCounter * 2;
        if (buttonPressedState == true) {
          rotateCounter = 1;
          buttonPressedState = false;
          volumeSetStep = 2;
          mp3.setVol(volume);
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
          volumeSetStep = 3;
        }
        if (buttonPressedState == true && rotateCounter == 2) {
          buttonPressedState = false;
          volumeSetStep = 1;
          mp3.stop();
          rotateCounter = volume;
        }
        break;

      case 3:
        mp3.play(2);
        volumeSet_page = false;
        delay(500);
        break;
    }
  } while (volumeSet_page == true);
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

void drawChimeMenu() {
  drawMenuLine();
  u8g2.drawUTF8(40, 10, "Carillon");
  u8g2.drawXBMP(2, 2, 8, 9, chimeON_BM);
}

void drawVolumeMenu() {
  drawMenuLine();
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawStr(46, 10, "Volume");
  u8g2.drawXBMP(2, 3, 8, 7, volume_BM);
}

void choice_is_OK() {
  if (rotateCounter == 1) {
    if (alarmSetMusic_page == true || chimeSetMusic_page == true) {
      u8g2.drawStr(25, 29, "Ce choix vous");
      u8g2.drawStr(25, 41, "convient-il ?");
    }
    if (volumeSet_page == true) {
      u8g2.drawStr(10, 29, "Ce choix de volume");
      u8g2.drawStr(10, 41, "vous convient-il ?");
    }
    u8g2.drawStr(34, 56, "Oui");
    u8g2.drawStr(77, 56, "Non");
    u8g2.setDrawColor(2);
    u8g2.drawBox(31, 46, 22, 13);
    u8g2.setDrawColor(1);
  }
  if (rotateCounter == 2) {
    if (alarmSetMusic_page == true || chimeSetMusic_page == true) {
      u8g2.drawStr(25, 29, "Ce choix vous");
      u8g2.drawStr(25, 41, "convient-il ?");
    }
    if (volumeSet_page == true) {
      u8g2.drawStr(10, 29, "Ce choix de volume");
      u8g2.drawStr(10, 41, "vous convient-il ?");
    }
    u8g2.drawStr(34, 56, "Oui");
    u8g2.drawStr(77, 56, "Non");
    u8g2.setDrawColor(2);
    u8g2.drawBox(74, 46, 23, 13);
    u8g2.setDrawColor(1);
  }
}


void printDate() {
  int dateY = 58;
  u8g2.setFont(u8g2_font_profont11_tf);
  if (RTClock.mon == 1) {
    u8g2.setCursor(19, dateY);
    u8g2.print(twoDigit(RTClock.date));
    u8g2.print(' ');
    u8g2.print("janvier ");
    u8g2.print(RTClock.year);
  }
  if (RTClock.mon == 2) {
    u8g2.setCursor(19, dateY);
    u8g2.print(twoDigit(RTClock.date));
    u8g2.print(' ');
    u8g2.print("février ");
    u8g2.print(RTClock.year);
  }
  if (RTClock.mon == 3) {
    u8g2.setCursor(28, dateY);
    u8g2.print(twoDigit(RTClock.date));
    u8g2.print(' ');
    u8g2.print("mars ");
    u8g2.print(RTClock.year);
  }
  if (RTClock.mon == 4) {
    u8g2.setCursor(25, dateY);
    u8g2.print(twoDigit(RTClock.date));
    u8g2.print(' ');
    u8g2.print("avril ");
    u8g2.print(RTClock.year);
  }
  if (RTClock.mon == 5) {
    u8g2.setCursor(31, dateY);
    u8g2.print(twoDigit(RTClock.date));
    u8g2.print(' ');
    u8g2.print("mai ");
    u8g2.print(RTClock.year);
  }
  if (RTClock.mon == 6) {
    u8g2.setCursor(28, dateY);
    u8g2.print(twoDigit(RTClock.date));
    u8g2.print(' ');
    u8g2.print("juin ");
    u8g2.print(RTClock.year);
  }
  if (RTClock.mon == 7) {
    u8g2.setCursor(19, dateY);
    u8g2.print(twoDigit(RTClock.date));
    u8g2.print(' ');
    u8g2.print("juillet ");
    u8g2.print(RTClock.year);
  }
  if (RTClock.mon == 8) {
    u8g2.setCursor(28, dateY);
    u8g2.print(twoDigit(RTClock.date));
    u8g2.print(' ');
    u8g2.print("août ");
    u8g2.print(RTClock.year);
  }
  if (RTClock.mon == 9) {
    u8g2.setCursor(13, dateY);
    u8g2.print(twoDigit(RTClock.date));
    u8g2.print(' ');
    u8g2.print("septembre ");
    u8g2.print(RTClock.year);
  }
  if (RTClock.mon == 10) {
    u8g2.setCursor(19, dateY);
    u8g2.print(twoDigit(RTClock.date));
    u8g2.print(' ');
    u8g2.print("octobre ");
    u8g2.print(RTClock.year);
  }
  if (RTClock.mon == 11) {
    u8g2.setCursor(16, dateY);
    u8g2.print(twoDigit(RTClock.date));
    u8g2.print(' ');
    u8g2.print("novembre ");
    u8g2.print(RTClock.year);
  }
  if (RTClock.mon == 12) {
    u8g2.setCursor(16, dateY);
    u8g2.print(twoDigit(RTClock.date));
    u8g2.print(' ');
    u8g2.print("décembre ");
    u8g2.print(RTClock.year);
  }
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
    delay(1200);
    mp3.play(alarmMusic);
  } else if (chime_is_activated == true && RTClock.hour >= chimeStartHour && RTClock.hour <= chimeLastHour && RTClock.min == 0 && RTClock.sec == 1) {
    delay(1200);
    mp3.play(chimeMusic);
  }
}

String twoDigit(int number) {
  if (number < 10) {              // If it's a single-digit number
    return "0" + String(number);  // Add a leading zero
  } else {
    return String(number);  // Otherwise, just return the number as is
  }
}