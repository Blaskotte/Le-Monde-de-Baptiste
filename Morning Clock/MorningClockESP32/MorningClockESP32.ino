#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <RTClib.h>
#include <GD3300.h>
#include <SoftwareSerial.h>
#include <at24c32.h>
#include <MCP4131.h>
#include <HotButton.h>
#include <ESP32Encoder.h>
#include "Bitmap.h"  // https://github.com/madhephaestus/ESP32Encoder.git

#define CLK 16  // CLK ENCODER
#define DT 15   // DT ENCODER

const int screenWidth = 128;
const int screenHeight = 96;

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

const char* alarmSoundTitles[] = {
  /*0*/ "Concerto, Haydn",
  /*1*/ "Suite, Peer Gynt",
  /*2*/ "Printemps, Vivaldi",
  /*3*/ "Frutiger Aero",
  /*4*/ "Retour"
};

const char* chimeSettingsMenuTitles[] = {
  /*0*/ "Activer",
  /*1*/ "Désactiver",
  /*2*/ "Heure",
  /*3*/ "Son",
  /*4*/ "Retour"
};

const char* chimeSoundTitles[] = {
  /*0*/ "Westiminster",
  /*1*/ "",
  /*2*/ "",
  /*3*/ "",
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
  "Économis. d'écran",
  "Retour"
};

const char* screensaverTitles[] = {
  /*0*/ "10 minutes",
  /*1*/ "30 minutes",
  /*2*/ "1 heure",
  /*3*/ "Jamais",
  /*4*/ "Retour"
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
bool chimeSetHour_menu = false;
bool chimeSetStep;

bool volumeSettingsMenu = false;

bool displaySettingsMenu = false;
bool brightness_menu = false;
bool contrast_menu = false;
bool screensaver_menu = false;

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

bool selectHourUP;
bool selectHourDOWN;
int rotatePreviousHour;
int millisHour;

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

unsigned int screensaverInterval;  // EEPROM adress 14


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
bool screensaverState;
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
  switch (eprom.read(14)) {
    case 6:
      screensaverState = true;
      screensaverInterval = eprom.read(14) * 100000;
      break;

    case 18:
      screensaverState = true;
      screensaverInterval = eprom.read(14) * 100000;
      break;

    case 36:
      screensaverState = true;
      screensaverInterval = eprom.read(14) * 100000;
      break;

    case 100:
      screensaverState = false;
      screensaverInterval = eprom.read(14);
      break;
  }
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
    if (screensaverState == true) {
      if (millis() - millisScreensaver >= screensaverInterval && screensaver == false) {  // activate the screensaver after inactive time
        millisScreensaver = millis();
        screensaver = true;
        SVX = 28;
        SVY = 58;
      }
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
      if (chimeSetHour_menu == true) {
        //printChimeSet_hour();
        //updateChimeSet_hour();

      } else if (alarmSetMusic_menu == true) {
        //updateChimeSet_music();
        //printChimeSet_music();
        //executeChimeSet_music();
      } else {
        updateChimeMenu();
        printChimeMenu();
        executeChimeMenu();
      }
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
      } else if (screensaver_menu == true) {
        updateScreensaverTime();
        printScreensaverTime();
        executeScreensaverTime();
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
