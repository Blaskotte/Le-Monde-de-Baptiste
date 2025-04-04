//   ==alarm settings menu==
////////////////////////////////////////////////////////////////////////////

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
        setRotary(4);
      }
      if (rotateCounter < 1) {
        setRotary(1);
      }
  }
}

void executeAlarmMenu() {
  if (mainButton.pressed()) {
    switch (rotateCounter) {
      case 1:
        alarmSet_state();
        if (alarm_is_activated == false) {
          alarmTrue_to_false();
        } else {
          alarmFalse_to_true();
        }
        break;

      case 2:
        alarmSetHour_menu = true;
        alarmSetStep = 1;
        setRotary(alarmHour);
        alarm_to_hourTransition();
        break;

      case 3:
        break;

      case 4:
        alarmSettingsMenu = false;
        setRotary(1);
        menuItemSelect = 1;
        alarm_to_settingsTransition();
        break;
    }
  }
}

void alarmTrue_to_false() {
  int transition = 128;
  int oldItem = 6;
  int newItem = 6 + transition;
  int speed = 2;
  do {
    u8g2.clearBuffer();
    drawAlarmBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.drawUTF8(oldItem, 27, alarmSettingsMenuTitles[1]);
    u8g2.drawUTF8(newItem, 27, alarmSettingsMenuTitles[0]);
    u8g2.drawStr(6, 43, alarmSettingsMenuTitles[2]);
    u8g2.drawStr(6, 59, alarmSettingsMenuTitles[3]);
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, 16, 122, 15);
    drawScrollbar();
    scrollbar_2frames();
    u8g2.sendBuffer();

    oldItem = oldItem - speed;
    newItem = newItem - speed;
    transition = transition - speed;
    delayMicroseconds(100);
  } while (transition > 0);
}

void alarmFalse_to_true() {
  int transition = 128;
  int oldItem = 6;
  int newItem = 6 + transition;
  int speed = 2;
  do {
    u8g2.clearBuffer();
    drawAlarmBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.drawUTF8(oldItem, 27, alarmSettingsMenuTitles[0]);
    u8g2.drawUTF8(newItem, 27, alarmSettingsMenuTitles[1]);
    u8g2.drawStr(6, 43, alarmSettingsMenuTitles[2]);
    u8g2.drawStr(6, 59, alarmSettingsMenuTitles[3]);
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, 16, 122, 15);
    drawScrollbar();
    scrollbar_2frames();
    u8g2.sendBuffer();

    oldItem = oldItem - speed;
    newItem = newItem - speed;
    transition = transition - speed;
    delayMicroseconds(100);
  } while (transition > 0);
}

void settings_to_alarmTransition() {
  int speed = 2;

  int pageTransition = 128;
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
    delayMicroseconds(100);
  } while (pageTransition > 0);
}

void alarm_to_settingsTransition() {
  int speed = 2;

  int pageTransition = -128;
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
    delayMicroseconds(100);
  } while (pageTransition < 0);
}

////////////////////////////////////////////////////////////////////////////



//    ==alarm settings submenus==
////////////////////////////////////////////////////////////////////////////

//alarm / state
void alarmSet_state() {
  if (alarm_is_activated == true) {
    alarm_is_activated = false;
    soundSystem.play(5);
  } else {
    alarm_is_activated = true;
    soundSystem.play(4);
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
        setRotary(0);
      }
      if (rotateCounter < 0) {
        setRotary(23);
      }
      alarmHour = rotateCounter;
      if (mainButton.pressed()) {
        setRotary(alarmMinute);
        alarmSetStep = 2;

        hour_to_minuteTransition();
      }
      break;

    case 2:  //Setting minutes
      if (rotateCounter >= 60) {
        setRotary(0);
      }
      if (rotateCounter < 0) {
        setRotary(59);
      }
      alarmMinute = rotateCounter;
      if (mainButton.pressed()) {
        alarmSetStep = 3;
      }
      break;

    case 3:
      eprom.update(3, alarmHour);
      eprom.update(4, alarmMinute);
      soundSystem.play(6);
      alarmSetHour_menu = false;
      setRotary(2);
      hour_to_alarmTransition();
      break;
  }
}

void alarm_to_hourTransition() {
  int speed = 2;

  int pageTransition = 128;
  int alarmItems = 6;
  int alarmBox = 0;
  int alarmArrow = 118;
  int clock = 37 + pageTransition;

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
    u8g2.setCursor(clock, 46);
    u8g2.print(twoDigit(alarmHour));
    u8g2.print(':');
    u8g2.print(twoDigit(alarmMinute));
    u8g2.sendBuffer();

    pageTransition = pageTransition - speed;
    alarmItems = alarmItems - speed;
    alarmBox = alarmBox - speed;
    alarmArrow = alarmArrow - speed;
    clock = clock - speed;
    delayMicroseconds(100);
  } while (pageTransition > 0);
}

void hour_to_alarmTransition() {
  int speed = 2;

  int pageTransition = -128;
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
    delayMicroseconds(100);
  } while (pageTransition < 0);
}

void hour_to_minuteTransition() {
  int speed = 1;
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
    delayMicroseconds(1000);
  } while (transition < 30);
}

////////////////////////////////////////////////////////////////////////////



//AlarmNotification message set
////////////////////////////////////////////////////////////////////////////

void updateAlarmNotification() {
  alarmNotificationMSG = alarmNotificationMSG - 6;
  clockBIG = clockBIG + 1;
  delay(150);
  if (alarmNotificationMSG < -166) {
    alarmNotificationMSG = 128;
  }
  if (clockBIG > 7) {
    clockBIG = 0;
    if (clockBIG_state == true) {
      clockBIG_state = false;
    } else {
      clockBIG_state = true;
    }
  }
}

void printAlarmNotification() {
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  u8g2.setDrawColor(1);
  u8g2.drawXBMP(56, 19, 17, 18, big_alarm_BM);
  if (clockBIG_state == true) {
    u8g2.drawXBMP(53, 15, 6, 6, big_alarm_left_BM);
    u8g2.drawXBMP(70, 15, 6, 6, big_alarm_right_BM);
  } else {
    u8g2.drawXBMP(54, 16, 6, 6, big_alarm_left_BM);
    u8g2.drawXBMP(69, 16, 6, 6, big_alarm_right_BM);
  }
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawUTF8(alarmNotificationMSG, 51, "Il est l'heure de se réveiller !");
  u8g2.drawFrame(24, 5, 80, 54);
  u8g2.setDrawColor(0);
  u8g2.drawBox(104, 0, 24, 64);
  u8g2.drawBox(0, 0, 24, 64);
  u8g2.setDrawColor(2);
  u8g2.drawPixel(103, 5);
  u8g2.drawPixel(24, 5);
  u8g2.drawPixel(103, 58);
  u8g2.drawPixel(24, 58);
  u8g2.sendBuffer();
}

void executeAlarmNotification() {
  if (mainButton.pressed()) {
    alarmNotification = false;
  }
}

////////////////////////////////////////////////////////////////////////////