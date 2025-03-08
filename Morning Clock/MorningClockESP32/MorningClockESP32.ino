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
const unsigned char up_arrow_small_BM[] PROGMEM = {
  // 'up_arrow_small_BM, 7x4px
  0x08, 0x14, 0x22, 0x41
};
const unsigned char down_arrow_small_BM[] PROGMEM = {
  // 'down_arrow_small_BM, 7x4px
  0x41, 0x22, 0x14, 0x08
};
const unsigned char clock_BM[] PROGMEM = {
  // 'clock, 8x9px
  0x3c, 0x42, 0x89, 0x89, 0xb9, 0x81, 0x42, 0x3c, 0x3c
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
const unsigned char settings_BM[] PROGMEM = {
  // 'settings_BM', 8x9px
  0x40, 0xff, 0x40, 0x02, 0xff, 0x02, 0x40, 0xff, 0x40
};
const unsigned char display_BM[] PROGMEM = {
  // 'display_BM', 8x9px
  0xff, 0x81, 0x81, 0x81, 0x81, 0xff, 0x18, 0x18, 0x7e
};
const unsigned char brightness_low_BM[] PROGMEM = {
  // 'brightness_low_BM', 8x12px
  0x3c, 0x7e, 0xfd, 0xf9, 0xf1, 0xe1, 0x42, 0x24, 0x3c, 0x34, 0x2c, 0x18
};
const unsigned char brightness_high_BM[] PROGMEM = {
  // 'brightness_high_BM', 16x13px
  0x02, 0x40, 0xc4, 0x23, 0x20, 0x04, 0x10, 0x08, 0x13, 0xc8, 0x10, 0x08, 0x10, 0x08, 0x24, 0x24, 0x42, 0x42, 0xc0, 0x03, 0x40, 0x03, 0xc0, 0x02, 0x80, 0x01
};
const unsigned char contrast_low_BM[] PROGMEM = {
  // 'contrast_low_BM', 8x8px
  0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c
};
const unsigned char contrast_high_BM[] PROGMEM = {
  // 'contrast_high_BM', 8x8px
  0x3c, 0x4e, 0x8f, 0x8f, 0x8f, 0x8f, 0x4e, 0x3c
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
// 'calendar_BM, 8x9px
// 'volume_BM, 8x7px
// 'right_arrow_BM, 4x7px
// 'left_arrow_BM, 4x7px
// 'up_arrow_BM, 8x4px
// 'down_arrow_BM, 8x4px
// 'about_BM, 8x7px
// 'settings_BM', 8x9px
// 'display_BM', 8x9px
// 'brightness_low_BM', 8x12px
// 'brightness_high_BM', 16x13px
// 'contrast_low_BM', 8x8px
// 'contrast_high_BM', 8x8px
// 'up_arrow_small_BM, 7x4px
// 'down_arrow_small_BM, 7x4px



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
  "août",
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

const char* displayMenuTitles[] = {
  "useless",
  "Luminosité",
  "Contraste",
  "Retour"
};

const char* shortMonths[] = {
  "useless",
  "janv",
  "févr",
  "mars",
  "avr",
  "mai",
  "juin",
  "juil",
  "août",
  "sept",
  "oct",
  "nov",
  "déc"
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

bool displaySettingsMenu = false;
bool brightness_menu = false;
bool contrast_menu = false;

bool clockDateSettingsMenu = false;
bool clockSet_menu = false;
int clockSetStep;
bool dateSet_menu = false;
int dateSetStep;


//brightness/contrast page
int brightnessBarSize;
int contrastBarSize;

//about page
bool aboutPage = false;
static int aboutPageSize = 150;
static int aboutStepSize = 10;
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
unsigned int brightness;          //EEPROM adress 10
unsigned int contrast;            //EEPROM adress 11


//variables used to set the hour
int temporaryHour;
int temporaryMinute;
int temporarySecond;

//variables used to set the date
int temporaryYear;
int temporaryMonth;
int temporaryDay;


//volume variable
unsigned int volume = 15;

unsigned long previousMillis = 0;
const long interval = 500;
bool blink = false;

unsigned long previousMillisHomePage = 0;
const long intervalHomePage = 30000;

unsigned long previousMillisRotary = 0;



void setup() {
  mp3Serial.begin(9600);
  soundSystem.begin(mp3Serial);
  u8g2.begin();

  u8g2.enableUTF8Print();
  u8g2.setContrast(130);

  u8g2.clearBuffer();
  u8g2.drawXBMP(57, 23, 14, 18, bigHappy_BM);
  u8g2.sendBuffer();
  delay(200);
  Wire.begin();
  rtc.begin();
  soundSystem.sendCommand(CMD_SEL_DEV, 0, 2);



  //setting up pins
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);



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
  brightness = eprom.read(10);
  contrast = eprom.read(11);

  u8g2.setContrast(contrast);
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
        updateAlarmMenu();
        printAlarmMenu();
        executeAlarmMenu();
      }
    }
    if (chimeSettingsMenu == true) {
    }
    if (volumeSettingsMenu == true) {
    }
    if (displaySettingsMenu == true) {
      if (brightness_menu == true) {
        updateBrightnessMenu();
        printBrightnessMenu();
        executeBrightnessMenu();
      } else if (contrast_menu == true) {
        updateContrastMenu();
        printContrastMenu();
        executeContrastMenu();
      } else {
        updateDisplayMenu();
        printDisplayMenu();
        executeDisplayMenu();
      }
    }
    if (clockDateSettingsMenu == true) {
      if (clockSet_menu == true) {
        updateClockMenu();
        printClockMenu();
        executeClockMenu();
      } else if (dateSet_menu == true) {
        updateDateMenu();
        printDateMenu();
        executeDateMenu();

      } else {
        updateClockDateMenu();
        printClockDateMenu();
        executeClockDateMenu();
      }
    }
    if (aboutPage == true) {
      updateAboutPage();
      printAboutPage();
      executeAboutPage();
    }
    if (alarmSettingsMenu == false && chimeSettingsMenu == false && volumeSettingsMenu == false && displaySettingsMenu == false && clockDateSettingsMenu == false && aboutPage == false) {
      updateSettingsMenu();
      printSettingsMenu();
      executeSettingsMenu();
    }
  }
}





//   ==main page==

void printHomePage() {

  DateTime now = rtc.now();

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    if (blink == true) {
      blink = false;
    } else {
      blink = true;
    }
  }

  u8g2.clearBuffer();
  drawHomePageBar();
  //u8g2.setFont(u8g2_font_logisoso24_tn);
  //u8g2.setFont(u8g2_font_maniac_tn);
  u8g2.setFont(u8g2_font_timR24_tn);
  //u8g2.setFont(u8g2_font_helvB24_tn);
  //u8g2.setFont(u8g2_font_luBIS24_te);
  u8g2.setCursor(28, 44);
  u8g2.print(twoDigit(now.hour()));

  if (blink == true) {
    u8g2.print(":");
  } else {
    u8g2.print(" ");
  }

  u8g2.setCursor(69, 44);
  u8g2.print(twoDigit(now.minute()));
  drawDate();
  u8g2.sendBuffer();
}
void updateHomePage() {
  if (millis() - previousMillisHomePage >= intervalHomePage) {
    previousMillisHomePage = millis();
    rotateCounter = 0;
  }
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
        if (alarm_is_activated == false) {
          alarmHomePageTransitionIN();
          alarmSet_state();
        } else {
          alarmSet_state();
          alarmHomePageTransitionOUT();
        }
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
void alarmHomePageTransitionIN() {
  int speed = 4;
  int transition = -12;
  int Yposition = 10 + transition;

  do {
    DateTime now = rtc.now();

    if (millis() - previousMillis >= interval) {
      previousMillis = millis();
      if (blink == true) {
        blink = false;
      } else {
        blink = true;
      }
    }

    u8g2.clearBuffer();
    drawHomePageBar();
    u8g2.setFont(u8g2_font_timR24_tn);
    u8g2.setCursor(28, 44);
    u8g2.print(twoDigit(now.hour()));

    if (blink == true) {
      u8g2.print(":");
    } else {
      u8g2.print(" ");
    }

    u8g2.setCursor(69, 44);
    u8g2.print(twoDigit(now.minute()));
    drawDate();

    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.setCursor(96, Yposition);
    u8g2.print(twoDigit(alarmHour));
    u8g2.print(':');
    u8g2.print(twoDigit(alarmMinute));

    u8g2.sendBuffer();
    transition = transition + speed;
    Yposition = Yposition + speed;
  } while (transition < 0);
}
void alarmHomePageTransitionOUT() {
  int speed = 4;
  int transition = 12;
  int Yposition = 10;

  do {
    DateTime now = rtc.now();

    if (millis() - previousMillis >= interval) {
      previousMillis = millis();
      if (blink == true) {
        blink = false;
      } else {
        blink = true;
      }
    }

    u8g2.clearBuffer();
    drawHomePageBar();
    u8g2.setFont(u8g2_font_timR24_tn);
    u8g2.setCursor(28, 44);
    u8g2.print(twoDigit(now.hour()));

    if (blink == true) {
      u8g2.print(":");
    } else {
      u8g2.print(" ");
    }

    u8g2.setCursor(69, 44);
    u8g2.print(twoDigit(now.minute()));
    drawDate();

    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.setCursor(96, Yposition);
    u8g2.print(twoDigit(alarmHour));
    u8g2.print(':');
    u8g2.print(twoDigit(alarmMinute));

    u8g2.sendBuffer();
    transition = transition - speed;
    Yposition = Yposition - speed;
  } while (transition > 0);
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
        displaySettingsMenu = true;
        rotateCounter = 1;
        menuItemSelect = 1;
        settings_to_displayTransition();
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
  int speed = 16;
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
  int speed = 12;
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
        delay(200);
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
  buttonPressedState = false;
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
        alarmSetStep = 2;
        hour_to_minuteTransition();
        buttonPressedState = false;
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
  buttonPressedState = false;
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
  buttonPressedState = false;
}
void hour_to_minuteTransition() {
  int speed = 10;
  int transition = 0;
  int arrow = 45;

  do {
    u8g2.clearBuffer();
    drawAlarmBar();
    u8g2.setFont(u8g2_font_timR18_tr);
    u8g2.setCursor(37, 46);
    u8g2.print(twoDigit(alarmHour));
    u8g2.print(':');
    u8g2.print(twoDigit(alarmMinute));
    u8g2.drawXBMP(arrow, 21, 8, 4, up_arrow_BM);
    u8g2.drawXBMP(arrow, 50, 8, 4, down_arrow_BM);
    u8g2.sendBuffer();

    transition = transition + speed;
    arrow = arrow + speed;
  } while (transition < 30);
}




//    ==chime settings submenus==

//chime / state
void chimeSet_state() {
  if (chime_is_activated == true) {
    chime_is_activated = false;
    soundSystem.stop();
  } else {
    chime_is_activated = true;
  }

  delay(200);
  buttonPressedState = false;
}





//displayMenu
void printDisplayMenu() {

  u8g2.clearBuffer();
  drawDisplayBar();
  u8g2.setFontMode(2);
  u8g2.setDrawColor(1);
  u8g2.setBitmapMode(1);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawUTF8(6, 27, displayMenuTitles[1]);
  u8g2.drawUTF8(6, 43, displayMenuTitles[2]);
  u8g2.drawStr(6, 59, displayMenuTitles[3]);
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
void updateDisplayMenu() {
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
void executeDisplayMenu() {
  if (buttonPressedState == true) {
    switch (rotateCounter) {
      case 1:
        delay(300);
        brightness_menu = true;
        rotateCounter = map(brightness, 0, 255, 0, 30);

        break;

      case 2:
        contrast_menu = true;
        rotateCounter = map(contrast, 80, 180, 0, 50);
        display_to_contrastTransition();
        break;

      case 3:
        displaySettingsMenu = false;
        rotateCounter = 4;
        menuItemSelect = 4;
        display_to_settingsTransition();
        break;
    }
  }
  buttonPressedState = false;
}
void settings_to_displayTransition() {
  int speed = 25;

  int pageTransition = 125;
  int settingItems = 6;
  int settingsBox = 0;
  int settingsArrow = 112;

  int displayItems = 6 + pageTransition;
  int displayBox = 0 + pageTransition;
  int displayArrow = 118 + pageTransition;

  do {
    u8g2.clearBuffer();
    drawDisplayBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont11_tf);
    switch (frameSettingsMenu) {
      case 2:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[2]);
        u8g2.drawStr(settingItems, 43, mainSettingsMenuTitles[3]);
        u8g2.drawStr(settingItems, 59, mainSettingsMenuTitles[4]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 48, 122, 15);
        u8g2.drawXBMP(settingsArrow, 52, 4, 7, right_arrow_BM);
        break;

      case 3:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[3]);
        u8g2.drawStr(settingItems, 43, mainSettingsMenuTitles[4]);
        u8g2.drawUTF8(settingItems, 59, mainSettingsMenuTitles[5]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 32, 122, 15);
        u8g2.drawXBMP(settingsArrow, 36, 4, 7, right_arrow_BM);
        break;

      case 4:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[4]);
        u8g2.drawUTF8(settingItems, 43, mainSettingsMenuTitles[5]);
        u8g2.drawStr(settingItems, 59, mainSettingsMenuTitles[6]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 16, 122, 15);
        u8g2.drawXBMP(settingsArrow, 20, 4, 7, right_arrow_BM);
        break;
    }
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.drawUTF8(displayItems, 27, displayMenuTitles[1]);
    u8g2.drawUTF8(displayItems, 43, displayMenuTitles[2]);
    u8g2.drawStr(displayItems, 59, displayMenuTitles[3]);
    u8g2.drawBox(displayBox, 16, 128, 15);
    u8g2.drawXBMP(displayArrow, 20, 4, 7, right_arrow_BM);
    u8g2.sendBuffer();

    pageTransition = pageTransition - speed;
    settingItems = settingItems - speed;
    settingsBox = settingsBox - speed;
    settingsArrow = settingsArrow - speed;

    displayItems = displayItems - speed;
    displayBox = displayBox - speed;
    displayArrow = displayArrow - speed;

  } while (pageTransition > 0);
}
void display_to_settingsTransition() {
  int speed = 25;

  int pageTransition = -125;
  int settingItems = 6 + pageTransition;
  int settingsBox = 0 + pageTransition;
  int settingsArrow = 112 + pageTransition;

  int displayItems = 6;
  int displayBox = 0;
  int displayArrow = 118;

  do {
    u8g2.clearBuffer();
    drawSettingsBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);

    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.drawUTF8(displayItems, 27, clockDateMenuTitles[1]);
    u8g2.drawUTF8(displayItems, 43, clockDateMenuTitles[2]);
    u8g2.drawStr(displayItems, 59, clockDateMenuTitles[3]);
    u8g2.setDrawColor(2);
    u8g2.drawBox(displayBox, 48, 128, 15);
    u8g2.drawXBMP(displayArrow, 52, 4, 7, left_arrow_BM);

    switch (frameSettingsMenu) {
      case 2:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[2]);
        u8g2.drawStr(settingItems, 43, mainSettingsMenuTitles[3]);
        u8g2.drawStr(settingItems, 59, mainSettingsMenuTitles[4]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 48, 122, 15);
        u8g2.drawXBMP(settingsArrow, 52, 4, 7, right_arrow_BM);
        break;

      case 3:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[3]);
        u8g2.drawStr(settingItems, 43, mainSettingsMenuTitles[4]);
        u8g2.drawUTF8(settingItems, 59, mainSettingsMenuTitles[5]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 32, 122, 15);
        u8g2.drawXBMP(settingsArrow, 36, 4, 7, right_arrow_BM);
        break;

      case 4:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[4]);
        u8g2.drawUTF8(settingItems, 43, mainSettingsMenuTitles[5]);
        u8g2.drawStr(settingItems, 59, mainSettingsMenuTitles[6]);
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

    displayItems = displayItems + speed;
    displayBox = displayBox + speed;
    displayArrow = displayArrow + speed;

  } while (pageTransition < 0);
}


//brightness
void printBrightnessMenu() {
  u8g2.clearBuffer();
  drawDisplayBar();
  u8g2.drawXBMP(11, 30, 8, 12, brightness_low_BM);
  u8g2.drawXBMP(106, 29, 16, 13, brightness_high_BM);
  u8g2.drawBox(29, 34, brightnessBarSize, 4);
  u8g2.drawFrame(27, 32, 75, 8);
  u8g2.sendBuffer();
}
void updateBrightnessMenu() {

  if (rotateCounter > 30) {
    rotateCounter = 30;
  }
  if (rotateCounter < 0) {
    rotateCounter = 0;
  }

  brightnessBarSize = map(rotateCounter, 0, 30, 0, 71);
  brightness = map(rotateCounter, 0, 30, 0, 255);
}
void executeBrightnessMenu() {
  if (buttonPressedState == true) {
    brightness_menu = false;
    rotateCounter = 1;
    menuItemSelect = 1;
    //eprom.update(10, brightness);
    delay(200);
    buttonPressedState = false;
  }
}

//contrast
void printContrastMenu() {
  u8g2.clearBuffer();
  drawDisplayBar();
  u8g2.drawXBM(110, 32, 8, 8, contrast_high_BM);
  u8g2.drawXBM(11, 32, 8, 8, contrast_low_BM);
  u8g2.drawFrame(27, 32, 75, 8);
  u8g2.drawBox(29, 34, contrastBarSize, 4);
  u8g2.sendBuffer();
}
void updateContrastMenu() {

  if (rotateCounter > 50) {
    rotateCounter = 50;
  }
  if (rotateCounter < 0) {
    rotateCounter = 0;
  }

  contrastBarSize = map(rotateCounter, 0, 50, 0, 71);
  contrast = map(rotateCounter, 0, 50, 80, 180);
  u8g2.setContrast(contrast);
}
void executeContrastMenu() {
  if (buttonPressedState == true) {
    contrast_menu = false;
    rotateCounter = 2;
    menuItemSelect = 2;
    eprom.update(11, contrast);
    contrast_to_displayTransition();
    buttonPressedState = false;
  }
}
void display_to_contrastTransition() {
  int speed = 25;

  int pageTransition = 125;
  int displayItems = 6;
  int displayBox = 0;
  int displayArrow = 118;

  int contrastIcon1 = 11 + pageTransition;
  int contrastIcon2 = 110 + pageTransition;
  int contrastFrame = 27 + pageTransition;
  int contrastBox = 29 + pageTransition;

  do {
    u8g2.clearBuffer();
    drawDisplayBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.drawUTF8(displayItems, 27, displayMenuTitles[1]);
    u8g2.drawUTF8(displayItems, 43, displayMenuTitles[2]);
    u8g2.drawStr(displayItems, 59, displayMenuTitles[3]);

    u8g2.drawXBM(contrastIcon1, 32, 8, 8, contrast_low_BM);
    u8g2.drawXBM(contrastIcon2, 32, 8, 8, contrast_high_BM);
    u8g2.drawFrame(contrastFrame, 32, 75, 8);
    u8g2.drawBox(contrastBox, 34, contrastBarSize, 4);

    u8g2.setDrawColor(2);
    u8g2.drawBox(displayBox, 32, 128, 15);
    u8g2.drawXBMP(displayArrow, 36, 4, 7, right_arrow_BM);
    u8g2.sendBuffer();

    pageTransition = pageTransition - speed;
    displayItems = displayItems - speed;
    displayBox = displayBox - speed;
    displayArrow = displayArrow - speed;

    contrastIcon1 = contrastIcon1 - speed;
    contrastIcon2 = contrastIcon2 - speed;
    contrastFrame = contrastFrame - speed;
    contrastBox = contrastBox - speed;

  } while (pageTransition > 0);
}
void contrast_to_displayTransition() {
  int speed = 25;

  int pageTransition = -125;
  int contrastIcon1 = 11;
  int contrastIcon2 = 110;
  int contrastFrame = 27;
  int contrastBox = 29;

  int displayItems = 6 + pageTransition;
  int displayBox = 0 + pageTransition;
  int displayArrow = 118 + pageTransition;

  do {
    u8g2.clearBuffer();
    drawDisplayBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);

    u8g2.drawXBM(contrastIcon1, 32, 8, 8, contrast_low_BM);
    u8g2.drawXBM(contrastIcon2, 32, 8, 8, contrast_high_BM);
    u8g2.drawFrame(contrastFrame, 32, 75, 8);
    u8g2.drawBox(contrastBox, 34, contrastBarSize, 4);

    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.drawUTF8(displayItems, 27, displayMenuTitles[1]);
    u8g2.drawUTF8(displayItems, 43, displayMenuTitles[2]);
    u8g2.drawStr(displayItems, 59, displayMenuTitles[3]);

    u8g2.setDrawColor(2);
    u8g2.drawBox(displayBox, 32, 128, 15);
    u8g2.drawXBMP(displayArrow, 36, 4, 7, right_arrow_BM);
    u8g2.sendBuffer();

    pageTransition = pageTransition + speed;
    displayItems = displayItems + speed;
    displayBox = displayBox + speed;
    displayArrow = displayArrow + speed;

    contrastIcon1 = contrastIcon1 + speed;
    contrastIcon2 = contrastIcon2 + speed;
    contrastFrame = contrastFrame + speed;
    contrastBox = contrastBox + speed;

  } while (pageTransition < 0);
}





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
    DateTime now = rtc.now();
    switch (rotateCounter) {
      case 1:
        clockSet_menu = true;
        clockSetStep = 1;
        temporaryHour = now.hour();
        temporaryMinute = now.minute();
        temporarySecond = now.second();
        rotateCounter = temporaryHour;
        clockDate_to_clockTransition();
        break;

      case 2:
        dateSet_menu = true;
        dateSetStep = 1;
        temporaryDay = now.day();
        temporaryMonth = now.month();
        temporaryYear = now.year();
        rotateCounter = temporaryDay;
        clockDate_to_dateTransition();
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

//clock
void printClockMenu() {
  u8g2.clearBuffer();
  drawClockBar();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  u8g2.setFont(u8g2_font_timR18_tr);
  u8g2.setCursor(22, 46);
  u8g2.print(twoDigit(temporaryHour));
  u8g2.print(':');
  u8g2.print(twoDigit(temporaryMinute));
  u8g2.print(':');
  u8g2.print(twoDigit(temporarySecond));

  switch (clockSetStep) {
    case 1:
      u8g2.drawXBM(30, 22, 8, 4, up_arrow_BM);
      u8g2.drawXBM(30, 50, 8, 4, down_arrow_BM);
      break;

    case 2:
      u8g2.drawXBM(60, 22, 8, 4, up_arrow_BM);
      u8g2.drawXBM(60, 50, 8, 4, down_arrow_BM);
      break;

    case 3:
      u8g2.drawXBM(90, 22, 8, 4, up_arrow_BM);
      u8g2.drawXBM(90, 50, 8, 4, down_arrow_BM);
      break;
  }
  u8g2.sendBuffer();
}
void updateClockMenu() {
  switch (clockSetStep) {
    case 1:
      if (rotateCounter > 23) {
        rotateCounter = 0;
      }
      if (rotateCounter < 0) {
        rotateCounter = 23;
      }
      temporaryHour = rotateCounter;
      break;

    case 2:
      if (rotateCounter > 59) {
        rotateCounter = 0;
      }
      if (rotateCounter < 0) {
        rotateCounter = 59;
      }
      temporaryMinute = rotateCounter;
      break;

    case 3:
      if (rotateCounter > 59) {
        rotateCounter = 0;
      }
      if (rotateCounter < 0) {
        rotateCounter = 59;
      }
      temporarySecond = rotateCounter;
      break;
  }
}
void executeClockMenu() {
  if (buttonPressedState == true) {
    switch (clockSetStep) {
      case 1:
        rotateCounter = temporaryMinute;
        clockSetStep = 2;
        ClockHour_to_minuteTransition();
        break;

      case 2:
        rotateCounter = temporarySecond;
        clockSetStep = 3;
        ClockMinute_to_secondTransition();
        break;

      case 3:
        clockSet_menu = false;
        rotateCounter = 1;
        DateTime now = rtc.now();
        rtc.adjust(DateTime(now.year(), now.month(), now.day(), temporaryHour, temporaryMinute, temporarySecond));
        clock_to_clockDateTransition();
        break;
    }
    buttonPressedState = false;
  }
}
void clockDate_to_clockTransition() {
  int speed = 25;

  int pageTransition = 125;

  int cursor = 22 + pageTransition;
  int arrow = 30 + pageTransition;

  int menus = 6;
  int box = 0;
  int arrowMenus = 118;

  do {

    u8g2.clearBuffer();
    drawClockBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);

    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.drawUTF8(menus, 27, clockDateMenuTitles[1]);
    u8g2.drawUTF8(menus, 43, clockDateMenuTitles[2]);
    u8g2.drawStr(menus, 59, clockDateMenuTitles[3]);
    u8g2.setDrawColor(2);
    u8g2.drawBox(box, 16, 128, 15);
    u8g2.drawXBMP(arrowMenus, 20, 4, 7, right_arrow_BM);
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);

    u8g2.setFont(u8g2_font_timR18_tr);
    u8g2.setCursor(cursor, 46);
    u8g2.print(twoDigit(temporaryHour));
    u8g2.print(':');
    u8g2.print(twoDigit(temporaryMinute));
    u8g2.print(':');
    u8g2.print(twoDigit(temporarySecond));

    u8g2.drawXBM(arrow, 22, 8, 4, up_arrow_BM);
    u8g2.drawXBM(arrow, 50, 8, 4, down_arrow_BM);

    u8g2.sendBuffer();

    pageTransition = pageTransition - speed;
    cursor = cursor - speed;
    arrow = arrow - speed;
    menus = menus - speed;
    box = box - speed;
    arrowMenus = arrowMenus - speed;

  } while (pageTransition > 0);
}
void clock_to_clockDateTransition() {
  int speed = 25;

  int pageTransition = -125;


  int cursor = 22;
  int arrow = 90;

  int menus = 6 + pageTransition;
  int box = 0 + pageTransition;
  int arrowMenus = 118 + pageTransition;

  do {

    u8g2.clearBuffer();
    drawClockDateBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);

    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.drawUTF8(menus, 27, clockDateMenuTitles[1]);
    u8g2.drawUTF8(menus, 43, clockDateMenuTitles[2]);
    u8g2.drawStr(menus, 59, clockDateMenuTitles[3]);
    u8g2.setDrawColor(2);
    u8g2.drawBox(box, 16, 128, 15);
    u8g2.drawXBMP(arrowMenus, 20, 4, 7, right_arrow_BM);

    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_timR18_tr);
    u8g2.setCursor(cursor, 46);
    u8g2.print(twoDigit(temporaryHour));
    u8g2.print(':');
    u8g2.print(twoDigit(temporaryMinute));
    u8g2.print(':');
    u8g2.print(twoDigit(temporarySecond));
    u8g2.drawXBM(arrow, 22, 8, 4, up_arrow_BM);
    u8g2.drawXBM(arrow, 50, 8, 4, down_arrow_BM);
    u8g2.sendBuffer();

    pageTransition = pageTransition + speed;
    cursor = cursor + speed;
    arrow = arrow + speed;
    menus = menus + speed;
    box = box + speed;
    arrowMenus = arrowMenus + speed;

  } while (pageTransition < 0);
}
void ClockHour_to_minuteTransition() {
  int speed = 8;
  int transition = 0;
  int arrow = 30;

  do {
    u8g2.clearBuffer();
    drawClockBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_timR18_tr);
    u8g2.setCursor(22, 46);
    u8g2.print(twoDigit(temporaryHour));
    u8g2.print(':');
    u8g2.print(twoDigit(temporaryMinute));
    u8g2.print(':');
    u8g2.print(twoDigit(temporarySecond));
    u8g2.drawXBM(arrow, 22, 8, 4, up_arrow_BM);
    u8g2.drawXBM(arrow, 50, 8, 4, down_arrow_BM);

    u8g2.sendBuffer();

    arrow = arrow + speed;
    transition = transition + speed;

  } while (transition < 30);
}
void ClockMinute_to_secondTransition() {
  int speed = 8;
  int transition = 0;
  int arrow = 60;

  do {
    u8g2.clearBuffer();
    drawClockBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_timR18_tr);
    u8g2.setCursor(22, 46);
    u8g2.print(twoDigit(temporaryHour));
    u8g2.print(':');
    u8g2.print(twoDigit(temporaryMinute));
    u8g2.print(':');
    u8g2.print(twoDigit(temporarySecond));
    u8g2.drawXBM(arrow, 22, 8, 4, up_arrow_BM);
    u8g2.drawXBM(arrow, 50, 8, 4, down_arrow_BM);
    u8g2.sendBuffer();

    arrow = arrow + speed;
    transition = transition + speed;

  } while (transition < 30);
}

//date
void printDateMenu() {
  u8g2.clearBuffer();
  drawDateBar();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  u8g2.setFont(u8g2_font_profont17_tf);
  u8g2.setCursor(10, 43);
  u8g2.print(twoDigit(temporaryDay));

  if (temporaryMonth == 4 || temporaryMonth == 5 || temporaryMonth == 10 || temporaryMonth == 11 || temporaryMonth == 12) {
    u8g2.setCursor(41, 43);
  } else {
    u8g2.setCursor(37, 43);
  }

  u8g2.print(shortMonths[temporaryMonth]);

  u8g2.setCursor(82, 43);
  u8g2.print(temporaryYear);


  switch (dateSetStep) {
    case 1:
      u8g2.drawXBM(15, 23, 7, 4, up_arrow_small_BM);
      u8g2.drawXBM(15, 48, 7, 4, down_arrow_small_BM);
      break;

    case 2:
      u8g2.drawXBM(50, 23, 7, 4, up_arrow_small_BM);
      u8g2.drawXBM(50, 48, 7, 4, down_arrow_small_BM);
      break;

    case 3:
      u8g2.drawXBM(96, 23, 7, 4, up_arrow_small_BM);
      u8g2.drawXBM(96, 48, 7, 4, down_arrow_small_BM);
      break;
  }
  u8g2.sendBuffer();
}
void updateDateMenu() {
  switch (dateSetStep) {
    case 1:
      if (rotateCounter > 31) {
        rotateCounter = 1;
      }
      if (rotateCounter < 1) {
        rotateCounter = 31;
      }
      temporaryDay = rotateCounter;
      break;

    case 2:
      if (rotateCounter > 12) {
        rotateCounter = 1;
      }
      if (rotateCounter < 1) {
        rotateCounter = 12;
      }
      temporaryMonth = rotateCounter;
      break;

    case 3:
      if (rotateCounter > 2099) {
        rotateCounter = 2000;
      }
      if (rotateCounter < 2000) {
        rotateCounter = 2099;
      }
      temporaryYear = rotateCounter;
      break;
  }
}
void executeDateMenu() {
  if (buttonPressedState == true) {
    switch (dateSetStep) {
      case 1:
        rotateCounter = temporaryMonth;
        dateSetStep = 2;
        day_to_monthTransition();
        break;

      case 2:
        if (temporaryMonth == 4 || temporaryMonth == 6 || temporaryMonth == 9 || temporaryMonth == 11 && temporaryDay > 30) {
          temporaryDay = 30;
        } else if (temporaryMonth == 2 && temporaryDay > 29) {
          temporaryDay = 29;
        }

        rotateCounter = temporaryYear;
        dateSetStep = 3;
        month_to_yearTransition();
        break;

      case 3:

        //verifiy leap year
        if (temporaryYear % 400 == 0) {
        } else if (temporaryYear % 100 == 0 && temporaryDay > 28) {
          temporaryDay = 28;
        } else if (temporaryYear % 4 == 0) {
        } else if (temporaryDay == 29) {
          temporaryDay = 28;
        }

        dateSet_menu = false;
        rotateCounter = 2;
        DateTime now = rtc.now();
        rtc.adjust(DateTime(temporaryYear, temporaryMonth, temporaryDay, now.hour(), now.minute(), now.second()));
        date_to_clockDateTransition();
        break;
    }
    buttonPressedState = false;
  }
}
void clockDate_to_dateTransition() {
  int speed = 25;

  int pageTransition = 125;


  int day = 10 + pageTransition;
  int month1 = 41 + pageTransition;
  int month2 = 37 + pageTransition;
  int year = 82 + pageTransition;
  int arrow = 15 + pageTransition;

  int menus = 6;
  int box = 0;
  int arrowMenus = 118;

  do {

    u8g2.clearBuffer();
    drawDateBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);

    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.drawUTF8(menus, 27, clockDateMenuTitles[1]);
    u8g2.drawUTF8(menus, 43, clockDateMenuTitles[2]);
    u8g2.drawStr(menus, 59, clockDateMenuTitles[3]);
    u8g2.setDrawColor(2);
    u8g2.drawBox(box, 32, 128, 15);
    u8g2.drawXBMP(arrow, 36, 4, 7, right_arrow_BM);

    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont17_tf);

    u8g2.setCursor(day, 43);
    u8g2.print(twoDigit(temporaryDay));

    if (temporaryMonth == 4 || temporaryMonth == 5 || temporaryMonth == 10 || temporaryMonth == 11 || temporaryMonth == 12) {
      u8g2.setCursor(month1, 43);
    } else {
      u8g2.setCursor(month2, 43);
    }
    u8g2.print(shortMonths[temporaryMonth]);

    u8g2.setCursor(year, 43);
    u8g2.print(temporaryYear);

    u8g2.drawXBM(arrow, 23, 7, 4, up_arrow_small_BM);
    u8g2.drawXBM(arrow, 48, 7, 4, down_arrow_small_BM);

    u8g2.sendBuffer();

    pageTransition = pageTransition - speed;
    day = day - speed;
    month1 = month1 - speed;
    month2 = month2 - speed;
    year = year - speed;
    arrow = arrow - speed;
    menus = menus - speed;
    box = box - speed;
    arrowMenus = arrowMenus - speed;

  } while (pageTransition > 0);
}
void date_to_clockDateTransition() {
  int speed = 25;

  int pageTransition = -125;


  int day = 10;
  int month1 = 41;
  int month2 = 37;
  int year = 82;
  int arrow = 96;

  int menus = 6 + pageTransition;
  int box = 0 + pageTransition;
  int arrowMenus = 118 + pageTransition;

  do {

    u8g2.clearBuffer();
    drawClockDateBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);

    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.drawUTF8(menus, 27, clockDateMenuTitles[1]);
    u8g2.drawUTF8(menus, 43, clockDateMenuTitles[2]);
    u8g2.drawStr(menus, 59, clockDateMenuTitles[3]);
    u8g2.setDrawColor(2);
    u8g2.drawBox(box, 32, 128, 15);
    u8g2.drawXBMP(arrow, 36, 4, 7, right_arrow_BM);

    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont17_tf);

    u8g2.setCursor(day, 43);
    u8g2.print(twoDigit(temporaryDay));

    if (temporaryMonth == 4 || temporaryMonth == 5 || temporaryMonth == 10 || temporaryMonth == 11 || temporaryMonth == 12) {
      u8g2.setCursor(month1, 43);
    } else {
      u8g2.setCursor(month2, 43);
    }
    u8g2.print(shortMonths[temporaryMonth]);

    u8g2.setCursor(year, 43);
    u8g2.print(temporaryYear);

    u8g2.drawXBM(arrow, 23, 7, 4, up_arrow_small_BM);
    u8g2.drawXBM(arrow, 48, 7, 4, down_arrow_small_BM);

    u8g2.sendBuffer();

    pageTransition = pageTransition + speed;
    day = day + speed;
    month1 = month1 + speed;
    month2 = month2 + speed;
    year = year + speed;
    arrow = arrow + speed;
    menus = menus + speed;
    box = box + speed;
    arrowMenus = arrowMenus + speed;

  } while (pageTransition < 0);
}
void day_to_monthTransition() {
  int speed = 10;
  int transition = 0;
  int arrow = 15;

  do {
    u8g2.clearBuffer();
    drawDateBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont17_tf);
    u8g2.setCursor(10, 43);
    u8g2.print(twoDigit(temporaryDay));

    if (temporaryMonth == 4 || temporaryMonth == 5 || temporaryMonth == 10 || temporaryMonth == 11 || temporaryMonth == 12) {
      u8g2.setCursor(41, 43);
    } else {
      u8g2.setCursor(37, 43);
    }

    u8g2.print(shortMonths[temporaryMonth]);

    u8g2.setCursor(82, 43);
    u8g2.print(temporaryYear);
    u8g2.drawXBM(arrow, 23, 7, 4, up_arrow_small_BM);
    u8g2.drawXBM(arrow, 48, 7, 4, down_arrow_small_BM);
    u8g2.sendBuffer();

    arrow = arrow + speed;
    transition = transition + speed;

  } while (transition < 35);
}
void month_to_yearTransition() {
  int speed = 12;
  int transition = 0;
  int arrow = 50;

  do {
    u8g2.clearBuffer();
    drawDateBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont17_tf);
    u8g2.setCursor(10, 43);
    u8g2.print(twoDigit(temporaryDay));

    if (temporaryMonth == 4 || temporaryMonth == 5 || temporaryMonth == 10 || temporaryMonth == 11 || temporaryMonth == 12) {
      u8g2.setCursor(41, 43);
    } else {
      u8g2.setCursor(37, 43);
    }

    u8g2.print(shortMonths[temporaryMonth]);

    u8g2.setCursor(82, 43);
    u8g2.print(temporaryYear);
    u8g2.drawXBM(arrow, 23, 7, 4, up_arrow_small_BM);
    u8g2.drawXBM(arrow, 48, 7, 4, down_arrow_small_BM);
    u8g2.sendBuffer();

    arrow = arrow + speed;
    transition = transition + speed;

  } while (transition < 46);
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
  u8g2.drawXBMP(2, 2, 8, 9, settings_BM);
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
void drawDisplayBar() {
  u8g2.drawXBMP(2, 2, 8, 9, display_BM);
  u8g2.setFont(u8g2_font_profont11_tr);
  u8g2.drawStr(37, 10, "Affichage");
  u8g2.drawLine(0, 13, 128, 13);
}
void drawClockDateBar() {
  u8g2.drawXBMP(2, 2, 8, 9, calendar_BM);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawStr(25, 10, "Heure et date");
  u8g2.drawLine(0, 13, 128, 13);
}
void drawClockBar() {
  u8g2.drawXBM(3, 2, 8, 9, clock_BM);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawUTF8(22, 10, "Régler l'heure");
  u8g2.drawLine(0, 13, 128, 13);
}
void drawDateBar() {
  u8g2.drawXBMP(2, 2, 8, 9, calendar_BM);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawUTF8(22, 10, "Régler la date");
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
  buttonPressedState = true;
}
void wheel_is_rotated() {
  //Store states
  CLKNow = digitalRead(RotaryCLK);  //Read the state of the CLK pin



  // If last and current state of CLK are different, then a pulse occurred
  if (CLKNow != CLKPrevious && CLKNow == 1) {
    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so increase
    if (digitalRead(RotaryDT) != CLKNow) {
      rotateCounter++;
    } else {
      rotateCounter--;
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