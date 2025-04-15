#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <RTClib.h>
#include <GD3300.h>
#include <SoftwareSerial.h>
#include <at24c32.h>
#include <MCP4131.h>
#include <HotButton.h>
#include <ESP32Encoder.h>  // https://github.com/madhephaestus/ESP32Encoder.git

#define CLK 16  // CLK ENCODER
#define DT 15   // DT ENCODER

HotButton mainButton(14, true, LOW);

ESP32Encoder rotaryEncoder;

AT24C32 eprom(AT24C_ADDRESS_7);

SoftwareSerial mp3Serial(17 /*module TX, ESP RX*/, 18 /*module RX, ESP TX*/);

GD3300 soundSystem;

RTC_DS3231 rtc;

//U8G2_ST7567_JLX12864_F_4W_SW_SPI u8g2(U8G2_R2, /* clock=*/1, /* data=*/2, /* cs=*/3, /* dc=*/4, /* reset=*/5);
//U8G2_ST7567_JLX12864_F_4W_HW_SPI u8g2(U8G2_R2, 3, 4, 5);
U8G2_ST7571_128X96_F_4W_HW_SPI u8g2(U8G2_R0, 3, 4, 5);

const int chipSelect = 10;

// Create MCP4131 object nominating digital pin used for Chip Select
MCP4131 Potentiometer(chipSelect);



//Bitmaps
////////////////////////////////////////////////////////////////////////////

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

const unsigned char big_alarm_BM[] PROGMEM = {
  // 'big_alarm_BM', 22x25px
  0x00, 0x3f, 0x00, 0xc0, 0xc0, 0x00, 0x30, 0x00, 0x03, 0x08, 0x04, 0x04, 0x04, 0x04, 0x08, 0x04, 0x04, 0x08, 0x02, 0x04, 0x10, 0x02, 0x04, 0x10, 0x01, 0x04, 0x20, 0x01, 0x04, 0x20, 0x01, 0x04, 0x20, 0x01, 0xfc, 0x20, 0x01, 0x00, 0x20, 0x01, 0x00, 0x20, 0x02, 0x00, 0x10, 0x02, 0x00, 0x10, 0x04, 0x00, 0x08, 0x04, 0x00, 0x08, 0x08, 0x00, 0x04, 0x30, 0x00, 0x03, 0xf8, 0xc0, 0x07, 0x1c, 0x3f, 0x0e, 0x0c, 0x00, 0x0c, 0x06, 0x00, 0x18, 0x03, 0x00, 0x30
};

const unsigned char big_alarm_left_BM[] PROGMEM = {
  // 'big_alarm_left_BM', 8x8px
  0x78, 0x86, 0x42, 0x31, 0x09, 0x09, 0x05, 0x02
};

const unsigned char big_alarm_right_BM[] PROGMEM = {
  // 'big_alarm_right_BM', 8x8px
  0x1e, 0x61, 0x42, 0x8c, 0x90, 0x90, 0xa0, 0x40
};

const unsigned char validate_BM[] PROGMEM = {
  // 'validate_BM', 7x9px
  0x40, 0x40, 0x20, 0x20, 0x10, 0x11, 0x0b, 0x0e, 0x04
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
// 'big_alarm_BM', 22x25px
// 'big_alarm_left_BM', 8x8px
// 'big_alarm_right_BM', 8x8px
// 'validate_BM', 7x9px

////////////////////////////////////////////////////////////////////////////



//   ==lists==
////////////////////////////////////////////////////////////////////////////

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

const char* alarmSoundTitles[5] = {
  /*0*/ "Concerto, Haydn",
  /*1*/ "Suite, Peer Gynt",
  /*2*/ "Printemps, Vivaldi",
  /*3*/ "Frutiger Aero",
  /*4*/ "Retour"
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

////////////////////////////////////////////////////////////////////////////



//   ==Variables==

//rotary encoder and button
int rotateCounter = 0;  //counts the rotation clicks




bool chimeSelect = false;
bool alarmSelect = false;
bool happyMenuSelect = false;


//variables for the menus
bool homePage = true;

bool mainSettingsMenu = false;

bool alarmSettingsMenu = false;
bool alarmSetHour_menu = false;
bool alarmSetMusic_menu = false;


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
static int aboutPageSize = 115;
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
static int endScrollbar = 94;
static int scrollbarTotalSize = 47;
static int screenSize = 82;

int elevatorSize;
int scrollCurrentPosition;


//varibales alarm
int alarmSelectNumber;
int alarmSetStep;



//variables for the adaptatives menus
int rotatePrevious;
int frameSettingsMenu = 1;
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

bool chime_is_activated;  //EEPROM adress 12
bool alarm_is_activated;  //EEPROM adress 13


//variables used to set the hour
int temporaryHour;
int temporaryMinute;
int temporarySecond;

//variables used to set the date
int temporaryYear;
int temporaryMonth;
int temporaryDay;


//variables for the screen saver
int SVX;
int SVY;
int xSpeed = 1;
int ySpeed = 1;
int screenWidth = 128;
int screenHeight = 96;

//volume variable
unsigned int volume = 15;

unsigned long previousMillis = 0;
const long interval = 500;
bool blink = false;

unsigned long millisHomePage = 0;
const long intervalHomePage = 30000;

const long rotaryInterval = 2;
unsigned long rotaryPreviousMillis = 0;

//variables notifications

bool alarmNotification = false;
bool clockBig;

bool chimeNotification = false;

bool screensaver = false;
int screensaverInterval = 600000;
int millisScreensaver;
int screensaverPreviousRotate;

int millisNoAction;
int noActionInterval = 120000;

////////////////////////////////////////////////////////////////////////////

void setup() {
  Potentiometer.writeWiper(129);
  mp3Serial.begin(9600);
  delay(200);
  soundSystem.begin(mp3Serial);
  u8g2.begin();

  u8g2.enableUTF8Print();
  u8g2.setContrast(130);

  u8g2.clearBuffer();
  u8g2.drawXBMP(57, 39, 14, 18, bigHappy_BM);
  u8g2.sendBuffer();
  delay(300);
  Wire.begin();
  rtc.begin();
  soundSystem.sendCommand(CMD_SEL_DEV, 0, 2);
  rotaryEncoder.attachHalfQuad(DT, CLK);
  rotaryEncoder.setCount(0);

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
  alarm_is_activated = eprom.read(12);
  chime_is_activated = eprom.read(13);
  Potentiometer.writeWiper(brightness);
  delay(300);
  u8g2.setContrast(contrast);
  soundSystem.play(1);
  delay(0);
}

void loop() {
  mainButton.update();
  rotateCounter = rotaryEncoder.getCount() / 2;

  if (noActionPerfom() && alarmNotification == false) {
    if (millis() - millisNoAction >= noActionInterval && homePage == false) {  //if no action as been performed and the user is inside the menus
      millisNoAction = millis();
      homePage = true;
      mainSettingsMenu = false;
      alarmSettingsMenu = false;
      alarmSetHour_menu = false;
      alarmSetMusic_menu = false;
      chimeSettingsMenu = false;
      volumeSettingsMenu = false;
      displaySettingsMenu = false;
      brightness_menu = false;
      contrast_menu = false;
      clockDateSettingsMenu = false;
      clockSet_menu = false;
      dateSet_menu = false;
      aboutPage = false;
      settings_to_homePageTransition();
      setRotary(0);
    }
    if (millis() - millisHomePage >= intervalHomePage && homePage == true) {
      millisHomePage = millis();
      setRotary(0);
    }
    if (millis() - millisScreensaver >= screensaverInterval && screensaver == false) {  // activate the screensaver after inactive time
      millisScreensaver = millis();
      screensaver = true;
      SVX = 28;
      SVY = 58;
    }
  } else {
    millisScreensaver = millis();
    millisNoAction = millis();
    millisHomePage = millis();
    screensaver = false;
  }

  screensaverPreviousRotate = rotateCounter;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    if (blink == true) {
      blink = false;
    } else {
      blink = true;
    }
  }


  if (alarmNotification == true) {
    screensaver = false;
    updateAlarmNotification();
    printHomePage();
    executeAlarmNotification();

  } else if (screensaver == true) {
    updateScreensaver();
    printScreensaver();
    executeScreenSaver();
  } else if (homePage == true && mainSettingsMenu == false) {
    updateHomePage();
    printHomePage();
    executeHomePage();
  } else if (homePage == false && mainSettingsMenu == true) {
    if (alarmSettingsMenu == true) {
      if (alarmSetHour_menu == true) {
        printAlarmSet_hour();
        updateAlarmSet_hour();

      } else if (alarmSetMusic_menu == true) {
        updateAlarmSet_music();
        printAlarmSet_music();
        executeAlarmSet_music();
      } else {
        updateAlarmMenu();
        printAlarmMenu();
        executeAlarmMenu();
      }
    } else if (chimeSettingsMenu == true) {
    } else if (volumeSettingsMenu == true) {
    } else if (displaySettingsMenu == true) {
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
    } else if (clockDateSettingsMenu == true) {
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
    } else if (aboutPage == true) {
      updateAboutPage();
      printAboutPage();
      executeAboutPage();
    } else {
      updateSettingsMenu();
      printSettingsMenu();
      executeSettingsMenu();
    }
  }
}

////////////////////////////////////////////////////////////////////////////



// scroll bar stuff
////////////////////////////////////////////////////////////////////////////

void drawScrollbar() {
  u8g2.setDrawColor(0);
  u8g2.drawBox(122, 13, 6, 83);
  u8g2.setDrawColor(1);
  u8g2.drawFrame(122, 13, 6, 83);
}

void drawSettingsScrollbar() {
  switch (frameSettingsMenu) {
    case 1:
      u8g2.drawFrame(124, 15, 2, 55);
      break;

    case 2:
      u8g2.drawFrame(124, 27, 2, 55);
      break;

    case 3:
      u8g2.drawFrame(124, 39, 2, 55);
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

////////////////////////////////////////////////////////////////////////////



// date main menu
////////////////////////////////////////////////////////////////////////////

void drawDate() {
  DateTime now = rtc.now();
  int dateY = 75;
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

////////////////////////////////////////////////////////////////////////////

bool noActionPerfom() {
  if (mainButton.pressed()) {
    return false;
  } else if (rotateCounter != screensaverPreviousRotate) {
    return false;
  } else {
    return true;
  }
}

String twoDigit(int number) {
  if (number < 10) {              // If it's a single-digit number
    return "0" + String(number);  // Add a leading zero
  } else {
    return String(number);  // Otherwise, just return the number as is
  }
}

void setRotary(int number) {
  rotaryEncoder.setCount(number * 2);
}
