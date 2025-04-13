//   ==alarm settings menu==
////////////////////////////////////////////////////////////////////////////

void printAlarmMenu() {

  u8g2.clearBuffer();
  drawAlarmBar();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  u8g2.setFont(u8g2_font_profont11_tf);
  if (alarm_is_activated == true) {
    u8g2.drawUTF8(6, 27, alarmSettingsMenuTitles[1]);
  } else {
    u8g2.drawUTF8(6, 27, alarmSettingsMenuTitles[0]);
  }
  u8g2.drawStr(6, 43, alarmSettingsMenuTitles[2]);
  u8g2.drawStr(6, 59, alarmSettingsMenuTitles[3]);
  u8g2.drawStr(6, 75, alarmSettingsMenuTitles[4]);
  u8g2.setDrawColor(2);
  switch (rotateCounter) {
    case 1:
      u8g2.drawBox(0, 16, 128, 15);
      break;

    case 2:
      u8g2.drawBox(0, 32, 128, 15);
      u8g2.drawXBMP(118, 36, 4, 7, right_arrow_BM);
      break;

    case 3:
      u8g2.drawBox(0, 48, 128, 15);
      u8g2.drawXBMP(118, 52, 4, 7, right_arrow_BM);
      break;
    case 4:
      u8g2.drawBox(0, 64, 128, 15);
      u8g2.drawXBMP(118, 68, 4, 7, left_arrow_BM);
      break;
  }
  u8g2.sendBuffer();
}

void updateAlarmMenu() {
  if (rotateCounter > 4) {
    setRotary(4);
  }
  if (rotateCounter < 1) {
    setRotary(1);
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
        alarmSetMusic_menu = true;
        setRotary(1);
        alarm_to_musicTransition();
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
    u8g2.drawStr(6, 75, alarmSettingsMenuTitles[4]);
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, 16, 128, 15);
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
    u8g2.drawStr(6, 75, alarmSettingsMenuTitles[4]);
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, 16, 128, 15);
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
  int settingsItems = 6;
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
    u8g2.drawUTF8(settingsItems, 27, mainSettingsMenuTitles[1]);
    u8g2.drawStr(settingsItems, 43, mainSettingsMenuTitles[2]);
    u8g2.drawStr(settingsItems, 59, mainSettingsMenuTitles[3]);
    u8g2.drawStr(settingsItems, 75, mainSettingsMenuTitles[4]);
    u8g2.drawUTF8(settingsItems, 91, mainSettingsMenuTitles[5]);
    u8g2.drawXBMP(settingsArrow, 20, 4, 7, right_arrow_BM);

    if (alarm_is_activated == true) {
      u8g2.drawUTF8(alarmItems, 27, alarmSettingsMenuTitles[1]);
    } else {
      u8g2.drawStr(alarmItems, 27, alarmSettingsMenuTitles[0]);
    }
    u8g2.drawStr(alarmItems, 43, alarmSettingsMenuTitles[2]);
    u8g2.drawStr(alarmItems, 59, alarmSettingsMenuTitles[3]);
    u8g2.drawStr(alarmItems, 75, alarmSettingsMenuTitles[4]);


    u8g2.setDrawColor(2);
    u8g2.drawBox(settingsBox, 16, 122, 15);
    u8g2.drawBox(alarmBox, 16, 128, 15);
    u8g2.sendBuffer();

    pageTransition = pageTransition - speed;
    settingsItems = settingsItems - speed;
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
  int settingsItems = 6 + pageTransition;
  int settingsBox = 0 + pageTransition;
  int settingsArrow = 112 + pageTransition;

  int alarmItems = 6;
  int alarmBox = 0;
  int alarmArrow = 118;

  do {
    u8g2.clearBuffer();
    drawSettingsBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.drawStr(settingsItems, 27, mainSettingsMenuTitles[1]);
    u8g2.drawStr(settingsItems, 43, mainSettingsMenuTitles[2]);
    u8g2.drawStr(settingsItems, 59, mainSettingsMenuTitles[3]);
    u8g2.drawStr(settingsItems, 75, mainSettingsMenuTitles[4]);
    u8g2.drawUTF8(settingsItems, 91, mainSettingsMenuTitles[5]);
    u8g2.drawXBMP(settingsArrow, 20, 4, 7, right_arrow_BM);

    if (alarm_is_activated == true) {
      u8g2.drawUTF8(alarmItems, 27, alarmSettingsMenuTitles[1]);
    } else {
      u8g2.drawStr(alarmItems, 27, alarmSettingsMenuTitles[0]);
    }
    u8g2.drawStr(alarmItems, 43, alarmSettingsMenuTitles[2]);
    u8g2.drawStr(alarmItems, 59, alarmSettingsMenuTitles[3]);
    u8g2.drawStr(alarmItems, 75, alarmSettingsMenuTitles[4]);
    u8g2.drawXBMP(alarmArrow, 68, 4, 7, left_arrow_BM);

    u8g2.setDrawColor(2);
    u8g2.drawBox(settingsBox, 16, 122, 15);
    u8g2.drawBox(alarmBox, 64, 128, 15);
    drawScrollbar();
    drawSettingsScrollbar();
    u8g2.sendBuffer();

    pageTransition = pageTransition + speed;
    settingsItems = settingsItems + speed;
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
    eprom.write(12, false);
    soundSystem.play(5);
  } else {
    alarm_is_activated = true;
    eprom.write(12, true);
    soundSystem.play(4);
  }
}


//alarm / hour
void printAlarmSet_hour() {

  u8g2.clearBuffer();
  drawAlarmBar();
  u8g2.setFont(u8g2_font_timR18_tr);
  u8g2.setCursor(37, 61);
  u8g2.print(twoDigit(alarmHour));
  u8g2.print(':');
  u8g2.print(twoDigit(alarmMinute));
  switch (alarmSetStep) {
    case 1:
      u8g2.drawXBMP(45, 36, 8, 4, up_arrow_BM);
      u8g2.drawXBMP(45, 65, 8, 4, down_arrow_BM);
      break;

    case 2:
      u8g2.drawXBM(75, 36, 8, 4, up_arrow_BM);
      u8g2.drawXBM(75, 66, 8, 4, down_arrow_BM);
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
    if (alarm_is_activated == true) {
      u8g2.drawUTF8(alarmItems, 27, alarmSettingsMenuTitles[1]);
    } else {
      u8g2.drawStr(alarmItems, 27, alarmSettingsMenuTitles[0]);
    }
    u8g2.drawStr(alarmItems, 43, alarmSettingsMenuTitles[2]);
    u8g2.drawStr(alarmItems, 59, alarmSettingsMenuTitles[3]);
    u8g2.drawStr(alarmItems, 75, alarmSettingsMenuTitles[4]);
    u8g2.drawBox(alarmBox, 32, 128, 15);
    u8g2.drawXBMP(alarmArrow, 36, 4, 7, right_arrow_BM);
    u8g2.setFont(u8g2_font_timR18_tr);
    u8g2.setCursor(clock, 61);
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
    u8g2.setCursor(clock, 61);
    u8g2.print(twoDigit(alarmHour));
    u8g2.print(':');
    u8g2.print(twoDigit(alarmMinute));

    u8g2.setFont(u8g2_font_profont11_tf);
    if (alarm_is_activated == true) {
      u8g2.drawUTF8(alarmItems, 27, alarmSettingsMenuTitles[1]);
    } else {
      u8g2.drawStr(alarmItems, 27, alarmSettingsMenuTitles[0]);
    }
    u8g2.drawStr(alarmItems, 43, alarmSettingsMenuTitles[2]);
    u8g2.drawStr(alarmItems, 59, alarmSettingsMenuTitles[3]);
    u8g2.drawStr(alarmItems, 75, alarmSettingsMenuTitles[4]);
    u8g2.drawBox(alarmBox, 32, 128, 15);
    u8g2.drawXBMP(alarmArrow, 36, 4, 7, right_arrow_BM);
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
    u8g2.setCursor(37, 61);
    u8g2.print(twoDigit(alarmHour));
    u8g2.print(':');
    u8g2.print(twoDigit(alarmMinute));
    u8g2.drawXBMP(arrow, 36, 8, 4, up_arrow_BM);
    u8g2.drawXBMP(arrow, 65, 8, 4, down_arrow_BM);
    u8g2.sendBuffer();

    transition = transition + speed;
    arrow = arrow + speed;
    delay(6);
  } while (transition < 30);
}

////////////////////////////////////////////////////////////////////////////

//alarm / music
void printAlarmSet_music() {
  u8g2.clearBuffer();
  drawAlarmBar();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawUTF8(6, 27, alarmSoundTitles[0]);
  u8g2.drawStr(6, 43, alarmSoundTitles[1]);
  u8g2.drawStr(6, 59, alarmSoundTitles[2]);
  u8g2.drawStr(6, 75, alarmSoundTitles[3]);
  u8g2.drawUTF8(6, 91, alarmSoundTitles[4]);
  switch (rotateCounter) {
    case 1:
      u8g2.drawBox(0, 16, 128, 15);
      break;
    case 2:
      u8g2.drawBox(0, 32, 128, 15);
      break;
    case 3:
      u8g2.drawBox(0, 48, 128, 15);
      break;
    case 4:
      u8g2.drawBox(0, 64, 128, 15);
      break;
    case 5:
      u8g2.drawBox(0, 80, 128, 15);
      u8g2.drawXBMP(118, 84, 4, 7, left_arrow_BM);
      break;
  }
  u8g2.setBitmapMode(2);
  switch (alarmMusic) {
    case 1:
      u8g2.drawXBM(117, 19, 7, 9, validate_BM);
      break;

    case 2:
      u8g2.drawXBM(117, 35, 7, 9, validate_BM);
      break;

    case 3:
      u8g2.drawXBM(117, 51, 7, 9, validate_BM);
      break;

    case 4:
      u8g2.drawXBM(117, 67, 7, 9, validate_BM);
      break;
  }
  u8g2.sendBuffer();
}

void updateAlarmSet_music() {
  switch (rotateCounter) {

    default:
      if (rotateCounter > 5) {
        setRotary(5);
      }
      if (rotateCounter < 1) {
        setRotary(1);
      }
  }
}

void executeAlarmSet_music() {
  if (mainButton.pressed()) {
    switch (rotateCounter) {
      case 1:
        alarmMusic = 1;
        eprom.update(1, alarmMusic);
        break;

      case 2:
        alarmMusic = 2;
        eprom.update(1, alarmMusic);
        break;

      case 3:
        alarmMusic = 3;
        eprom.update(1, alarmMusic);
        break;

      case 4:
        alarmMusic = 4;
        eprom.update(1, alarmMusic);
        break;

      case 5:
        alarmSetMusic_menu = false;
        setRotary(3);
        music_to_alarmTransition();
        break;
    }
  }
}

void alarm_to_musicTransition() {
  int speed = 2;

  int pageTransition = 128;
  int alarmItems = 6;
  int alarmBox = 0;
  int alarmArrow = 118;

  int alarmMusicItems = 6 + pageTransition;
  int alarmMusicBox = 0 + pageTransition;
  int alarmMusicValidate = 117 + pageTransition;

  do {
    u8g2.clearBuffer();
    drawAlarmBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont11_tf);

    if (alarm_is_activated == true) {
      u8g2.drawUTF8(alarmItems, 27, alarmSettingsMenuTitles[1]);
    } else {
      u8g2.drawStr(alarmItems, 27, alarmSettingsMenuTitles[0]);
    }
    u8g2.drawUTF8(alarmItems, 43, alarmSettingsMenuTitles[2]);
    u8g2.drawUTF8(alarmItems, 59, alarmSettingsMenuTitles[3]);
    u8g2.drawUTF8(alarmItems, 75, alarmSettingsMenuTitles[4]);
    u8g2.drawXBMP(alarmArrow, 52, 4, 7, right_arrow_BM);

    u8g2.drawUTF8(alarmMusicItems, 27, alarmSoundTitles[0]);
    u8g2.drawUTF8(alarmMusicItems, 43, alarmSoundTitles[1]);
    u8g2.drawUTF8(alarmMusicItems, 59, alarmSoundTitles[2]);
    u8g2.drawUTF8(alarmMusicItems, 75, alarmSoundTitles[3]);
    u8g2.drawUTF8(alarmMusicItems, 91, alarmSoundTitles[4]);


    u8g2.setDrawColor(2);
    u8g2.drawBox(alarmBox, 48, 128, 15);

    u8g2.drawBox(alarmMusicBox, 16, 128, 15);

    switch (alarmMusic) {
      case 1:
        u8g2.drawXBM(alarmMusicValidate, 19, 7, 9, validate_BM);
        break;

      case 2:
        u8g2.drawXBM(alarmMusicValidate, 35, 7, 9, validate_BM);
        break;

      case 3:
        u8g2.drawXBM(alarmMusicValidate, 51, 7, 9, validate_BM);
        break;

      case 4:
        u8g2.drawXBM(alarmMusicValidate, 67, 7, 9, validate_BM);
        break;
    }

    u8g2.sendBuffer();

    pageTransition = pageTransition - speed;
    alarmItems = alarmItems - speed;
    alarmBox = alarmBox - speed;
    alarmArrow = alarmArrow - speed;

    alarmMusicItems = alarmMusicItems - speed;
    alarmMusicBox = alarmMusicBox - speed;
    alarmMusicValidate = alarmMusicValidate - speed;
    delayMicroseconds(100);
  } while (pageTransition > 0);
}

void music_to_alarmTransition() {
  int speed = 2;

  int pageTransition = -128;
  int alarmItems = 6 + pageTransition;
  int alarmBox = 0 + pageTransition;
  int alarmArrow = 118 + pageTransition;

  int alarmMusicItems = 6;
  int alarmMusicBox = 0;
  int alarmMusicValidate = 117;
  int alarmMusicArrow = 118;


  do {
    u8g2.clearBuffer();
    drawAlarmBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont11_tf);

    if (alarm_is_activated == true) {
      u8g2.drawUTF8(alarmItems, 27, alarmSettingsMenuTitles[1]);
    } else {
      u8g2.drawStr(alarmItems, 27, alarmSettingsMenuTitles[0]);
    }
    u8g2.drawUTF8(alarmItems, 43, alarmSettingsMenuTitles[2]);
    u8g2.drawUTF8(alarmItems, 59, alarmSettingsMenuTitles[3]);
    u8g2.drawUTF8(alarmItems, 75, alarmSettingsMenuTitles[4]);
    u8g2.drawXBMP(alarmArrow, 52, 4, 7, right_arrow_BM);

    u8g2.drawUTF8(alarmMusicItems, 27, alarmSoundTitles[0]);
    u8g2.drawUTF8(alarmMusicItems, 43, alarmSoundTitles[1]);
    u8g2.drawUTF8(alarmMusicItems, 59, alarmSoundTitles[2]);
    u8g2.drawUTF8(alarmMusicItems, 75, alarmSoundTitles[3]);
    u8g2.drawUTF8(alarmMusicItems, 91, alarmSoundTitles[4]);
    u8g2.drawXBMP(alarmMusicArrow, 84, 4, 7, left_arrow_BM);


    u8g2.setDrawColor(2);
    u8g2.drawBox(alarmBox, 48, 128, 15);

    u8g2.drawBox(alarmMusicBox, 80, 128, 15);

    switch (alarmMusic) {
      case 1:
        u8g2.drawXBM(alarmMusicValidate, 19, 7, 9, validate_BM);
        break;

      case 2:
        u8g2.drawXBM(alarmMusicValidate, 35, 7, 9, validate_BM);
        break;

      case 3:
        u8g2.drawXBM(alarmMusicValidate, 51, 7, 9, validate_BM);
        break;

      case 4:
        u8g2.drawXBM(alarmMusicValidate, 67, 7, 9, validate_BM);
        break;
    }

    u8g2.sendBuffer();

    pageTransition = pageTransition + speed;
    alarmItems = alarmItems + speed;
    alarmBox = alarmBox + speed;
    alarmArrow = alarmArrow + speed;

    alarmMusicItems = alarmMusicItems + speed;
    alarmMusicBox = alarmMusicBox + speed;
    alarmMusicValidate = alarmMusicValidate + speed;
    alarmMusicArrow = alarmMusicArrow + speed;
    delayMicroseconds(100);
  } while (pageTransition < 0);
}


//AlarmNotification message set
////////////////////////////////////////////////////////////////////////////

void updateAlarmNotification() {
  if (millis() - previousMillis >= 500) {
    previousMillis = millis();
    if (clockBig == true) {
      clockBig = false;
    } else {
      clockBig = true;
    }
  }

  if (rotateCounter > 2) {
    setRotary(2);
  }
  if (rotateCounter < 1) {
    setRotary(1);
  }
}

void printAlarmNotification() {
  DateTime now = rtc.now();
  u8g2.setDrawColor(0);
  u8g2.drawRBox(21, 19, 86, 58, 9);
  u8g2.setDrawColor(1);
  u8g2.drawRFrame(21, 19, 86, 58, 9);
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  u8g2.drawXBMP(53, 30, 22, 25, big_alarm_BM);

  if (clockBig == true) {
    u8g2.drawXBM(51, 28, 8, 8, big_alarm_left_BM);
    u8g2.drawXBM(69, 28, 8, 8, big_alarm_right_BM);
  }
  if (clockBig == false) {
    u8g2.drawXBM(50, 27, 8, 8, big_alarm_left_BM);
    u8g2.drawXBM(70, 27, 8, 8, big_alarm_right_BM);
  }
  u8g2.setFont(u8g2_font_profont10_tf);
  u8g2.drawStr(31, 69, "Snooze");
  u8g2.drawUTF8(73, 69, "Arrêt");
  u8g2.setDrawColor(2);
  if (rotateCounter == 1) {
    u8g2.drawRBox(28, 61, 35, 10, 0);
  }
  if (rotateCounter == 2) {
    u8g2.drawRBox(70, 61, 30, 10, 0);
  }
  u8g2.setFont(u8g2_font_profont11_tf);

  u8g2.setCursor(49, 10);
  u8g2.print(twoDigit(now.hour()));

  if (blink == true) {
    u8g2.print(":");
  } else {
    u8g2.print(" ");
  }

  u8g2.setCursor(67, 10);
  u8g2.print(twoDigit(now.minute()));
}

void executeAlarmNotification() {
  if (rotateCounter == 2 && mainButton.pressed()) {
    alarmNotification = false;
    setRotary(0);
  }
  if (rotateCounter == 1 && mainButton.pressed()) {
  }
}

////////////////////////////////////////////////////////////////////////////