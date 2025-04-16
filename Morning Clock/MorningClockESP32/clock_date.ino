//date / clock menu
////////////////////////////////////////////////////////////////////////////

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
        setRotary(3);
      }
      if (rotateCounter < 1) {
        setRotary(1);
      }
      break;
  }
}

void executeClockDateMenu() {
  if (mainButton.pressed()) {
    DateTime now = rtc.now();
    switch (rotateCounter) {
      case 1:
        clockSet_menu = true;
        clockSetStep = 1;
        temporaryHour = now.hour();
        temporaryMinute = now.minute();
        temporarySecond = now.second();
        setRotary(temporaryHour);
        selectHourDOWN = false;
        selectHourUP = false;
        rotatePreviousHour = temporaryHour;
        clockDate_to_clockTransition();
        break;

      case 2:
        dateSet_menu = true;
        dateSetStep = 1;
        temporaryDay = now.day();
        temporaryMonth = now.month();
        temporaryYear = now.year();
        setRotary(temporaryDay);
        selectHourDOWN = false;
        selectHourUP = false;
        rotatePreviousHour = temporaryDay;
        clockDate_to_dateTransition();
        break;

      case 3:
        clockDateSettingsMenu = false;
        setRotary(5);
        menuItemSelect = 5;
        clockDate_to_settingsTransition();
        break;
    }
  }
}

void settings_to_clockDateTransition() {
  int speed = 2;

  int pageTransition = 128;
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
      case 1:
        u8g2.drawUTF8(settingItems, 27, mainSettingsMenuTitles[1]);
        u8g2.drawUTF8(settingItems, 43, mainSettingsMenuTitles[2]);
        u8g2.drawUTF8(settingItems, 59, mainSettingsMenuTitles[3]);
        u8g2.drawUTF8(settingItems, 75, mainSettingsMenuTitles[4]);
        u8g2.drawUTF8(settingItems, 91, mainSettingsMenuTitles[5]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 80, 122, 15);
        u8g2.drawXBMP(settingsArrow, 84, 4, 7, right_arrow_BM);
        break;

      case 2:
        u8g2.drawUTF8(settingItems, 27, mainSettingsMenuTitles[2]);
        u8g2.drawUTF8(settingItems, 43, mainSettingsMenuTitles[3]);
        u8g2.drawUTF8(settingItems, 59, mainSettingsMenuTitles[4]);
        u8g2.drawUTF8(settingItems, 75, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingItems, 91, mainSettingsMenuTitles[6]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 64, 122, 15);
        u8g2.drawXBMP(settingsArrow, 68, 4, 7, right_arrow_BM);
        break;

      case 3:
        u8g2.drawUTF8(settingItems, 27, mainSettingsMenuTitles[3]);
        u8g2.drawUTF8(settingItems, 43, mainSettingsMenuTitles[4]);
        u8g2.drawUTF8(settingItems, 59, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingItems, 75, mainSettingsMenuTitles[6]);
        u8g2.drawUTF8(settingItems, 91, mainSettingsMenuTitles[7]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 48, 122, 15);
        u8g2.drawXBMP(settingsArrow, 52, 4, 7, right_arrow_BM);
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
    delayMicroseconds(100);
  } while (pageTransition > 0);
}

void clockDate_to_settingsTransition() {
  int speed = 2;

  int pageTransition = -128;
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
      case 1:
        u8g2.drawUTF8(settingItems, 27, mainSettingsMenuTitles[1]);
        u8g2.drawUTF8(settingItems, 43, mainSettingsMenuTitles[2]);
        u8g2.drawUTF8(settingItems, 59, mainSettingsMenuTitles[3]);
        u8g2.drawUTF8(settingItems, 75, mainSettingsMenuTitles[4]);
        u8g2.drawUTF8(settingItems, 91, mainSettingsMenuTitles[5]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 80, 122, 15);
        u8g2.drawXBMP(settingsArrow, 84, 4, 7, right_arrow_BM);
        break;

      case 2:
        u8g2.drawUTF8(settingItems, 27, mainSettingsMenuTitles[2]);
        u8g2.drawUTF8(settingItems, 43, mainSettingsMenuTitles[3]);
        u8g2.drawUTF8(settingItems, 59, mainSettingsMenuTitles[4]);
        u8g2.drawUTF8(settingItems, 75, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingItems, 91, mainSettingsMenuTitles[6]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 64, 122, 15);
        u8g2.drawXBMP(settingsArrow, 68, 4, 7, right_arrow_BM);
        break;

      case 3:
        u8g2.drawUTF8(settingItems, 27, mainSettingsMenuTitles[3]);
        u8g2.drawUTF8(settingItems, 43, mainSettingsMenuTitles[4]);
        u8g2.drawUTF8(settingItems, 59, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingItems, 75, mainSettingsMenuTitles[6]);
        u8g2.drawUTF8(settingItems, 91, mainSettingsMenuTitles[7]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 48, 122, 15);
        u8g2.drawXBMP(settingsArrow, 52, 4, 7, right_arrow_BM);
        break;
    }
    drawScrollbar();
    drawSettingsScrollbar();
    u8g2.sendBuffer();

    pageTransition = pageTransition + speed;
    settingItems = settingItems + speed;
    settingsBox = settingsBox + speed;
    settingsArrow = settingsArrow + speed;

    clockDateItems = clockDateItems + speed;
    clockDateBox = clockDateBox + speed;
    clockDateArrow = clockDateArrow + speed;
    delayMicroseconds(100);
  } while (pageTransition < 0);
}

////////////////////////////////////////////////////////////////////////////



//clock
////////////////////////////////////////////////////////////////////////////

void printClockMenu() {
  u8g2.clearBuffer();
  drawClockBar();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  u8g2.setFont(u8g2_font_timR18_tr);
  u8g2.setCursor(22, 61);
  u8g2.print(twoDigit(temporaryHour));
  u8g2.print(':');
  u8g2.print(twoDigit(temporaryMinute));
  u8g2.print(':');
  u8g2.print(twoDigit(temporarySecond));

  switch (clockSetStep) {
    case 1:
      u8g2.drawXBM(30, 36, 8, 4, up_arrow_BM);
      u8g2.drawXBM(30, 65, 8, 4, down_arrow_BM);
      u8g2.setDrawColor(2);
      if (selectHourUP == true) {
        u8g2.drawXBM(28, 34, 12, 8, select_hour_BM);
      }
      if (selectHourDOWN == true) {
        u8g2.drawXBM(28, 63, 12, 8, select_hour_BM);
      }
      break;

    case 2:
      u8g2.drawXBM(60, 36, 8, 4, up_arrow_BM);
      u8g2.drawXBM(60, 65, 8, 4, down_arrow_BM);
      u8g2.setDrawColor(2);
      if (selectHourUP == true) {
        u8g2.drawXBM(58, 34, 12, 8, select_hour_BM);
      }
      if (selectHourDOWN == true) {
        u8g2.drawXBM(58, 63, 12, 8, select_hour_BM);
      }
      break;

    case 3:
      u8g2.drawXBM(90, 36, 8, 4, up_arrow_BM);
      u8g2.drawXBM(90, 65, 8, 4, down_arrow_BM);
      u8g2.setDrawColor(2);
      if (selectHourUP == true) {
        u8g2.drawXBM(88, 34, 12, 8, select_hour_BM);
      }
      if (selectHourDOWN == true) {
        u8g2.drawXBM(88, 63, 12, 8, select_hour_BM);
      }
      break;
  }
  u8g2.sendBuffer();
}

void updateClockMenu() {
  if (rotatePreviousHour > rotateCounter) {
    selectHourDOWN = true;
    selectHourUP = false;
    millisHour = millis();
    rotatePreviousHour = rotateCounter;
  }
  if (rotatePreviousHour < rotateCounter) {
    selectHourUP = true;
    selectHourDOWN = false;
    millisHour = millis();
    rotatePreviousHour = rotateCounter;
  }
  if (millis() - millisHour > 200 && (selectHourUP == true || selectHourDOWN == true)) {
    millisHour = millis();
    selectHourUP = false;
    selectHourDOWN = false;
  }

  switch (clockSetStep) {
    case 1:
      if (rotateCounter > 23) {
        setRotary(0);
      }
      if (rotateCounter < 0) {
        setRotary(23);
      }
      temporaryHour = rotateCounter;
      break;

    case 2:
      if (rotateCounter > 59) {
        setRotary(0);
      }
      if (rotateCounter < 0) {
        setRotary(59);
      }
      temporaryMinute = rotateCounter;
      break;

    case 3:
      if (rotateCounter > 59) {
        setRotary(0);
      }
      if (rotateCounter < 0) {
        setRotary(59);
      }
      temporarySecond = rotateCounter;
      break;
  }
}

void executeClockMenu() {
  if (mainButton.pressed()) {
    switch (clockSetStep) {
      case 1:
        setRotary(temporaryMinute);
        clockSetStep = 2;
        selectHourDOWN = false;
        selectHourUP = false;
        rotatePreviousHour = temporaryMinute;
        ClockHour_to_minuteTransition();
        break;

      case 2:
        setRotary(temporarySecond);
        clockSetStep = 3;
        selectHourDOWN = false;
        selectHourUP = false;
        rotatePreviousHour = temporarySecond;
        ClockMinute_to_secondTransition();
        break;

      case 3:
        clockSet_menu = false;
        setRotary(1);
        selectHourDOWN = false;
        selectHourUP = false;
        DateTime now = rtc.now();
        rtc.adjust(DateTime(now.year(), now.month(), now.day(), temporaryHour, temporaryMinute, temporarySecond));
        //soundSystem.play(6);
        clock_to_clockDateTransition();
        break;
    }
  }
}

void clockDate_to_clockTransition() {
  int speed = 2;
  int pageTransition = 128;

  int cursor = 22 + pageTransition;

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
    u8g2.setCursor(cursor, 61);
    u8g2.print(twoDigit(temporaryHour));
    u8g2.print(':');
    u8g2.print(twoDigit(temporaryMinute));
    u8g2.print(':');
    u8g2.print(twoDigit(temporarySecond));
    u8g2.sendBuffer();

    pageTransition = pageTransition - speed;
    cursor = cursor - speed;
    menus = menus - speed;
    box = box - speed;
    arrowMenus = arrowMenus - speed;
    delayMicroseconds(100);
  } while (pageTransition > 0);
}

void clock_to_clockDateTransition() {
  int speed = 2;
  int pageTransition = -128;

  int cursor = 22;

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
    u8g2.setCursor(cursor, 61);
    u8g2.print(twoDigit(temporaryHour));
    u8g2.print(':');
    u8g2.print(twoDigit(temporaryMinute));
    u8g2.print(':');
    u8g2.print(twoDigit(temporarySecond));
    u8g2.sendBuffer();

    pageTransition = pageTransition + speed;
    cursor = cursor + speed;
    menus = menus + speed;
    box = box + speed;
    arrowMenus = arrowMenus + speed;
    delayMicroseconds(100);
  } while (pageTransition < 0);
}

void ClockHour_to_minuteTransition() {
  int speed = 1;
  int transition = 0;
  int arrow = 30;

  do {
    u8g2.clearBuffer();
    drawClockBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_timR18_tr);
    u8g2.setCursor(22, 61);
    u8g2.print(twoDigit(temporaryHour));
    u8g2.print(':');
    u8g2.print(twoDigit(temporaryMinute));
    u8g2.print(':');
    u8g2.print(twoDigit(temporarySecond));
    u8g2.drawXBM(arrow, 36, 8, 4, up_arrow_BM);
    u8g2.drawXBM(arrow, 65, 8, 4, down_arrow_BM);

    u8g2.sendBuffer();

    arrow = arrow + speed;
    transition = transition + speed;
    delay(6);
  } while (transition < 30);
}

void ClockMinute_to_secondTransition() {
  int speed = 1;
  int transition = 0;
  int arrow = 60;

  do {
    u8g2.clearBuffer();
    drawClockBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_timR18_tr);
    u8g2.setCursor(22, 61);
    u8g2.print(twoDigit(temporaryHour));
    u8g2.print(':');
    u8g2.print(twoDigit(temporaryMinute));
    u8g2.print(':');
    u8g2.print(twoDigit(temporarySecond));
    u8g2.drawXBM(arrow, 36, 8, 4, up_arrow_BM);
    u8g2.drawXBM(arrow, 65, 8, 4, down_arrow_BM);
    u8g2.sendBuffer();

    arrow = arrow + speed;
    transition = transition + speed;
    delay(6);
  } while (transition < 30);
}

////////////////////////////////////////////////////////////////////////////



//date
////////////////////////////////////////////////////////////////////////////

void printDateMenu() {
  u8g2.clearBuffer();
  drawDateBar();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  u8g2.setFont(u8g2_font_profont17_tf);
  u8g2.setCursor(10, 56);
  u8g2.print(twoDigit(temporaryDay));

  if (temporaryMonth == 4 || temporaryMonth == 5 || temporaryMonth == 10 || temporaryMonth == 11 || temporaryMonth == 12) {
    u8g2.setCursor(41, 56);
  } else {
    u8g2.setCursor(37, 56);
  }

  u8g2.print(shortMonths[temporaryMonth]);

  u8g2.setCursor(82, 56);
  u8g2.print(temporaryYear);


  switch (dateSetStep) {
    case 1:
      u8g2.drawXBM(15, 37, 7, 4, up_arrow_small_BM);
      u8g2.drawXBM(15, 60, 7, 4, down_arrow_small_BM);
      if (selectHourUP == true) {
        u8g2.drawXBM(13, 35, 11, 8, select_date_BM);
      }
      if (selectHourDOWN == true) {
        u8g2.drawXBM(13, 58, 11, 8, select_date_BM);
      }
      break;

    case 2:
      u8g2.drawXBM(50, 37, 7, 4, up_arrow_small_BM);
      u8g2.drawXBM(50, 60, 7, 4, down_arrow_small_BM);
      if (selectHourUP == true) {
        u8g2.drawXBM(48, 35, 11, 8, select_date_BM);
      }
      if (selectHourDOWN == true) {
        u8g2.drawXBM(48, 58, 12, 8, select_date_BM);
      }
      break;

    case 3:
      u8g2.drawXBM(96, 37, 7, 4, up_arrow_small_BM);
      u8g2.drawXBM(96, 60, 7, 4, down_arrow_small_BM);
      if (selectHourUP == true) {
        u8g2.drawXBM(94, 35, 11, 8, select_date_BM);
      }
      if (selectHourDOWN == true) {
        u8g2.drawXBM(94, 58, 12, 8, select_date_BM);
      }
      break;
  }
  u8g2.sendBuffer();
}

void updateDateMenu() {

  if (rotatePreviousHour > rotateCounter) {
    selectHourDOWN = true;
    selectHourUP = false;
    millisHour = millis();
    rotatePreviousHour = rotateCounter;
  }
  if (rotatePreviousHour < rotateCounter) {
    selectHourUP = true;
    selectHourDOWN = false;
    millisHour = millis();
    rotatePreviousHour = rotateCounter;
  }
  if (millis() - millisHour > 200 && (selectHourUP == true || selectHourDOWN == true)) {
    millisHour = millis();
    selectHourUP = false;
    selectHourDOWN = false;
  }

  switch (dateSetStep) {
    case 1:
      if (rotateCounter > 31) {
        setRotary(1);
      }
      if (rotateCounter < 1) {
        setRotary(31);
      }
      temporaryDay = rotateCounter;
      break;

    case 2:
      if (rotateCounter > 12) {
        setRotary(1);
      }
      if (rotateCounter < 1) {
        setRotary(12);
      }
      temporaryMonth = rotateCounter;
      break;

    case 3:
      if (rotateCounter > 2099) {
        setRotary(2000);
      }
      if (rotateCounter < 2000) {
        setRotary(2099);
      }
      temporaryYear = rotateCounter;
      break;
  }
}

void executeDateMenu() {
  if (mainButton.pressed()) {
    switch (dateSetStep) {
      case 1:
        setRotary(temporaryMonth);
        dateSetStep = 2;
        selectHourDOWN = false;
        selectHourUP = false;
        rotatePreviousHour = temporaryMonth;
        day_to_monthTransition();
        break;

      case 2:
        if ((temporaryMonth == 4 || temporaryMonth == 6 || temporaryMonth == 9 || temporaryMonth == 11) && temporaryDay > 30) {
          temporaryDay = 30;
        } else if (temporaryMonth == 2 && temporaryDay > 29) {
          temporaryDay = 29;
        }

        setRotary(temporaryYear);
        dateSetStep = 3;
        selectHourDOWN = false;
        selectHourUP = false;
        rotatePreviousHour = temporaryYear;
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
        setRotary(2);
        selectHourDOWN = false;
        selectHourUP = false;
        DateTime now = rtc.now();
        rtc.adjust(DateTime(temporaryYear, temporaryMonth, temporaryDay, now.hour(), now.minute(), now.second()));
        //soundSystem.play(6);
        date_to_clockDateTransition();
        break;
    }
  }
}

void clockDate_to_dateTransition() {
  int speed = 2;

  int pageTransition = 128;


  int day = 10 + pageTransition;
  int month1 = 41 + pageTransition;
  int month2 = 37 + pageTransition;
  int year = 82 + pageTransition;

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
    u8g2.drawXBMP(arrowMenus, 36, 4, 7, right_arrow_BM);

    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont17_tf);

    u8g2.setCursor(day, 56);
    u8g2.print(twoDigit(temporaryDay));

    if (temporaryMonth == 4 || temporaryMonth == 5 || temporaryMonth == 10 || temporaryMonth == 11 || temporaryMonth == 12) {
      u8g2.setCursor(month1, 56);
    } else {
      u8g2.setCursor(month2, 56);
    }
    u8g2.print(shortMonths[temporaryMonth]);

    u8g2.setCursor(year, 56);
    u8g2.print(temporaryYear);

    u8g2.sendBuffer();

    pageTransition = pageTransition - speed;
    day = day - speed;
    month1 = month1 - speed;
    month2 = month2 - speed;
    year = year - speed;
    menus = menus - speed;
    box = box - speed;
    arrowMenus = arrowMenus - speed;
    delayMicroseconds(100);
  } while (pageTransition > 0);
}

void date_to_clockDateTransition() {
  int speed = 2;

  int pageTransition = -128;


  int day = 10;
  int month1 = 41;
  int month2 = 37;
  int year = 82;

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
    u8g2.drawXBMP(arrowMenus, 36, 4, 7, right_arrow_BM);

    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont17_tf);

    u8g2.setCursor(day, 56);
    u8g2.print(twoDigit(temporaryDay));

    if (temporaryMonth == 4 || temporaryMonth == 5 || temporaryMonth == 10 || temporaryMonth == 11 || temporaryMonth == 12) {
      u8g2.setCursor(month1, 56);
    } else {
      u8g2.setCursor(month2, 56);
    }
    u8g2.print(shortMonths[temporaryMonth]);

    u8g2.setCursor(year, 56);
    u8g2.print(temporaryYear);

    u8g2.sendBuffer();

    pageTransition = pageTransition + speed;
    day = day + speed;
    month1 = month1 + speed;
    month2 = month2 + speed;
    year = year + speed;
    menus = menus + speed;
    box = box + speed;
    arrowMenus = arrowMenus + speed;
    delayMicroseconds(100);
  } while (pageTransition < 0);
}

void day_to_monthTransition() {
  int speed = 1;
  int transition = 0;
  int arrow = 15;

  do {
    u8g2.clearBuffer();
    drawDateBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont17_tf);
    u8g2.setCursor(10, 56);
    u8g2.print(twoDigit(temporaryDay));

    if (temporaryMonth == 4 || temporaryMonth == 5 || temporaryMonth == 10 || temporaryMonth == 11 || temporaryMonth == 12) {
      u8g2.setCursor(41, 56);
    } else {
      u8g2.setCursor(37, 56);
    }

    u8g2.print(shortMonths[temporaryMonth]);

    u8g2.setCursor(82, 56);
    u8g2.print(temporaryYear);
    u8g2.drawXBM(arrow, 37, 7, 4, up_arrow_small_BM);
    u8g2.drawXBM(arrow, 60, 7, 4, down_arrow_small_BM);
    u8g2.sendBuffer();

    arrow = arrow + speed;
    transition = transition + speed;
    delay(5);
  } while (transition < 35);
}

void month_to_yearTransition() {
  int speed = 1;
  int transition = 0;
  int arrow = 50;

  do {
    u8g2.clearBuffer();
    drawDateBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont17_tf);
    u8g2.setCursor(10, 56);
    u8g2.print(twoDigit(temporaryDay));

    if (temporaryMonth == 4 || temporaryMonth == 5 || temporaryMonth == 10 || temporaryMonth == 11 || temporaryMonth == 12) {
      u8g2.setCursor(41, 56);
    } else {
      u8g2.setCursor(37, 56);
    }

    u8g2.print(shortMonths[temporaryMonth]);

    u8g2.setCursor(82, 56);
    u8g2.print(temporaryYear);
    u8g2.drawXBM(arrow, 37, 7, 4, up_arrow_small_BM);
    u8g2.drawXBM(arrow, 60, 7, 4, down_arrow_small_BM);
    u8g2.sendBuffer();

    arrow = arrow + speed;
    transition = transition + speed;
    delay(4);
  } while (transition < 46);
}

////////////////////////////////////////////////////////////////////////////