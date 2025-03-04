#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <RTClib.h>
#include <GD3300.h>
#include <SoftwareSerial.h>
#include <at24c32.h>


AT24C32 eprom(AT24C_ADDRESS_7);

SoftwareSerial mp3Serial(17 /*module TX, ESP RX*/, 18 /*module RX, ESP TX*/);

GD3300 soundSystem;

RTC_DS3231 rtc;

U8G2_ST7567_JLX12864_F_4W_SW_SPI u8g2(U8G2_R2, /* clock=*/12, /* data=*/13, /* cs=*/14, /* dc=*/15, /* reset=*/16);



//Defining pins for buttons
const int RotaryCLK = 2;  //CLK
const int RotaryDT = 3;   //DT
const int RotarySW = 4;   //SW (Button function)
const int backlight = 5;  //control LCD backlight





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
  // 'right_arrow, 4x7px
  0x01, 0x02, 0x04, 0x08, 0x04, 0x02, 0x01
};
const unsigned char left_arrow_BM[] PROGMEM = {
  // 'right_arrow, 4x7px
  0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08
};
const unsigned char up_arrow_BM[] PROGMEM = {
  // 'up_arrow, 8x4px
  0x18, 0x24, 0x42, 0x81
};
const unsigned char down_arrow_BM[] PROGMEM = {
  // 'down_arrow, 8x4px
  0x81, 0x42, 0x24, 0x18
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
const unsigned char volume_BM[] PROGMEM = {
  // 'volume_BM, 8x7px
  0x08, 0x4c, 0x8f, 0xaf, 0x8f, 0x4c, 0x08
};
const unsigned char about_BM[] PROGMEM = {
  // 'about_BM, 8x7px
  0x66, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x18
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
// 'volume_BM, 8x7px
// 'right_arrow_BM, 4x7px
// 'left_arrow_BM, 4x7px
// 'up_arrow_BM, 8x4px
// 'down_arrow_BM, 8x4px
// 'about_BM, 8x7px




//   ==Menus items==

const char* mainSettingsMenuTitles[] = {
  /*0*/ "Useless",
  /*1*/ "Réveil",
  /*2*/ "Carillon",
  /*3*/ "Volume",
  /*4*/ "Affichage",
  /*5*/ "Heure & date",
  /*6*/ "À propos",
  /*7*/ "Retour",
};

const char* alarmSettingsMenuTitles[] = {
  /*0*/ "Activer",
  /*1*/ "Désactiver",
  /*2*/ "Heure",
  /*3*/ "Son",
  /*4*/ "Retour",
};

const char* monthsOfTheYear[] = {
  "useless",
  "janvier",
  "février",
  "mars",
  "avril",
  "mai",
  "juin",
  "juillet",
  "septembre",
  "octobre",
  "novembre",
  "décembre"
};

const char* clockDateMenuTitles[] = {
  "useless",
  "Régler l'heure",
  "Régler la date",
  "Retour"
};




//   ==Variables==

//rotary encoder and button
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


//variables for the menus
bool homePage = true;

bool mainSettingsMenu = false;

bool alarmSettingsMenu = false;
bool alarmSetHour_menu = false;
int alarmSetStep;

bool chimeSettingsMenu = false;
bool chimeSetMusic_menu = false;

bool volumeSettingsMenu = false;

bool screenSettingsMenu = false;

bool clockDateSettingsMenu = false;
bool clockSet_menu = false;
bool dateSet_menu = false;


//about page
bool aboutPage = false;
static int aboutPageSize = 150;
static int aboutStepSize = 5;
int aboutNumberStep;
int aboutCurrentStep;
int text1;
int text2;
int text3;
int text4;
int text5;
int text6;
int text7;
int text8;
int text9;
int text10;
int text11;
int text12;


//variables for the scrollbar
static int startScrollbar = 15;
static int endScrollbar = 62;
static int scrollbarTotalSize = 47;
static int screenSize = 50;

int elevatorSize;
int scrollCurrentPosition;


//variables for the adaptatives menus
int rotatePrevious;
int frameSettingsMenu = 1;
int frameAlarmMenu = 1;
int menuItemSelect = 1;


//variables that will be saved into the EEPROM
int alarmMusic;                   //EEPROM adress 1
int chimeMusic;                   //EEPROM adress 2
unsigned int alarmHour;           //EEPROM adress 3
unsigned int alarmMinute;         //EEPROM adress 4
unsigned int chimeStartHour;      //EEPROM adress 5
unsigned int chimeLastHour;       //EEPROM adress 6
unsigned int volumeAlarm;         //EEPROM adress 7
unsigned int volumeChime;         //EEPROM adress 8
unsigned int volumeNotification;  //EEPROM adress 9


//variables used to set the hour
unsigned int temporaryHour;
unsigned int temporaryMinute;
unsigned int temporarySecond;


//volume variable
unsigned int volume = 15;





void setup() {
  mp3Serial.begin(9600);
  soundSystem.begin(mp3Serial);
  u8g2.begin();

  u8g2.enableUTF8Print();
  u8g2.setContrast(130);
  pinMode(backlight, OUTPUT);

  u8g2.clearBuffer();
  u8g2.drawXBMP(57, 23, 14, 18, bigHappy_BM);
  u8g2.sendBuffer();
  delay(200);
  int lightning = 0;

  do {
    analogWrite(backlight, lightning);
    lightning++;
    delay(5);
  } while (lightning < 256);

  Wire.begin();
  rtc.begin();
  soundSystem.sendCommand(CMD_SEL_DEV, 0, 2);



  //setting up pins
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);


  //Store states
  CLKPrevious = digitalRead(RotaryCLK);
  DTPrevious = digitalRead(RotaryDT);

  attachInterrupt((RotaryCLK), wheel_is_rotated, CHANGE);
  attachInterrupt((RotarySW), button_is_pressed, FALLING);

  alarmMusic = eprom.read(1);
  chimeMusic = eprom.read(2);
  alarmHour = eprom.read(3);
  alarmMinute = eprom.read(4);
  chimeStartHour = eprom.read(5);
  chimeLastHour = eprom.read(6);
  volumeAlarm = eprom.read(7);
  volumeChime = eprom.read(8);
  volumeNotification = eprom.read(9);


  TimeNow1 = millis();  //Start timer 1
  soundSystem.play(1);
}

void loop() {

  if (homePage == true && mainSettingsMenu == false) {
    updateHomePage();
    printHomePage();
    executeHomePage();
  }

  if (homePage == false && mainSettingsMenu == true) {
    if (alarmSettingsMenu == true) {
      if (alarmSetHour_menu == true) {
        printAlarmSet_hour();
        updateAlarmSet_hour();
      } else {
        printAlarmMenu();
        updateAlarmMenu();
        executeAlarmMenu();
      }
    }
    if (chimeSettingsMenu == true) {
    }
    if (volumeSettingsMenu == true) {
    }
    if (screenSettingsMenu == true) {
    }
    if (clockDateSettingsMenu == true) {
      if (clockSet_menu == true) {
      } else if (dateSet_menu == true) {

      } else {
        printClockDateMenu();
        updateClockDateMenu();
        executeClockDateMenu();
      }
    }
    if (aboutPage == true) {
      updateAboutPage();
      printAboutPage();
      executeAboutPage();
    }
    if (alarmSettingsMenu == false && chimeSettingsMenu == false && volumeSettingsMenu == false && screenSettingsMenu == false && clockDateSettingsMenu == false && aboutPage == false) {
      printSettingsMenu();
      updateSettingsMenu();
      executeSettingsMenu();
    }
  }
}





//   ==main page==

void printHomePage() {

  DateTime now = rtc.now();

  u8g2.clearBuffer();
  drawHomePageBar();

  //u8g2.setFont(u8g2_font_logisoso24_tn);
  //u8g2.setFont(u8g2_font_maniac_tn);
  u8g2.setFont(u8g2_font_timR24_tn);

  u8g2.setCursor(28, 44);
  u8g2.print(twoDigit(now.hour()));
  u8g2.print(':');
  u8g2.print(twoDigit(now.minute()));
  drawDate();
  u8g2.sendBuffer();
}
void updateHomePage() {
  switch (rotateCounter) {
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

    default:
      if (rotateCounter > 0) {
        rotateCounter = -3;
      }
      if (rotateCounter < -3) {
        rotateCounter = 0;
      }
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
        mainSettingsMenu = true;
        rotateCounter = 1;
        frameSettingsMenu = 1;
        menuItemSelect = 1;
        homePage_to_settingsTransition();
        break;
    }
  }
  buttonPressedState = false;  //reset this variable
}





//   ==main settings menu==

void printSettingsMenu() {

  u8g2.clearBuffer();
  drawSettingsBar();
  drawScrollbar();
  u8g2.setFontMode(2);
  u8g2.setDrawColor(1);
  u8g2.setBitmapMode(1);
  u8g2.setFont(u8g2_font_profont11_tf);
  switch (frameSettingsMenu) {
    case 1:
      scrollbar_5frames();
      u8g2.drawUTF8(6, 27, mainSettingsMenuTitles[1]);
      u8g2.drawStr(6, 43, mainSettingsMenuTitles[2]);
      u8g2.drawStr(6, 59, mainSettingsMenuTitles[3]);
      u8g2.setDrawColor(2);
      switch (menuItemSelect) {
        case 1:
          u8g2.drawBox(0, 16, 122, 15);
          u8g2.drawXBMP(112, 20, 4, 7, right_arrow_BM);
          break;

        case 2:
          u8g2.drawBox(0, 32, 122, 15);
          u8g2.drawXBMP(112, 36, 4, 7, right_arrow_BM);
          break;

        case 3:
          u8g2.drawBox(0, 48, 122, 15);
          u8g2.drawXBMP(112, 52, 4, 7, right_arrow_BM);
          break;
      }
      break;

    case 2:
      scrollbar_5frames();
      u8g2.drawStr(6, 27, mainSettingsMenuTitles[2]);
      u8g2.drawStr(6, 43, mainSettingsMenuTitles[3]);
      u8g2.drawStr(6, 59, mainSettingsMenuTitles[4]);
      u8g2.setDrawColor(2);
      switch (menuItemSelect) {
        case 2:
          u8g2.drawBox(0, 16, 122, 15);
          u8g2.drawXBMP(112, 20, 4, 7, right_arrow_BM);
          break;

        case 3:
          u8g2.drawBox(0, 32, 122, 15);
          u8g2.drawXBMP(112, 36, 4, 7, right_arrow_BM);

          break;

        case 4:
          u8g2.drawBox(0, 48, 122, 15);
          u8g2.drawXBMP(112, 52, 4, 7, right_arrow_BM);
          break;
      }
      break;

    case 3:
      scrollbar_5frames();
      u8g2.drawStr(6, 27, mainSettingsMenuTitles[3]);
      u8g2.drawStr(6, 43, mainSettingsMenuTitles[4]);
      u8g2.drawStr(6, 59, mainSettingsMenuTitles[5]);
      u8g2.setDrawColor(2);
      switch (menuItemSelect) {
        case 3:
          u8g2.drawBox(0, 16, 122, 15);
          u8g2.drawXBMP(112, 20, 4, 7, right_arrow_BM);
          break;

        case 4:
          u8g2.drawBox(0, 32, 122, 15);
          u8g2.drawXBMP(112, 36, 4, 7, right_arrow_BM);
          break;

        case 5:
          u8g2.drawBox(0, 48, 122, 15);
          u8g2.drawXBMP(112, 52, 4, 7, right_arrow_BM);
          break;
      }
      break;

    case 4:
      scrollbar_5frames();
      u8g2.drawStr(6, 27, mainSettingsMenuTitles[4]);
      u8g2.drawStr(6, 43, mainSettingsMenuTitles[5]);
      u8g2.drawUTF8(6, 59, mainSettingsMenuTitles[6]);
      u8g2.setDrawColor(2);
      switch (menuItemSelect) {
        case 4:
          u8g2.drawBox(0, 16, 122, 15);
          u8g2.drawXBMP(112, 20, 4, 7, right_arrow_BM);
          break;

        case 5:
          u8g2.drawBox(0, 32, 122, 15);
          u8g2.drawXBMP(112, 36, 4, 7, right_arrow_BM);
          break;

        case 6:
          u8g2.drawBox(0, 48, 122, 15);
          u8g2.drawXBMP(112, 52, 4, 7, right_arrow_BM);
          break;
      }
      break;

    case 5:
      scrollbar_5frames();
      u8g2.drawStr(6, 27, mainSettingsMenuTitles[5]);
      u8g2.drawUTF8(6, 43, mainSettingsMenuTitles[6]);
      u8g2.drawStr(6, 59, mainSettingsMenuTitles[7]);
      u8g2.setDrawColor(2);
      switch (menuItemSelect) {
        case 5:
          u8g2.drawBox(0, 16, 122, 15);
          u8g2.drawXBMP(112, 20, 4, 7, right_arrow_BM);
          break;

        case 6:
          u8g2.drawBox(0, 32, 122, 15);
          u8g2.drawXBMP(112, 36, 4, 7, right_arrow_BM);
          break;

        case 7:
          u8g2.drawBox(0, 48, 122, 15);
          u8g2.drawXBMP(112, 52, 4, 7, left_arrow_BM);
          break;
      }
      break;
  }
  u8g2.sendBuffer();
}
void updateSettingsMenu() {
  switch (rotateCounter) {
    case 1:
      menuItemSelect = 1;
      frameSettingsMenu = 1;
      if (frameSettingsMenu > 1 || frameSettingsMenu < 1) {
        frameSettingsMenu = 1;
      }
      rotatePrevious = rotateCounter;
      break;


    case 2:
      menuItemSelect = 2;
      if (rotatePrevious > rotateCounter && frameSettingsMenu == 3) {
        frameSettingsMenu--;
      }
      if (frameSettingsMenu > 2 || frameSettingsMenu < 1) {
        frameSettingsMenu = 1;
      }
      rotatePrevious = rotateCounter;
      break;

    case 3:
      menuItemSelect = 3;
      if (rotatePrevious > rotateCounter && frameSettingsMenu == 4) {
        frameSettingsMenu--;
      }
      if (frameSettingsMenu > 3 || frameSettingsMenu < 1) {
        frameSettingsMenu = 2;
      }
      rotatePrevious = rotateCounter;
      break;

    case 4:
      menuItemSelect = 4;
      if (rotatePrevious > rotateCounter && frameSettingsMenu == 5) {
        frameSettingsMenu--;
      }
      if (rotatePrevious < rotateCounter && frameSettingsMenu == 1) {
        frameSettingsMenu++;
      }
      if (frameSettingsMenu > 4 || frameSettingsMenu < 2) {
        frameSettingsMenu = 3;
      }
      rotatePrevious = rotateCounter;
      break;

    case 5:
      menuItemSelect = 5;
      if (rotatePrevious < rotateCounter && frameSettingsMenu == 2) {
        frameSettingsMenu++;
      }
      if (frameSettingsMenu > 5 || frameSettingsMenu < 3) {
        frameSettingsMenu = 4;
      }
      rotatePrevious = rotateCounter;
      break;

    case 6:
      menuItemSelect = 6;
      if (rotatePrevious < rotateCounter && frameSettingsMenu == 3) {
        frameSettingsMenu++;
      }
      if (frameSettingsMenu > 5 || frameSettingsMenu < 4) {
        frameSettingsMenu = 5;
      }
      rotatePrevious = rotateCounter;
      break;


    case 7:
      menuItemSelect = 7;
      frameSettingsMenu = 5;
      if (frameSettingsMenu > 5 || frameSettingsMenu < 4) {
        frameSettingsMenu = 5;
      }
      rotatePrevious = rotateCounter;
      break;

    default:
      if (rotateCounter > 7) {
        rotateCounter = 7;
      }
      if (rotateCounter < 1) {
        rotateCounter = 1;
      }
      break;
  }
}
void executeSettingsMenu() {
  if (buttonPressedState == true) {
    switch (rotateCounter) {
      case 1:
        alarmSettingsMenu = true;
        rotateCounter = 1;
        menuItemSelect = 1;
        frameAlarmMenu = 1;
        settings_to_alarmTransition();
        break;

      case 2:
        break;

      case 3:
        break;

      case 4:
        break;

      case 5:
        clockDateSettingsMenu = true;
        rotateCounter = 1;
        menuItemSelect = 1;
        settings_to_clockDateTransition();
        break;

      case 6:
        aboutPage = true;
        rotateCounter = 0;
        aboutCurrentStep = 0;
        settings_to_aboutTransition();
        break;

      case 7:
        mainSettingsMenu = false;
        homePage = true;
        rotateCounter = 0;
        happyMenuSelect = false;
        settings_to_homePageTransition();
        break;
    }
  }
  buttonPressedState = false;  //reset this variable
}

void homePage_to_settingsTransition() {
  int speed = 10;
  int pageTransition = 50;

  int background = 64;
  int menuItem1 = 27 + pageTransition;
  int menuItem2 = 43 + pageTransition;
  int menuItem3 = 59 + pageTransition;
  int menuItemBox = 16 + pageTransition;
  int arrow = 20 + pageTransition;

  do {
    DateTime now = rtc.now();

    u8g2.clearBuffer();
    u8g2.setDrawColor(1);
    drawSettingsBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_timR24_tn);
    u8g2.setCursor(28, 44);
    u8g2.print(twoDigit(now.hour()));
    u8g2.print(':');
    u8g2.print(twoDigit(now.minute()));
    drawDate();
    u8g2.setDrawColor(0);
    u8g2.drawBox(0, background, 128, 50);
    u8g2.setDrawColor(1);
    drawScrollbar();
    scrollbar_5frames();
    u8g2.drawUTF8(6, menuItem1, mainSettingsMenuTitles[1]);
    u8g2.drawStr(6, menuItem2, mainSettingsMenuTitles[2]);
    u8g2.drawStr(6, menuItem3, mainSettingsMenuTitles[3]);
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, menuItemBox, 122, 15);
    u8g2.drawXBMP(112, arrow, 4, 7, right_arrow_BM);
    u8g2.sendBuffer();

    pageTransition = pageTransition - speed;
    background = background - speed;
    menuItem1 = menuItem1 - speed;
    menuItem2 = menuItem2 - speed;
    menuItem3 = menuItem3 - speed;
    menuItemBox = menuItemBox - speed;
    arrow = arrow - speed;
  } while (pageTransition > 0);
}
void settings_to_homePageTransition() {
  int speed = 10;
  int pageTransition = 50;

  int background = 14;
  int menuItem1 = 27;
  int menuItem2 = 43;
  int menuItem3 = 59;
  int menuItemBox = 48;
  int arrow = 52;
  int scrollbar = 13;
  int elevator = 42;

  do {
    DateTime now = rtc.now();

    u8g2.clearBuffer();
    u8g2.setDrawColor(1);
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    drawHomePageBar();
    u8g2.setFont(u8g2_font_timR24_tn);
    u8g2.setCursor(28, 44);
    u8g2.print(twoDigit(now.hour()));
    u8g2.print(':');
    u8g2.print(twoDigit(now.minute()));
    drawDate();
    u8g2.setDrawColor(0);
    u8g2.drawBox(0, background, 128, 50);
    u8g2.setDrawColor(1);
    //draw scrollbar
    u8g2.drawFrame(122, scrollbar, 6, 51);
    u8g2.drawFrame(124, elevator, 2, 20);
    u8g2.drawStr(6, menuItem1, mainSettingsMenuTitles[5]);
    u8g2.drawUTF8(6, menuItem2, mainSettingsMenuTitles[6]);
    u8g2.drawStr(6, menuItem3, mainSettingsMenuTitles[7]);
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, menuItemBox, 122, 15);
    u8g2.drawXBMP(112, arrow, 4, 7, left_arrow_BM);
    u8g2.sendBuffer();

    pageTransition = pageTransition - speed;
    background = background + speed;
    menuItem1 = menuItem1 + speed;
    menuItem2 = menuItem2 + speed;
    menuItem3 = menuItem3 + speed;
    menuItemBox = menuItemBox + speed;
    arrow = arrow + speed;
    scrollbar = scrollbar + speed;
    elevator = elevator + speed;

  } while (pageTransition > 0);
}





//   ==alarm settings menu==

void printAlarmMenu() {

  u8g2.clearBuffer();
  drawAlarmBar();
  drawScrollbar();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  u8g2.setFont(u8g2_font_profont11_tf);
  switch (frameAlarmMenu) {
    case 1:
      if (alarm_is_activated == true) {
        u8g2.drawUTF8(6, 27, alarmSettingsMenuTitles[1]);
      } else {
        u8g2.drawUTF8(6, 27, alarmSettingsMenuTitles[0]);
      }
      u8g2.drawStr(6, 43, alarmSettingsMenuTitles[2]);
      u8g2.drawStr(6, 59, alarmSettingsMenuTitles[3]);
      u8g2.setDrawColor(2);
      scrollbar_2frames();
      switch (menuItemSelect) {
        case 1:
          u8g2.drawBox(0, 16, 122, 15);
          break;

        case 2:
          u8g2.drawBox(0, 32, 122, 15);
          u8g2.drawXBMP(112, 36, 4, 7, right_arrow_BM);
          break;

        case 3:
          u8g2.drawBox(0, 48, 122, 15);
          u8g2.drawXBMP(112, 52, 4, 7, right_arrow_BM);
          break;
      }
      break;

    case 2:
      u8g2.drawStr(6, 27, alarmSettingsMenuTitles[2]);
      u8g2.drawStr(6, 43, alarmSettingsMenuTitles[3]);
      u8g2.drawStr(6, 59, alarmSettingsMenuTitles[4]);
      u8g2.setDrawColor(2);
      scrollbar_2frames();
      switch (menuItemSelect) {
        case 2:
          u8g2.drawBox(0, 16, 122, 15);
          u8g2.drawXBMP(112, 20, 4, 7, right_arrow_BM);
          break;

        case 3:
          u8g2.drawBox(0, 32, 122, 15);
          u8g2.drawXBMP(112, 36, 4, 7, right_arrow_BM);
          break;

        case 4:
          u8g2.drawBox(0, 48, 122, 15);
          u8g2.drawXBMP(112, 52, 4, 7, left_arrow_BM);
          break;
      }
      break;
  }
  u8g2.sendBuffer();
}
void updateAlarmMenu() {
  switch (rotateCounter) {
    case 1:
      menuItemSelect = 1;
      if (frameAlarmMenu > 1) {
        frameAlarmMenu = 1;
      }
      break;

    case 2:
      menuItemSelect = 2;
      break;

    case 3:
      menuItemSelect = 3;
      break;

    case 4:
      menuItemSelect = 4;
      if (frameAlarmMenu < 2) {
        frameAlarmMenu = 2;
      }
      break;

    default:
      if (rotateCounter > 4) {
        rotateCounter = 4;
      }
      if (rotateCounter < 1) {
        rotateCounter = 1;
      }
  }
}
void executeAlarmMenu() {
  if (buttonPressedState == true) {
    switch (rotateCounter) {
      case 1:
        alarmSet_state();
        break;

      case 2:
        alarmSetHour_menu = true;
        alarmSetStep = 1;
        rotateCounter = alarmHour;
        alarm_to_hourTransition();
        break;

      case 3:
        break;

      case 4:
        alarmSettingsMenu = false;
        rotateCounter = 1;
        menuItemSelect = 1;
        alarm_to_settingsTransition();
        break;
    }
  }
  buttonPressedState = false;
}

void settings_to_alarmTransition() {
  int speed = 25;

  int pageTransition = 125;
  int settingItems = 6;
  int settingsBox = 0;
  int settingsArrow = 112;

  int alarmItems = 6 + pageTransition;
  int alarmBox = 0 + pageTransition;

  do {
    u8g2.clearBuffer();
    drawAlarmBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.drawUTF8(settingItems, 27, mainSettingsMenuTitles[1]);
    u8g2.drawStr(settingItems, 43, mainSettingsMenuTitles[2]);
    u8g2.drawStr(settingItems, 59, mainSettingsMenuTitles[3]);
    u8g2.drawXBMP(settingsArrow, 20, 4, 7, right_arrow_BM);

    if (alarm_is_activated == true) {
      u8g2.drawUTF8(alarmItems, 27, alarmSettingsMenuTitles[1]);
    } else {
      u8g2.drawStr(alarmItems, 27, alarmSettingsMenuTitles[0]);
    }
    u8g2.drawStr(alarmItems, 43, alarmSettingsMenuTitles[2]);
    u8g2.drawStr(alarmItems, 59, alarmSettingsMenuTitles[3]);


    u8g2.setDrawColor(2);
    u8g2.drawBox(settingsBox, 16, 122, 15);
    u8g2.drawBox(alarmBox, 16, 122, 15);
    drawScrollbar();
    scrollbar_2frames();
    u8g2.sendBuffer();

    pageTransition = pageTransition - speed;
    settingItems = settingItems - speed;
    settingsBox = settingsBox - speed;
    settingsArrow = settingsArrow - speed;

    alarmItems = alarmItems - speed;
    alarmBox = alarmBox - speed;

  } while (pageTransition > 0);
}
void alarm_to_settingsTransition() {
  int speed = 25;

  int pageTransition = -125;
  int settingItems = 6 + pageTransition;
  int settingsBox = 0 + pageTransition;
  int settingsArrow = 118 + pageTransition;

  int alarmItems = 6;
  int alarmBox = 0;
  int alarmArrow = 112;

  do {
    u8g2.clearBuffer();
    drawSettingsBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[1]);
    u8g2.drawStr(settingItems, 43, mainSettingsMenuTitles[2]);
    u8g2.drawStr(settingItems, 59, mainSettingsMenuTitles[3]);
    u8g2.drawXBMP(settingsArrow, 20, 4, 7, right_arrow_BM);

    u8g2.drawStr(alarmItems, 27, alarmSettingsMenuTitles[2]);
    u8g2.drawStr(alarmItems, 43, alarmSettingsMenuTitles[3]);
    u8g2.drawStr(alarmItems, 59, alarmSettingsMenuTitles[4]);
    u8g2.drawXBMP(alarmArrow, 52, 4, 7, left_arrow_BM);

    u8g2.setDrawColor(2);
    u8g2.drawBox(settingsBox, 16, 122, 15);
    u8g2.drawBox(alarmBox, 48, 122, 15);
    drawScrollbar();
    scrollbar_5frames();
    u8g2.sendBuffer();

    pageTransition = pageTransition + speed;
    settingItems = settingItems + speed;
    settingsBox = settingsBox + speed;
    settingsArrow = settingsArrow + speed;

    alarmItems = alarmItems + speed;
    alarmBox = alarmBox + speed;
    alarmArrow = alarmArrow + speed;

  } while (pageTransition < 0);
}


//    ==alarm settings submenus==

//alarm / state
void alarmSet_state() {
  if (alarm_is_activated == true) {
    alarm_is_activated = false;
  } else {
    alarm_is_activated = true;
  }
}

//alarm / hour
void printAlarmSet_hour() {

  u8g2.clearBuffer();
  drawAlarmBar();
  u8g2.setFont(u8g2_font_timR18_tr);
  u8g2.setCursor(37, 46);
  u8g2.print(twoDigit(alarmHour));
  u8g2.print(':');
  u8g2.print(twoDigit(alarmMinute));
  switch (alarmSetStep) {
    case 1:
      u8g2.drawXBMP(45, 21, 8, 4, up_arrow_BM);
      u8g2.drawXBMP(45, 50, 8, 4, down_arrow_BM);
      break;

    case 2:
      u8g2.drawXBM(75, 21, 8, 4, up_arrow_BM);
      u8g2.drawXBM(75, 50, 8, 4, down_arrow_BM);
      break;

    case 3:
      break;
  }
  u8g2.sendBuffer();
}
void updateAlarmSet_hour() {
  switch (alarmSetStep) {

    case 1:  //Setting hour
      if (rotateCounter >= 24) {
        rotateCounter = 0;
      }
      if (rotateCounter < 0) {
        rotateCounter = 23;
      }
      alarmHour = rotateCounter;
      if (buttonPressedState == true) {
        rotateCounter = alarmMinute;
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
        buttonPressedState = false;
        alarmSetStep = 3;
      }
      break;

    case 3:
      eprom.update(3, alarmHour);
      eprom.update(4, alarmMinute);
      alarmSetHour_menu = false;
      buttonPressedState = false;
      rotateCounter = 2;
      hour_to_alarmTransition();
      break;
  }
}
void alarm_to_hourTransition() {
  int speed = 25;

  int pageTransition = 125;
  int alarmItems = 6;
  int alarmBox = 0;
  int alarmArrow = 118;

  int arrow = 45 + pageTransition;
  int hourCursor = 37 + pageTransition;

  do {

    u8g2.clearBuffer();
    drawAlarmBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont11_tf);
    if (frameAlarmMenu == 1) {
      if (alarm_is_activated == true) {
        u8g2.drawUTF8(alarmItems, 27, alarmSettingsMenuTitles[1]);
      } else {
        u8g2.drawStr(alarmItems, 27, alarmSettingsMenuTitles[0]);
      }
      u8g2.drawStr(alarmItems, 43, alarmSettingsMenuTitles[2]);
      u8g2.drawStr(alarmItems, 59, alarmSettingsMenuTitles[3]);
      u8g2.drawBox(alarmBox, 32, 122, 15);
      u8g2.drawXBMP(alarmArrow, 36, 4, 7, right_arrow_BM);
    }
    if (frameAlarmMenu == 2) {
      u8g2.drawStr(alarmItems, 27, alarmSettingsMenuTitles[2]);
      u8g2.drawStr(alarmItems, 43, alarmSettingsMenuTitles[3]);
      u8g2.drawStr(alarmItems, 59, alarmSettingsMenuTitles[4]);
      u8g2.drawBox(alarmBox, 16, 122, 15);
    }

    u8g2.setFont(u8g2_font_timR18_tr);
    u8g2.drawStr(hourCursor, 46, "00:00");
    u8g2.drawXBMP(arrow, 21, 8, 4, up_arrow_BM);
    u8g2.drawXBMP(arrow, 50, 8, 4, down_arrow_BM);
    u8g2.sendBuffer();

    pageTransition = pageTransition - speed;
    alarmItems = alarmItems - speed;
    alarmBox = alarmBox - speed;
    alarmArrow = alarmArrow - speed;
    hourCursor = hourCursor - speed;
    arrow = arrow - speed;
  } while (pageTransition > 0);
}
void hour_to_alarmTransition() {
  int speed = 25;

  int pageTransition = -125;
  int alarmItems = 6 + pageTransition;
  int alarmBox = 0 + pageTransition;
  int alarmArrow = 118 + pageTransition;
  int clock = 37;

  do {

    u8g2.clearBuffer();
    drawAlarmBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);

    u8g2.setFont(u8g2_font_timR18_tr);
    u8g2.setCursor(clock, 46);
    u8g2.print(twoDigit(alarmHour));
    u8g2.print(':');
    u8g2.print(twoDigit(alarmMinute));

    u8g2.setFont(u8g2_font_profont11_tf);

    if (frameAlarmMenu == 1) {
      if (alarm_is_activated == true) {
        u8g2.drawUTF8(alarmItems, 27, alarmSettingsMenuTitles[1]);
      } else {
        u8g2.drawStr(alarmItems, 27, alarmSettingsMenuTitles[0]);
      }
      u8g2.drawStr(alarmItems, 43, alarmSettingsMenuTitles[2]);
      u8g2.drawStr(alarmItems, 59, alarmSettingsMenuTitles[3]);
      u8g2.drawBox(alarmBox, 32, 122, 15);
      u8g2.drawXBMP(alarmArrow, 36, 4, 7, right_arrow_BM);
    }
    if (frameAlarmMenu == 2) {
      u8g2.drawStr(alarmItems, 27, alarmSettingsMenuTitles[2]);
      u8g2.drawStr(alarmItems, 43, alarmSettingsMenuTitles[3]);
      u8g2.drawStr(alarmItems, 59, alarmSettingsMenuTitles[4]);
      u8g2.drawBox(alarmBox, 16, 122, 15);
    }
    drawScrollbar();
    scrollbar_2frames();
    u8g2.sendBuffer();

    pageTransition = pageTransition + speed;
    alarmItems = alarmItems + speed;
    alarmBox = alarmBox + speed;
    alarmArrow = alarmArrow + speed;
    clock = clock + speed;
  } while (pageTransition < 0);
}

//alarm / music
/*void alarmSet_music() {
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





//   ==chime settings menu==

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
}*/


//    ==chime settings submenus==

//chime / state
void chimeSet_state() {
  if (chime_is_activated == true) {
    chime_is_activated = false;
    soundSystem.stop();
  } else {
    chime_is_activated = true;
  }
  buttonPressedState = false;
} /*

//chime / hour
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
        chimeSetHour_page = false;
        delay(500);
        buttonPressedState = false;
        rotateCounter = 2;
        break;
    }
  } while (chimeSetHour_page == true);
}

//chime / music
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
        }
        break;

      case 3:
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




*/
//date / clock menu
void printClockDateMenu() {

  u8g2.clearBuffer();
  drawClockDateBar();
  u8g2.setFontMode(2);
  u8g2.setDrawColor(1);
  u8g2.setBitmapMode(1);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawUTF8(6, 27, clockDateMenuTitles[1]);
  u8g2.drawUTF8(6, 43, clockDateMenuTitles[2]);
  u8g2.drawStr(6, 59, clockDateMenuTitles[3]);
  u8g2.setDrawColor(2);
  switch (menuItemSelect) {
    case 1:
      u8g2.drawBox(0, 16, 128, 15);
      u8g2.drawXBMP(118, 20, 4, 7, right_arrow_BM);
      break;

    case 2:
      u8g2.drawBox(0, 32, 128, 15);
      u8g2.drawXBMP(118, 36, 4, 7, right_arrow_BM);
      break;

    case 3:
      u8g2.drawBox(0, 48, 128, 15);
      u8g2.drawXBMP(118, 52, 4, 7, left_arrow_BM);
      break;
  }
  u8g2.sendBuffer();
}
void updateClockDateMenu() {
  switch (rotateCounter) {
    case 1:
      menuItemSelect = 1;
      break;

    case 2:
      menuItemSelect = 2;
      break;

    case 3:
      menuItemSelect = 3;
      break;

    default:
      if (rotateCounter > 3) {
        rotateCounter = 3;
      }
      if (rotateCounter < 1) {
        rotateCounter = 1;
      }
      break;
  }
}
void executeClockDateMenu() {
  if (buttonPressedState == true) {
    switch (rotateCounter) {
      case 1:
        break;

      case 2:
        break;

      case 3:
        clockDateSettingsMenu = false;
        rotateCounter = 5;
        menuItemSelect = 5;
        clockDate_to_settingsTransition();
        break;
    }
  }
  buttonPressedState = false;
}
void settings_to_clockDateTransition() {
  int speed = 25;

  int pageTransition = 125;
  int settingItems = 6;
  int settingsBox = 0;
  int settingsArrow = 112;

  int clockDateItems = 6 + pageTransition;
  int clockDateBox = 0 + pageTransition;
  int clockDateArrow = 118 + pageTransition;

  do {
    u8g2.clearBuffer();
    drawClockDateBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont11_tf);
    switch (frameSettingsMenu) {
      case 3:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[3]);
        u8g2.drawStr(settingItems, 43, mainSettingsMenuTitles[4]);
        u8g2.drawStr(settingItems, 59, mainSettingsMenuTitles[5]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 48, 122, 15);
        u8g2.drawXBMP(settingsArrow, 52, 4, 7, right_arrow_BM);
        break;

      case 4:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[4]);
        u8g2.drawStr(settingItems, 43, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingItems, 59, mainSettingsMenuTitles[6]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 32, 122, 15);
        u8g2.drawXBMP(settingsArrow, 36, 4, 7, right_arrow_BM);
        break;

      case 5:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingItems, 43, mainSettingsMenuTitles[6]);
        u8g2.drawStr(settingItems, 59, mainSettingsMenuTitles[7]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 16, 122, 15);
        u8g2.drawXBMP(settingsArrow, 20, 4, 7, right_arrow_BM);
        break;
    }
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.drawUTF8(clockDateItems, 27, clockDateMenuTitles[1]);
    u8g2.drawUTF8(clockDateItems, 43, clockDateMenuTitles[2]);
    u8g2.drawStr(clockDateItems, 59, clockDateMenuTitles[3]);
    u8g2.drawBox(clockDateBox, 16, 128, 15);
    u8g2.drawXBMP(clockDateArrow, 20, 4, 7, right_arrow_BM);
    u8g2.sendBuffer();

    pageTransition = pageTransition - speed;
    settingItems = settingItems - speed;
    settingsBox = settingsBox - speed;
    settingsArrow = settingsArrow - speed;

    clockDateItems = clockDateItems - speed;
    clockDateBox = clockDateBox - speed;
    clockDateArrow = clockDateArrow - speed;

  } while (pageTransition > 0);
}
void clockDate_to_settingsTransition() {
  int speed = 25;

  int pageTransition = -125;
  int settingItems = 6 + pageTransition;
  int settingsBox = 0 + pageTransition;
  int settingsArrow = 112 + pageTransition;

  int clockDateItems = 6;
  int clockDateBox = 0;
  int clockDateArrow = 118;

  do {
    u8g2.clearBuffer();
    drawSettingsBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);

    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.drawUTF8(clockDateItems, 27, clockDateMenuTitles[1]);
    u8g2.drawUTF8(clockDateItems, 43, clockDateMenuTitles[2]);
    u8g2.drawStr(clockDateItems, 59, clockDateMenuTitles[3]);
    u8g2.setDrawColor(2);
    u8g2.drawBox(clockDateBox, 48, 128, 15);
    u8g2.drawXBMP(clockDateArrow, 52, 4, 7, left_arrow_BM);

    switch (frameSettingsMenu) {
      case 3:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[3]);
        u8g2.drawStr(settingItems, 43, mainSettingsMenuTitles[4]);
        u8g2.drawStr(settingItems, 59, mainSettingsMenuTitles[5]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 48, 122, 15);
        u8g2.drawXBMP(settingsArrow, 52, 4, 7, right_arrow_BM);
        break;

      case 4:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[4]);
        u8g2.drawStr(settingItems, 43, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingItems, 59, mainSettingsMenuTitles[6]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 32, 122, 15);
        u8g2.drawXBMP(settingsArrow, 36, 4, 7, right_arrow_BM);
        break;

      case 5:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingItems, 43, mainSettingsMenuTitles[6]);
        u8g2.drawStr(settingItems, 59, mainSettingsMenuTitles[7]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 16, 122, 15);
        u8g2.drawXBMP(settingsArrow, 20, 4, 7, right_arrow_BM);
        break;
    }
    drawScrollbar();
    scrollbar_5frames();
    u8g2.sendBuffer();

    pageTransition = pageTransition + speed;
    settingItems = settingItems + speed;
    settingsBox = settingsBox + speed;
    settingsArrow = settingsArrow + speed;

    clockDateItems = clockDateItems + speed;
    clockDateBox = clockDateBox + speed;
    clockDateArrow = clockDateArrow + speed;

  } while (pageTransition < 0);
}
/*



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
          rotateCounter = volume;
        }
        break;

      case 3:
        volumeSet_page = false;
        delay(500);
        break;
    }
  } while (volumeSet_page == true);
}
*/




//about page
void printAboutPage() {

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_profont17_tf);
  u8g2.drawStr(3, text1, "Morning Clock");

  u8g2.setFont(u8g2_font_profont12_tf);
  u8g2.drawUTF8(19, text2, "Imaginé, conçu");

  u8g2.setFont(u8g2_font_profont15_tf);
  u8g2.drawStr(61, text3, "Baptiste");

  u8g2.setFont(u8g2_font_profont12_tf);
  u8g2.drawStr(45, text4, "WILD-CARUANA");
  u8g2.drawUTF8(9, text5, " et fabriqué par ");
  u8g2.drawUTF8(4, text6, "Février");
  u8g2.drawStr(4, text7, "et Mars 2025");
  u8g2.drawStr(6, text8, "Exemplaire de test");
  u8g2.drawUTF8(25, text9, "attribué à :");

  u8g2.setFont(u8g2_font_profont15_tf);
  u8g2.drawStr(30, text10, "Version :");
  u8g2.drawStr(32, text11, "BETA 2.0");
  u8g2.drawUTF8(33, text12, "Baptiste");

  drawAboutBar();
  drawScrollbar();
  scrollbarAbout();
  u8g2.sendBuffer();
}
void updateAboutPage() {

  aboutNumberStep = aboutPageSize / aboutStepSize;

  if (rotateCounter < 0) {
    rotateCounter = 0;
  }
  if (rotateCounter > aboutNumberStep) {
    rotateCounter = aboutNumberStep;
  }

  aboutCurrentStep = map(rotateCounter, 0, aboutNumberStep, 0, aboutPageSize);

  text1 = 34 - aboutCurrentStep;
  text2 = 85 - aboutCurrentStep;
  text3 = 111 - aboutCurrentStep;
  text4 = 123 - aboutCurrentStep;
  text5 = 96 - aboutCurrentStep;
  text6 = 137 - aboutCurrentStep;
  text7 = 147 - aboutCurrentStep;
  text8 = 171 - aboutCurrentStep;
  text9 = 182 - aboutCurrentStep;
  text10 = 51 - aboutCurrentStep;
  text11 = 66 - aboutCurrentStep;
  text12 = 198 - aboutCurrentStep;
}
void executeAboutPage() {
  if (buttonPressedState == true) {
    aboutPage = false;
    rotateCounter = 6;
    menuItemSelect = 6;
    about_to_settingsTransition();
  }
  buttonPressedState = false;
}
void settings_to_aboutTransition() {

  int speed = 25;

  int pageTransition = 125;

  int settingsItems = 6;
  int settingsBox = 0;
  int settingsArrow = 112;

  int textTransition1 = 3 + pageTransition;
  int textTransition2 = 30 + pageTransition;
  int textTransition3 = 32 + pageTransition;

  do {
    u8g2.clearBuffer();
    u8g2.setFontMode(2);
    u8g2.setDrawColor(1);
    u8g2.setBitmapMode(1);
    switch (frameSettingsMenu) {
      case 4:
        u8g2.drawStr(settingsItems, 27, mainSettingsMenuTitles[4]);
        u8g2.drawStr(settingsItems, 43, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingsItems, 59, mainSettingsMenuTitles[6]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 48, 122, 15);
        u8g2.drawXBMP(settingsArrow, 52, 4, 7, right_arrow_BM);
        break;

      case 5:
        u8g2.drawStr(settingsItems, 27, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingsItems, 43, mainSettingsMenuTitles[6]);
        u8g2.drawStr(settingsItems, 59, mainSettingsMenuTitles[7]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 32, 122, 15);
        u8g2.drawXBMP(settingsArrow, 36, 4, 7, right_arrow_BM);
        break;
    }
    u8g2.setFontMode(2);
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_profont17_tf);
    u8g2.drawStr(textTransition1, 34, "Morning Clock");
    u8g2.setFont(u8g2_font_profont15_tf);
    u8g2.drawStr(textTransition2, 51, "Version :");
    u8g2.drawStr(textTransition3, 66, "BETA 2.0");
    drawAboutBar();
    drawScrollbar();
    scrollbarAbout();
    u8g2.sendBuffer();

    settingsItems = settingsItems - speed;
    settingsBox = settingsBox - speed;
    settingsArrow = settingsArrow - speed;

    textTransition1 = textTransition1 - speed;
    textTransition2 = textTransition2 - speed;
    textTransition3 = textTransition3 - speed;
    pageTransition = pageTransition - speed;

  } while (pageTransition > 0);
}
void about_to_settingsTransition() {
  int speed = 25;

  int pageTransition = -125;

  int settingsItems = 6 + pageTransition;
  int settingsBox = 0 + pageTransition;
  int settingsArrow = 112 + pageTransition;

  int textScroll1 = 3;
  int textScroll2 = 19;
  int textScroll3 = 61;
  int textScroll4 = 45;
  int textScroll5 = 9;
  int textScroll6 = 4;
  int textScroll7 = 6;
  int textScroll8 = 25;
  int textScroll9 = 30;
  int textScroll10 = 32;
  int textScroll11 = 33;

  do {
    u8g2.clearBuffer();
    u8g2.setFontMode(2);
    u8g2.setDrawColor(1);
    u8g2.setBitmapMode(1);
    switch (frameSettingsMenu) {
      case 4:
        u8g2.drawStr(settingsItems, 27, mainSettingsMenuTitles[4]);
        u8g2.drawStr(settingsItems, 43, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingsItems, 59, mainSettingsMenuTitles[6]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 48, 122, 15);
        u8g2.drawXBMP(settingsArrow, 52, 4, 7, right_arrow_BM);
        break;

      case 5:
        u8g2.drawStr(settingsItems, 27, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingsItems, 43, mainSettingsMenuTitles[6]);
        u8g2.drawStr(settingsItems, 59, mainSettingsMenuTitles[7]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 32, 122, 15);
        u8g2.drawXBMP(settingsArrow, 36, 4, 7, right_arrow_BM);
        break;
    }
    u8g2.setFontMode(2);
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_profont17_tf);
    u8g2.drawStr(textScroll1, text1, "Morning Clock");

    u8g2.setFont(u8g2_font_profont12_tf);
    u8g2.drawUTF8(textScroll2, text2, "Imaginé, conçu");

    u8g2.setFont(u8g2_font_profont15_tf);
    u8g2.drawStr(textScroll3, text3, "Baptiste");

    u8g2.setFont(u8g2_font_profont12_tf);
    u8g2.drawStr(textScroll4, text4, "WILD-CARUANA");
    u8g2.drawUTF8(textScroll5, text5, " et fabriqué par ");
    u8g2.drawUTF8(textScroll6, text6, "Février");
    u8g2.drawStr(textScroll6, text7, "et Mars 2025");
    u8g2.drawStr(textScroll7, text8, "Exemplaire de test");
    u8g2.drawUTF8(textScroll8, text9, "attribué à :");

    u8g2.setFont(u8g2_font_profont15_tf);
    u8g2.drawStr(textScroll9, text10, "Version :");
    u8g2.drawStr(textScroll10, text11, "BETA 2.0");
    u8g2.drawUTF8(textScroll11, text12, "Baptiste");

    drawSettingsBar();
    drawScrollbar();
    scrollbar_5frames();
    u8g2.sendBuffer();

    pageTransition = pageTransition + speed;

    settingsItems = settingsItems + speed;
    settingsBox = settingsBox + speed;
    settingsArrow = settingsArrow + speed;

    textScroll1 = textScroll1 + speed;
    textScroll2 = textScroll2 + speed;
    textScroll3 = textScroll3 + speed;
    textScroll4 = textScroll4 + speed;
    textScroll5 = textScroll5 + speed;
    textScroll6 = textScroll6 + speed;
    textScroll7 = textScroll7 + speed;
    textScroll8 = textScroll8 + speed;
    textScroll9 = textScroll9 + speed;
    textScroll10 = textScroll10 + speed;
    textScroll11 = textScroll11 + speed;

  } while (pageTransition < 0);
}




//   ==draw menus bar==

void drawHomePageBar() {
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

  u8g2.drawLine(0, 13, 128, 13);
}
void drawSettingsBar() {
  u8g2.setDrawColor(0);
  u8g2.drawBox(0, 0, 128, 14);
  u8g2.setDrawColor(1);
  u8g2.drawXBMP(2, 2, 7, 9, happyMenu_BM);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawUTF8(40, 10, "Réglages");
  u8g2.drawLine(0, 13, 128, 13);
}
void drawAlarmBar() {
  u8g2.drawXBMP(2, 2, 8, 9, alarmON_BM);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawUTF8(46, 10, "Réveil");
  u8g2.drawLine(0, 13, 128, 13);
}
void drawChimeBar() {
  u8g2.drawXBMP(2, 2, 8, 9, chimeON_BM);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawUTF8(40, 10, "Carillon");
  u8g2.drawLine(0, 13, 128, 13);
}
void drawVolumeBar() {
  u8g2.drawXBMP(2, 3, 8, 7, volume_BM);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawStr(46, 10, "Volume");
  u8g2.drawLine(0, 13, 128, 13);
}
void drawClockDateBar() {
  u8g2.drawXBMP(2, 2, 8, 9, calendar_BM);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawStr(25, 10, "Heure et date");
  u8g2.drawLine(0, 13, 128, 13);
}
void drawAboutBar() {
  u8g2.setDrawColor(0);
  u8g2.drawBox(0, 0, 128, 14);
  u8g2.setDrawColor(1);
  u8g2.drawXBMP(2, 3, 8, 7, about_BM);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawUTF8(40, 10, "À propos");
  u8g2.drawLine(0, 13, 127, 13);
}





// scroll bar stuff
void drawScrollbar() {
  u8g2.setDrawColor(0);
  u8g2.drawBox(122, 13, 6, 51);
  u8g2.setDrawColor(1);
  u8g2.drawFrame(122, 13, 6, 51);
}
void scrollbar_2frames() {
  switch (frameAlarmMenu) {
    case 1:
      u8g2.drawFrame(124, 15, 2, 36);
      break;

    case 2:
      u8g2.drawFrame(124, 26, 2, 36);
      break;
  }
}
void scrollbar_5frames() {
  switch (frameSettingsMenu) {
    case 1:
      u8g2.drawFrame(124, 15, 2, 20);
      break;

    case 2:
      u8g2.drawFrame(124, 21, 2, 20);
      break;

    case 3:
      u8g2.drawFrame(124, 28, 2, 20);
      break;

    case 4:
      u8g2.drawFrame(124, 35, 2, 20);
      break;

    case 5:
      u8g2.drawFrame(124, 42, 2, 20);
      break;
  }
}
void scrollbarAbout() {

  int aboutStopScrollbar;

  elevatorSize = scrollbarTotalSize / (aboutPageSize / screenSize);

  aboutStopScrollbar = endScrollbar - elevatorSize;

  scrollCurrentPosition = map(aboutCurrentStep, 0, aboutPageSize, startScrollbar, aboutStopScrollbar);

  u8g2.drawFrame(124, scrollCurrentPosition, 2, elevatorSize);
}



// date main menu
void drawDate() {
  DateTime now = rtc.now();
  int dateY = 58;
  u8g2.setFont(u8g2_font_profont11_tf);

  switch (now.month()) {
    case 1:
      u8g2.setCursor(19, dateY);
      break;

    case 2:
      u8g2.setCursor(19, dateY);
      break;

    case 3:
      u8g2.setCursor(28, dateY);
      break;

    case 4:
      u8g2.setCursor(25, dateY);
      break;

    case 5:
      u8g2.setCursor(31, dateY);
      break;

    case 6:
      u8g2.setCursor(28, dateY);
      break;

    case 7:
      u8g2.setCursor(19, dateY);
      break;

    case 8:
      u8g2.setCursor(28, dateY);
      break;

    case 9:
      u8g2.setCursor(13, dateY);
      break;

    case 10:
      u8g2.setCursor(19, dateY);
      break;

    case 11:
      u8g2.setCursor(16, dateY);
      break;

    case 12:
      u8g2.setCursor(16, dateY);
      break;
  }
  u8g2.print(twoDigit(now.day()));
  u8g2.print(' ');
  u8g2.print(monthsOfTheYear[now.month()]);
  u8g2.print(' ');
  u8g2.print(now.year());
}
//   ==buttons control==
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
      rotateCounter = rotateCounter + 1;
    } else {
      rotateCounter = rotateCounter - 1;
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