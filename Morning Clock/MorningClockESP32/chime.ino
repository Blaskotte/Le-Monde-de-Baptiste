//  ==chime settings menu==
////////////////////////////////////////////////////////////////////////////

void printChimeMenu() {

  u8g2.clearBuffer();
  drawChimeBar();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  u8g2.setFont(u8g2_font_profont11_tf);
  if (chime_is_activated == true) {
    u8g2.drawUTF8(6, 27, chimeSettingsMenuTitles[1]);
  } else {
    u8g2.drawUTF8(6, 27, chimeSettingsMenuTitles[0]);
  }
  u8g2.drawStr(6, 43, chimeSettingsMenuTitles[2]);
  u8g2.drawStr(6, 59, chimeSettingsMenuTitles[3]);
  u8g2.drawStr(6, 75, chimeSettingsMenuTitles[4]);
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

void updateChimeMenu() {
  if (rotateCounter > 4) {
    setRotary(4);
  }
  if (rotateCounter < 1) {
    setRotary(1);
  }
}

void executeChimeMenu() {
  if (mainButton.pressed()) {
    switch (rotateCounter) {
      case 1:
        chimeSet_state();
        if (chime_is_activated == false) {
          chimeTrue_to_false();
        } else {
          chimeFalse_to_true();
        }
        break;

      case 2:
        chimeSetHour_menu = true;
        chimeSetStep = 1;
        //setRotary();
        //chime_to_hourTransition();
        break;

      case 3:
        chimeSetMusic_menu = true;
        setRotary(1);
        //chime_to_musicTransition();
        break;

      case 4:
        chimeSettingsMenu = false;
        setRotary(2);
        menuItemSelect = 2;
        chime_to_settingsTransition();
        break;
    }
  }
}

void chimeTrue_to_false() {
  int transition = 128;
  int oldItem = 6;
  int newItem = 6 + transition;
  int speed = 2;
  do {
    u8g2.clearBuffer();
    drawChimeBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.drawUTF8(oldItem, 27, chimeSettingsMenuTitles[1]);
    u8g2.drawUTF8(newItem, 27, chimeSettingsMenuTitles[0]);
    u8g2.drawStr(6, 43, chimeSettingsMenuTitles[2]);
    u8g2.drawStr(6, 59, chimeSettingsMenuTitles[3]);
    u8g2.drawStr(6, 75, chimeSettingsMenuTitles[4]);
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, 16, 128, 15);
    u8g2.sendBuffer();

    oldItem = oldItem - speed;
    newItem = newItem - speed;
    transition = transition - speed;
    delayMicroseconds(100);
  } while (transition > 0);
}

void chimeFalse_to_true() {
  int transition = 128;
  int oldItem = 6;
  int newItem = 6 + transition;
  int speed = 2;
  do {
    u8g2.clearBuffer();
    drawChimeBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.drawUTF8(oldItem, 27, chimeSettingsMenuTitles[0]);
    u8g2.drawUTF8(newItem, 27, chimeSettingsMenuTitles[1]);
    u8g2.drawStr(6, 43, chimeSettingsMenuTitles[2]);
    u8g2.drawStr(6, 59, chimeSettingsMenuTitles[3]);
    u8g2.drawStr(6, 75, chimeSettingsMenuTitles[4]);
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, 16, 128, 15);
    u8g2.sendBuffer();

    oldItem = oldItem - speed;
    newItem = newItem - speed;
    transition = transition - speed;
    delayMicroseconds(100);
  } while (transition > 0);
}

void settings_to_chimeTransition() {
  int speed = 2;

  int pageTransition = 128;
  int settingsItems = 6;
  int settingsBox = 0;
  int settingsArrow = 112;

  int chimeItems = 6 + pageTransition;
  int chimeBox = 0 + pageTransition;

  do {
    u8g2.clearBuffer();
    drawChimeBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont11_tf);
    switch (frameSettingsMenu) {
      case 1:
        u8g2.drawUTF8(settingsItems, 27, mainSettingsMenuTitles[1]);
        u8g2.drawStr(settingsItems, 43, mainSettingsMenuTitles[2]);
        u8g2.drawStr(settingsItems, 59, mainSettingsMenuTitles[3]);
        u8g2.drawStr(settingsItems, 75, mainSettingsMenuTitles[4]);
        u8g2.drawUTF8(settingsItems, 91, mainSettingsMenuTitles[5]);
        u8g2.drawXBMP(settingsArrow, 36, 4, 7, right_arrow_BM);
        u8g2.drawBox(settingsBox, 32, 122, 15);
        break;

      case 2:
        u8g2.drawUTF8(settingsItems, 27, mainSettingsMenuTitles[2]);
        u8g2.drawStr(settingsItems, 43, mainSettingsMenuTitles[3]);
        u8g2.drawStr(settingsItems, 59, mainSettingsMenuTitles[4]);
        u8g2.drawStr(settingsItems, 75, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingsItems, 91, mainSettingsMenuTitles[6]);
        u8g2.drawXBMP(settingsArrow, 20, 4, 7, right_arrow_BM);
        u8g2.drawBox(settingsBox, 16, 122, 15);
        break;
    }
    if (alarm_is_activated == true) {
      u8g2.drawUTF8(chimeItems, 27, chimeSettingsMenuTitles[1]);
    } else {
      u8g2.drawStr(chimeItems, 27, chimeSettingsMenuTitles[0]);
    }
    u8g2.drawStr(chimeItems, 43, chimeSettingsMenuTitles[2]);
    u8g2.drawStr(chimeItems, 59, chimeSettingsMenuTitles[3]);
    u8g2.drawStr(chimeItems, 75, chimeSettingsMenuTitles[4]);
    u8g2.drawStr(chimeItems, 91, chimeSettingsMenuTitles[5]);


    u8g2.setDrawColor(2);
    u8g2.drawBox(chimeBox, 16, 128, 15);
    u8g2.sendBuffer();

    pageTransition = pageTransition - speed;
    settingsItems = settingsItems - speed;
    settingsBox = settingsBox - speed;
    settingsArrow = settingsArrow - speed;

    chimeItems = chimeItems - speed;
    chimeBox = chimeBox - speed;
    delayMicroseconds(100);
  } while (pageTransition > 0);
}

void chime_to_settingsTransition() {
  int speed = 2;

  int pageTransition = -128;
  int settingsItems = 6 + pageTransition;
  int settingsBox = 0 + pageTransition;
  int settingsArrow = 112 + pageTransition;

  int chimeItems = 6;
  int chimeBox = 0;
  int chimeArrow = 118;

  do {
    u8g2.clearBuffer();
    drawSettingsBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont11_tf);
    switch (frameSettingsMenu) {
      case 1:
        u8g2.drawUTF8(settingsItems, 27, mainSettingsMenuTitles[1]);
        u8g2.drawStr(settingsItems, 43, mainSettingsMenuTitles[2]);
        u8g2.drawStr(settingsItems, 59, mainSettingsMenuTitles[3]);
        u8g2.drawStr(settingsItems, 75, mainSettingsMenuTitles[4]);
        u8g2.drawUTF8(settingsItems, 91, mainSettingsMenuTitles[5]);
        u8g2.drawXBMP(settingsArrow, 36, 4, 7, right_arrow_BM);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 32, 122, 15);
        break;

      case 2:
        u8g2.drawUTF8(settingsItems, 27, mainSettingsMenuTitles[2]);
        u8g2.drawStr(settingsItems, 43, mainSettingsMenuTitles[3]);
        u8g2.drawStr(settingsItems, 59, mainSettingsMenuTitles[4]);
        u8g2.drawStr(settingsItems, 75, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingsItems, 91, mainSettingsMenuTitles[6]);
        u8g2.drawXBMP(settingsArrow, 20, 4, 7, right_arrow_BM);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 16, 122, 15);
        break;
    }
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    if (alarm_is_activated == true) {
      u8g2.drawUTF8(chimeItems, 27, chimeSettingsMenuTitles[1]);
    } else {
      u8g2.drawStr(chimeItems, 27, chimeSettingsMenuTitles[0]);
    }
    u8g2.drawStr(chimeItems, 43, chimeSettingsMenuTitles[2]);
    u8g2.drawStr(chimeItems, 59, chimeSettingsMenuTitles[3]);
    u8g2.drawStr(chimeItems, 75, chimeSettingsMenuTitles[4]);
    u8g2.drawStr(chimeItems, 91, chimeSettingsMenuTitles[5]);

    u8g2.drawXBMP(chimeArrow, 68, 4, 7, left_arrow_BM);

    u8g2.setDrawColor(2);
    u8g2.drawBox(chimeBox, 64, 128, 15);
    drawScrollbar();
    drawSettingsScrollbar();
    u8g2.sendBuffer();

    pageTransition = pageTransition + speed;
    settingsItems = settingsItems + speed;
    settingsBox = settingsBox + speed;
    settingsArrow = settingsArrow + speed;

    chimeItems = chimeItems + speed;
    chimeBox = chimeBox + speed;
    chimeArrow = chimeArrow + speed;
    delayMicroseconds(100);
  } while (pageTransition < 0);
}
////////////////////////////////////////////////////////////////////////////



//  ==chime settings submenus
////////////////////////////////////////////////////////////////////////////

//chime / state
void chimeSet_state() {
  if (chime_is_activated == true) {
    chime_is_activated = false;
    eprom.write(13, false);
    soundSystem.play(5);
  } else {
    chime_is_activated = true;
    eprom.write(13, true);
    soundSystem.play(4);
  }
}

////////////////////////////////////////////////////////////////////////////
