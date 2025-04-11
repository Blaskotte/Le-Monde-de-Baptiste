//   ==main page==
////////////////////////////////////////////////////////////////////////////

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
  u8g2.setCursor(28, 58);
  u8g2.print(twoDigit(now.hour()));

  if (blink == true) {
    u8g2.print(":");
  } else {
    u8g2.print(" ");
  }

  u8g2.setCursor(69, 58);
  u8g2.print(twoDigit(now.minute()));
  drawDate();
  if (alarmNotification == true) {
  printAlarmNotification();
  }
  u8g2.sendBuffer();
}

void updateHomePage() {
  if (millis() - previousMillisHomePage >= intervalHomePage) {
    previousMillisHomePage = millis();
    setRotary(0);
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
        setRotary(-3);
      }
      if (rotateCounter < -3) {
        setRotary(0);
      }
  }
}

void executeHomePage() {
  if (mainButton.pressed()) {

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
        setRotary(1);
        frameSettingsMenu = 1;
        menuItemSelect = 1;
        soundSystem.play(2);
        homePage_to_settingsTransition();
        break;
    }
  }
}

void alarmHomePageTransitionIN() {
  int speed = 1;
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
    u8g2.setCursor(28, 58);
    u8g2.print(twoDigit(now.hour()));

    if (blink == true) {
      u8g2.print(":");
    } else {
      u8g2.print(" ");
    }

    u8g2.setCursor(69, 58);
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
    delayMicroseconds(5000);
  } while (transition < 0);
}

void alarmHomePageTransitionOUT() {
  int speed = 1;
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
    u8g2.setCursor(28, 58);
    u8g2.print(twoDigit(now.hour()));

    if (blink == true) {
      u8g2.print(":");
    } else {
      u8g2.print(" ");
    }

    u8g2.setCursor(69, 58);
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
    delayMicroseconds(5000);
  } while (transition > 0);
}

////////////////////////////////////////////////////////////////////////////