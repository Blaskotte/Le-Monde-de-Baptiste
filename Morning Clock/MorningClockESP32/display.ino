//displayMenu
////////////////////////////////////////////////////////////////////////////

void printDisplayMenu() {

  u8g2.clearBuffer();
  drawDisplayBar();
  u8g2.setFontMode(2);
  u8g2.setDrawColor(1);
  u8g2.setBitmapMode(1);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawUTF8(6, 27, displayMenuTitles[1]);
  u8g2.drawUTF8(6, 43, displayMenuTitles[2]);
  u8g2.drawUTF8(6, 59, displayMenuTitles[3]);
  u8g2.drawUTF8(6, 75, displayMenuTitles[4]);
  u8g2.setDrawColor(2);
  switch (rotateCounter) {
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
      u8g2.drawXBMP(118, 52, 4, 7, right_arrow_BM);
      break;

    case 4:
      u8g2.drawBox(0, 64, 128, 15);
      u8g2.drawXBMP(118, 68, 4, 7, left_arrow_BM);
      break;
  }
  u8g2.sendBuffer();
}

void updateDisplayMenu() {

  if (rotateCounter > 4) {
    setRotary(4);
  }
  if (rotateCounter < 1) {
    setRotary(1);
  }
}

void executeDisplayMenu() {
  if (mainButton.pressed()) {
    switch (rotateCounter) {
      case 1:
        brightness_menu = true;
        setRotary(map(brightness, 10, 0, 0, 10));
        display_to_brightnessTransition();
        break;

      case 2:
        contrast_menu = true;
        setRotary(map(contrast, 80, 180, 0, 50));
        display_to_contrastTransition();
        break;

      case 3:
        screensaver_menu = true;
        setRotary(1);
        //setRotary(map(contrast, 80, 180, 0, 50));
        //display_to_screensaverTransition();
        break;

      case 4:
        displaySettingsMenu = false;
        setRotary(4);
        menuItemSelect = 4;
        display_to_settingsTransition();
        break;
    }
  }
}

void settings_to_displayTransition() {
  int speed = 2;

  int pageTransition = 128;
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
      case 1:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[1]);
        u8g2.drawStr(settingItems, 43, mainSettingsMenuTitles[2]);
        u8g2.drawStr(settingItems, 59, mainSettingsMenuTitles[3]);
        u8g2.drawUTF8(settingItems, 75, mainSettingsMenuTitles[4]);
        u8g2.drawUTF8(settingItems, 91, mainSettingsMenuTitles[5]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 64, 122, 15);
        u8g2.drawXBMP(settingsArrow, 68, 4, 7, right_arrow_BM);
        break;

      case 2:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[2]);
        u8g2.drawStr(settingItems, 43, mainSettingsMenuTitles[3]);
        u8g2.drawStr(settingItems, 59, mainSettingsMenuTitles[4]);
        u8g2.drawUTF8(settingItems, 75, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingItems, 91, mainSettingsMenuTitles[6]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 48, 122, 15);
        u8g2.drawXBMP(settingsArrow, 52, 4, 7, right_arrow_BM);
        break;

      case 3:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[3]);
        u8g2.drawStr(settingItems, 43, mainSettingsMenuTitles[4]);
        u8g2.drawStr(settingItems, 59, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingItems, 75, mainSettingsMenuTitles[6]);
        u8g2.drawUTF8(settingItems, 91, mainSettingsMenuTitles[7]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 32, 122, 15);
        u8g2.drawXBMP(settingsArrow, 36, 4, 7, right_arrow_BM);
        break;
    }
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.drawUTF8(displayItems, 27, displayMenuTitles[1]);
    u8g2.drawUTF8(displayItems, 43, displayMenuTitles[2]);
    u8g2.drawStr(displayItems, 59, displayMenuTitles[3]);
    u8g2.drawStr(displayItems, 75, displayMenuTitles[4]);
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
    delayMicroseconds(100);
  } while (pageTransition > 0);
}

void display_to_settingsTransition() {
  int speed = 2;

  int pageTransition = -128;
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
    u8g2.drawUTF8(displayItems, 59, clockDateMenuTitles[3]);
    u8g2.drawUTF8(displayItems, 59, clockDateMenuTitles[4]);
    u8g2.setDrawColor(2);
    u8g2.drawBox(displayBox, 64, 128, 15);
    u8g2.drawXBMP(displayArrow, 68, 4, 7, left_arrow_BM);

    switch (frameSettingsMenu) {
      case 1:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[1]);
        u8g2.drawStr(settingItems, 43, mainSettingsMenuTitles[2]);
        u8g2.drawStr(settingItems, 59, mainSettingsMenuTitles[3]);
        u8g2.drawUTF8(settingItems, 75, mainSettingsMenuTitles[4]);
        u8g2.drawUTF8(settingItems, 91, mainSettingsMenuTitles[5]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 64, 122, 15);
        u8g2.drawXBMP(settingsArrow, 68, 4, 7, right_arrow_BM);
        break;

      case 2:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[2]);
        u8g2.drawStr(settingItems, 43, mainSettingsMenuTitles[3]);
        u8g2.drawStr(settingItems, 59, mainSettingsMenuTitles[4]);
        u8g2.drawUTF8(settingItems, 75, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingItems, 91, mainSettingsMenuTitles[6]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 48, 122, 15);
        u8g2.drawXBMP(settingsArrow, 52, 4, 7, right_arrow_BM);
        break;

      case 3:
        u8g2.drawStr(settingItems, 27, mainSettingsMenuTitles[3]);
        u8g2.drawStr(settingItems, 43, mainSettingsMenuTitles[4]);
        u8g2.drawStr(settingItems, 59, mainSettingsMenuTitles[5]);
        u8g2.drawUTF8(settingItems, 75, mainSettingsMenuTitles[6]);
        u8g2.drawUTF8(settingItems, 91, mainSettingsMenuTitles[7]);
        u8g2.setDrawColor(2);
        u8g2.drawBox(settingsBox, 32, 122, 15);
        u8g2.drawXBMP(settingsArrow, 36, 4, 7, right_arrow_BM);
        break;
    }
    drawScrollbar();
    drawSettingsScrollbar();
    u8g2.sendBuffer();

    pageTransition = pageTransition + speed;
    settingItems = settingItems + speed;
    settingsBox = settingsBox + speed;
    settingsArrow = settingsArrow + speed;

    displayItems = displayItems + speed;
    displayBox = displayBox + speed;
    displayArrow = displayArrow + speed;
    delayMicroseconds(100);
  } while (pageTransition < 0);
}

////////////////////////////////////////////////////////////////////////////



//brightness
////////////////////////////////////////////////////////////////////////////

void printBrightnessMenu() {
  u8g2.clearBuffer();
  drawDisplayBar();
  u8g2.drawXBMP(11, 48, 8, 12, brightness_low_BM);
  u8g2.drawXBMP(106, 47, 16, 13, brightness_high_BM);
  u8g2.drawBox(29, 52, brightnessBarSize, 4);
  u8g2.drawFrame(27, 50, 75, 8);
  u8g2.sendBuffer();
}

void updateBrightnessMenu() {

  if (rotateCounter > 10) {
    setRotary(10);
  }
  if (rotateCounter < 0) {
    setRotary(0);
  }

  brightnessBarSize = map(rotateCounter, 0, 10, 0, 71);
  brightness = map(rotateCounter, 0, 10, 10, 0);
  Potentiometer.writeWiper(brightness);
}

void executeBrightnessMenu() {
  if (mainButton.pressed()) {
    brightness_menu = false;
    setRotary(1);
    menuItemSelect = 1;
    eprom.update(10, brightness);
    //soundSystem.play(6);
    brightness_to_displayTransition();
  }
}

void display_to_brightnessTransition() {
  int speed = 2;

  int pageTransition = 128;
  int displayItems = 6;
  int displayBox = 0;
  int displayArrow = 118;

  int brightnessIcon1 = 11 + pageTransition;
  int brightnessIcon2 = 106 + pageTransition;
  int brightnessFrame = 27 + pageTransition;
  int brightnessBox = 29 + pageTransition;

  do {
    u8g2.clearBuffer();
    drawDisplayBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.drawUTF8(displayItems, 27, displayMenuTitles[1]);
    u8g2.drawUTF8(displayItems, 43, displayMenuTitles[2]);
    u8g2.drawStr(displayItems, 59, displayMenuTitles[3]);

    u8g2.drawXBMP(brightnessIcon1, 48, 8, 12, brightness_low_BM);
    u8g2.drawXBMP(brightnessIcon2, 47, 16, 13, brightness_high_BM);
    u8g2.drawBox(brightnessBox, 52, brightnessBarSize, 4);
    u8g2.drawFrame(brightnessFrame, 50, 75, 8);

    u8g2.setDrawColor(2);
    u8g2.drawBox(displayBox, 16, 128, 15);
    u8g2.drawXBMP(displayArrow, 20, 4, 7, right_arrow_BM);

    u8g2.sendBuffer();

    pageTransition = pageTransition - speed;
    displayItems = displayItems - speed;
    displayBox = displayBox - speed;
    displayArrow = displayArrow - speed;

    brightnessIcon1 = brightnessIcon1 - speed;
    brightnessIcon2 = brightnessIcon2 - speed;
    brightnessFrame = brightnessFrame - speed;
    brightnessBox = brightnessBox - speed;
    delayMicroseconds(100);
  } while (pageTransition > 0);
}

void brightness_to_displayTransition() {
  int speed = 2;

  int pageTransition = -128;
  int brightnessIcon1 = 11;
  int brightnessIcon2 = 110;
  int brightnessFrame = 27;
  int brightnessBox = 29;

  int displayItems = 6 + pageTransition;
  int displayBox = 0 + pageTransition;
  int displayArrow = 118 + pageTransition;

  do {
    u8g2.clearBuffer();
    drawDisplayBar();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);

    u8g2.drawXBMP(brightnessIcon1, 48, 8, 12, brightness_low_BM);
    u8g2.drawXBMP(brightnessIcon2, 47, 16, 13, brightness_high_BM);
    u8g2.drawBox(brightnessBox, 52, brightnessBarSize, 4);
    u8g2.drawFrame(brightnessFrame, 50, 75, 8);

    u8g2.setFont(u8g2_font_profont11_tf);
    u8g2.drawUTF8(displayItems, 27, displayMenuTitles[1]);
    u8g2.drawUTF8(displayItems, 43, displayMenuTitles[2]);
    u8g2.drawStr(displayItems, 59, displayMenuTitles[3]);

    u8g2.setDrawColor(2);
    u8g2.drawBox(displayBox, 16, 128, 15);
    u8g2.drawXBMP(displayArrow, 20, 4, 7, right_arrow_BM);
    u8g2.sendBuffer();

    pageTransition = pageTransition + speed;
    displayItems = displayItems + speed;
    displayBox = displayBox + speed;
    displayArrow = displayArrow + speed;

    brightnessIcon1 = brightnessIcon1 + speed;
    brightnessIcon2 = brightnessIcon2 + speed;
    brightnessFrame = brightnessFrame + speed;
    brightnessBox = brightnessBox + speed;
    delayMicroseconds(100);
  } while (pageTransition < 0);
}

////////////////////////////////////////////////////////////////////////////



//contrast
////////////////////////////////////////////////////////////////////////////

void printContrastMenu() {
  u8g2.clearBuffer();
  drawDisplayBar();
  u8g2.drawXBM(110, 50, 8, 8, contrast_high_BM);
  u8g2.drawXBM(11, 50, 8, 8, contrast_low_BM);
  u8g2.drawFrame(27, 50, 75, 8);
  u8g2.drawBox(29, 52, contrastBarSize, 4);
  u8g2.sendBuffer();
}

void updateContrastMenu() {

  if (rotateCounter > 50) {
    setRotary(50);
  }
  if (rotateCounter < 0) {
    setRotary(0);
  }

  contrastBarSize = map(rotateCounter, 0, 50, 0, 71);
  contrast = map(rotateCounter, 0, 50, 80, 180);
  u8g2.setContrast(contrast);
}

void executeContrastMenu() {
  if (mainButton.pressed()) {
    contrast_menu = false;
    setRotary(2);
    menuItemSelect = 2;
    eprom.update(11, contrast);
    //soundSystem.play(6);
    contrast_to_displayTransition();
  }
}

void display_to_contrastTransition() {
  int speed = 2;

  int pageTransition = 128;
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

    u8g2.drawXBM(contrastIcon1, 50, 8, 8, contrast_low_BM);
    u8g2.drawXBM(contrastIcon2, 50, 8, 8, contrast_high_BM);
    u8g2.drawFrame(contrastFrame, 50, 75, 8);
    u8g2.drawBox(contrastBox, 52, contrastBarSize, 4);

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
    delayMicroseconds(100);
  } while (pageTransition > 0);
}

void contrast_to_displayTransition() {
  int speed = 2;

  int pageTransition = -128;
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

    u8g2.drawXBM(contrastIcon1, 50, 8, 8, contrast_low_BM);
    u8g2.drawXBM(contrastIcon2, 50, 8, 8, contrast_high_BM);
    u8g2.drawFrame(contrastFrame, 50, 75, 8);
    u8g2.drawBox(contrastBox, 52, contrastBarSize, 4);

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
    delayMicroseconds(100);
  } while (pageTransition < 0);
}

////////////////////////////////////////////////////////////////////////////



//Screensaver
////////////////////////////////////////////////////////////////////////////
void printScreensaverTime() {
  u8g2.clearBuffer();
  drawDisplayBar();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  u8g2.setFont(u8g2_font_profont11_tf);
  u8g2.drawUTF8(6, 27, screensaverTitles[0]);
  u8g2.drawUTF8(6, 43, screensaverTitles[1]);
  u8g2.drawUTF8(6, 59, screensaverTitles[2]);
  u8g2.drawUTF8(6, 75, screensaverTitles[3]);
  u8g2.drawUTF8(6, 91, screensaverTitles[4]);
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
  switch (screensaverInterval) {
    case 600000:
      u8g2.drawXBM(117, 19, 7, 9, validate_BM);
      break;

    case 1800000:
      u8g2.drawXBM(117, 35, 7, 9, validate_BM);
      break;

    case 3600000:
      u8g2.drawXBM(117, 51, 7, 9, validate_BM);
      break;

    case 100:
      u8g2.drawXBM(117, 67, 7, 9, validate_BM);
      break;
  }
  u8g2.sendBuffer();
}

void updateScreensaverTime() {
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

void executeScreensaverTime() {
  if (mainButton.pressed()) {
    switch (rotateCounter) {
      case 1:
        screensaverState = true;
        screensaverInterval = 600000;
        eprom.update(14, 6);
        break;

      case 2:
        screensaverState = true;
        screensaverInterval = 1800000;
        eprom.update(14, 18);
        break;

      case 3:
        screensaverState = true;
        screensaverInterval = 3600000;
        eprom.update(14, 36);
        break;

      case 4:
        screensaverState = false;
        screensaverInterval = 100;
        eprom.update(14, 100);
        break;

      case 5:
        screensaver_menu = false;
        setRotary(3);
        //screensaver_to_DisplayTransition();
        break;
    }
  }
}